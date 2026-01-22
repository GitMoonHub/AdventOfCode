#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <array>
#include <vector>

// #define PART_ONE
// #define PART_TWO

enum FoldEnum{
    UP,
    LEFT
};

struct FoldType {
    FoldType(FoldEnum t, int c) : type(t), coord(c) {

    }
    FoldEnum type = UP;
    int coord = -1;
};

struct Point {
    int x = -1;
    int y = -1;
};

struct Paper {
    Paper() {

    }

    std::list<FoldType> folds;
    std::vector<std::vector<bool>> grid;
    int pointCount = 0;
    int foldedXSize = 0;
    int foldedYSize = 0;
};

void Fold();
void ReadInput(std::string path, std::list<Point>& buffer);

int biggestX = 0;
int biggestY = 0;
Paper paper;


int main() {
    std::list<Point> points;
    ReadInput("AoC2021_13_input.txt", points);
    paper.grid = std::vector<std::vector<bool>>(biggestX + 1, std::vector<bool>(biggestY + 1, false));
    paper.foldedXSize = biggestX + 1;
    paper.foldedYSize = biggestY + 1;

    while(!points.empty()) {
        Point p = points.front();
        if(!paper.grid[p.x][p.y]) {
            paper.grid[p.x][p.y] = true;
            paper.pointCount++;
        }
        points.pop_front();
    }
    Fold();
}


void Fold() {

#if defined PART_ONE
    // Fold only once
    FoldType fold = paper.folds.front();

    if(fold.type == UP){
        for(int x = 0; x < paper.grid.size(); x++) {
            for(int y = fold.coord + 1; y < paper.grid[0].size(); y++) {
                
                if(!paper.grid[x][y])
                    continue;

                Point p = Point();
                p.x = x;
                p.y = fold.coord - (y - fold.coord);
                if(paper.grid[p.x][p.y]) {
                    paper.pointCount--;
                }
                paper.grid[x][y] = false;
                paper.grid[p.x][p.y] = true;
            }
        }
    }
    else{
        for(int x = fold.coord + 1; x < paper.grid.size(); x++) {
            for(int y = 0; y < paper.grid[0].size(); y++) {
                
                if(!paper.grid[x][y])
                    continue;

                Point p = Point();
                p.x = fold.coord - (x - fold.coord);
                p.y = y;
                if(paper.grid[p.x][p.y]) {
                    paper.pointCount--;
                }
                paper.grid[x][y] = false;
                paper.grid[p.x][p.y] = true;
            }
        }
    }

    std::cout << paper.pointCount << "\n";
    return;

#elif defined PART_TWO

    while(!paper.folds.empty()) {

        FoldType fold = paper.folds.front();

        if(fold.type == UP){
            for(int x = 0; x < paper.foldedXSize; x++) {
                for(int y = fold.coord + 1; y < paper.foldedYSize; y++) {
                    
                    if(!paper.grid[x][y])
                        continue;

                    Point p = Point();
                    p.x = x;
                    p.y = fold.coord - (y - fold.coord);
                    if(paper.grid[p.x][p.y]) {
                        paper.pointCount--;
                    }
                    paper.grid[x][y] = false;
                    paper.grid[p.x][p.y] = true;
                }
            }
            paper.foldedYSize = fold.coord;
        }
        else{
            for(int x = fold.coord + 1; x < paper.foldedXSize; x++) {
                for(int y = 0; y < paper.foldedYSize; y++) {
                    
                    if(!paper.grid[x][y])
                        continue;

                    Point p = Point();
                    p.x = fold.coord - (x - fold.coord);
                    p.y = y;
                    if(paper.grid[p.x][p.y]) {
                        paper.pointCount--;
                    }
                    paper.grid[x][y] = false;
                    paper.grid[p.x][p.y] = true;
                }
            }
            paper.foldedXSize = fold.coord;
        }
        paper.folds.pop_front();
    }

    for(int y = 0; y < paper.foldedYSize; y++) {
        for(int x = 0; x < paper.foldedXSize; x++) {
            bool b = paper.grid[x][y];
            std::cout << (b ? '#' : '.');
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
    return;
#endif

}

void ReadInput(std::string path, std::list<Point>& buffer) {
    std::ifstream file;
    file.open(path);

    if(!file.is_open()){
        std::cout << "Failed to open file.\n";
        exit(-1);
    }
    std::string line;

    while(!file.eof()) {
        std::getline(file, line);

        if(line.empty())
            continue;

        if(line[0] != 'f') {

            int commaPos = line.find_first_of(',');
            std::string str = line.substr(0, commaPos);
            int x = std::stoi(str);
            if(x > biggestX)
                biggestX = x;

            str = line.substr(commaPos + 1, std::string::npos);
            int y = std::stoi(str);
            if(y > biggestY)
                biggestY = y;

            Point p;
            p.x = x;
            p.y = y;
            buffer.push_back(p);
        }
        else {
            constexpr int offset = 11;
            std::string str = line.substr(offset, std::string::npos);
            const int equalPos = str.find_first_of('=');
            std::string strType = str.substr(0, equalPos);
            std::string strCoord = str.substr(equalPos + 1, std::string::npos);
            const int coord = std::stoi(strCoord);
            FoldEnum fe = UP;
            if(strType == "x") {
                fe = LEFT;
            }
            paper.folds.push_back(FoldType(fe, coord));
        }
    }
}