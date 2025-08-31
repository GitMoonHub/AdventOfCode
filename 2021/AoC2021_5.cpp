#include <string>
#include <fstream>
#include <regex>
#include <iostream>

#define PART_ONE
// #define PART_TWO

struct Point {
    Point(){}
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int x = 0;
    int y = 0;
};

struct Line {
    Line(){}
    Line(Point p1, Point p2){
        this->p1 = p1;
        this->p2 = p2;
    }
    Point p1;
    Point p2;
};

void ReadInput(std::string path, std::vector<Line>& lines, std::size_t& array_size);
void PrintOutVents(std::vector<std::vector<u_int>>& vents);
void LogGridToFile(std::vector<std::vector<u_int>>& vents, std::string file);

int main() {
    std::vector<Line> lines;
    std::string PATH("AoC2021_5_input.txt");

    std::size_t array_size = 0;
    ReadInput(PATH, lines, array_size);
    std::vector<std::vector<u_int>> vents(array_size, std::vector<u_int>(array_size));

#ifdef PART_ONE
// PART 1
    // Fill out the map
    for(int i = 0; i < lines.size(); i++) {
        Point& p1 = lines[i].p1;
        Point& p2 = lines[i].p2;
        int delta = 0;
        int x = (p1.x > p2.x ? p2.x : p1.x);
        int y = (p1.y > p2.y ? p2.y : p1.y);
        bool horizontal = false;
        
        // Check for diagonal line
        if(p1.x != p2.x && p1.y != p2.y){
            continue;
        }

        if(p1.x == p2.x) {
            delta = abs(p1.y - p2.y);
        }
        else if(p1.y == p2.y) {
            delta = abs(p1.x - p2.x);
            horizontal = true;
        }


        for(int j = 0; j <= delta; j++) {
            int x_index = horizontal ? x + j : x;
            int y_index = horizontal ? y : y + j;

            vents[x_index][y_index] += 1;
        }
    }
    #endif

    #ifdef PART_TWO
// PART 2
    // Fill out the map
    for(int i = 0; i < lines.size(); i++) {
        Point& p1 = lines[i].p1;
        Point& p2 = lines[i].p2;
        int delta = 0;
        int x = (p1.x > p2.x ? p2.x : p1.x);
        int y = (p1.y > p2.y ? p2.y : p1.y);
        bool horizontal = false;
        bool diagonal = false;
        bool dy_dec = false;

        // Check for diagonal line
        if((p1.x != p2.x && p1.y != p2.y) || (p1.x == p2.x && p1.y == p2.y)){
            diagonal = true;
        }

        if(diagonal) {
            // This is disgusting lol
            if(p1.x < p2.x) {
                if(p1.y > p2.y){
                    dy_dec = true;
                }
                y = p1.y;
            }
            else if(p2.x < p1.x) {
                if(p2.y > p1.y) {
                    dy_dec = true;
                }
                y = p2.y;
            }
            delta = abs(p2.x - p1.x);
        }
        else if(p1.x == p2.x) {
            delta = abs(p1.y - p2.y);
        }
        else if(p1.y == p2.y) {
            delta = abs(p1.x - p2.x);
            horizontal = true;
        }


        for(int j = 0; j <= delta; j++) {
            int x_index = horizontal ? x + j : x;
            int y_index = horizontal ? y : y + j;

            if(diagonal) {
                x_index = x + j;
                if(dy_dec){
                    y_index = y - j;
                } 
                else {
                    y_index = y + j;
                }
            }    
            vents[x_index][y_index] += 1;
        }
    }
#endif
    u_int POI = 0;
    for(int x = 0; x < array_size; x++){
        for(int y = 0; y < array_size; y++){
            if(vents[x][y] >= 2){
                POI++;
            }
        }
    }

   std::cout << POI << " POIs" << std::endl;
   exit(0);
}

void ReadInput(std::string path, std::vector<Line>& lines, std::size_t& array_size) {
    std::ifstream file(path.c_str());
    std::string input;
    std::smatch m;
    std::regex regex("([0-9]+)");
    
    // Read the data into a vector and find the array size
    while(!file.eof()){
        std::getline(file, input);
        std::array<int, 4> a;
        int counter = 0;
        while(std::regex_search(input, m, regex)){
            int num = std::stoi(m.str());
            if(num > array_size) {
                array_size = num;
            }
            a[counter] = num;
            counter++;
            input = m.suffix();
        }
    
        Line l = Line(Point(a[0], a[1]), Point(a[2], a[3]));
        lines.push_back(l);
    }
    file.close();
    array_size++;   // Important because of C++ is 0 indexed
}

void PrintOutVents(std::vector<std::vector<u_int>>& vents) {
    for(int y = 0; y < vents.size(); y++){
        for(int x = 0; x < vents[0].size(); x++) {
            int i = vents[x][y];
            if(i == 0) {
                std::cout << ". ";
            }
            else {
                std::cout << vents[x][y] << " ";
            }
        }
        std::cout << "\n";
    }
    std::string s;
    getline(std::cin, s);
}

void LogGridToFile(std::vector<std::vector<u_int>>& vents, std::string path) {
    std::filebuf fb;
    fb.open(path.c_str(), std::ios::out);
    std::ostream os(&fb);

    for(int y = 0; y < vents.size(); y++){
        for(int x = 0; x < vents[0].size(); x++){
            std::string n = std::to_string(vents[x][y]);
            if(n == "0"){
                os << ". ";
            }else{
                os << n << ' ';
            }
        }
        os << '\n';
    }
    os << "\n\n";
    fb.close();
}