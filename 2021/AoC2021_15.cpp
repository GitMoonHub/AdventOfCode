#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <tuple>
#include <climits>
#include <set>
#include <cstring>

// Uncomment this for Part Two
//#define PART_TWO

enum DIRECTION {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Point {
    Point(){}
    Point(int x, int y, u_int64_t risk) :
     x(x), y(y), risk(risk) {

    }
    bool operator<(const Point& rhs) const {
        return risk < rhs.risk;
    }

    int x = -1;
    int y = -1;
    u_int64_t risk = -1;
    int64_t f = -1, g = -1, h = -1;
    int parent_x = -1, parent_y = -1;
};

typedef std::pair<int, std::pair<int, int>> PriorityQueueType;

std::vector<std::vector<Point>> cave;
size_t gridW = 0;
size_t gridH = 0;
#ifdef PART_TWO
size_t fullGridW = 0;
size_t fullGridH = 0;
constexpr int divisor = 5;
#endif


u_int64_t AStarSearch(std::vector<std::vector<Point>>& grid, Point& src, Point& dst);
int ComputeManhattanDist(int x, int y, Point& d);
bool isValid(int x, int y);
bool isDestination(int x, int y, Point& dst);
void ReadInput(std::string filepath);
#ifdef PART_TWO
void ComputeFullGrid(std::vector<std::vector<Point>>& grid);
#endif

int main() {
    ReadInput("AoC2021_15_input.txt");
    #ifdef PART_TWO
    ComputeFullGrid(cave);
    u_int64_t cost = AStarSearch(cave, cave[0][0], cave[fullGridW-1][fullGridH-1]);
    #else
    u_int64_t cost = AStarSearch(cave, cave[0][0], cave[gridW-1][gridH-1]);
    #endif

    std::cout << cost << std::endl;
}

u_int64_t AStarSearch(std::vector<std::vector<Point>>& grid, Point& src, Point& dst) {
    if(!isValid(src.x, src.y)) {
        std::cout << "Src is not a valid cell.\n"; 
        exit(-1);
    }

    if(!isValid(dst.x, dst.y)) {
        std::cout << "Dst is not a valid cell.\n";
        exit(-1);
    }
#ifdef PART_TWO
    const int row = fullGridW;
    const int col = fullGridH;
#else
    const int row = grid.size();
    const int col = grid[0].size();
#endif
    // Initialize the whole grid to max values
    for(int x = 0; x < grid.size(); x++) {
        for(int y = 0; y < grid[0].size(); y++) {
            grid[x][y].f = INT_MAX;
            grid[x][y].g = INT_MAX;
            grid[x][y].h = INT_MAX;
            grid[x][y].parent_x = -1;
            grid[x][y].parent_y = -1;
        }
    }

    // Initialize source to 0
    grid[0][0].f = 0;
    grid[0][0].g = 0;
    grid[0][0].h = 0;

    grid[dst.x][dst.y].parent_x = dst.x;
    grid[dst.x][dst.y].parent_y = dst.y;

    std::set<PriorityQueueType> open;
    open.insert(std::make_pair(0, std::make_pair(src.x, src.y)));
    bool foundDst = false;
    bool closed[row][col];
    memset(closed, false, sizeof(closed));
    

    while(!open.empty()) {
        PriorityQueueType cell = *open.begin();
        open.erase(open.begin());
        const int& x = cell.second.first;
        const int& y = cell.second.second;
        const u_int offset_x = 0;
        const u_int offset_y = 0;
        closed[x][y] = true;

        u_int64_t gNew = 0, fNew = 0, hNew = 0;
        // Inspect North point
        if(isValid(x, y - 1)) {
            // Check if we reached destination
            if(isDestination(x, y - 1, dst)) {
                foundDst = true;
                grid[x][y - 1].parent_x = x;
                grid[x][y - 1].parent_y = y;
                grid[x][y - 1].g = grid[x][y].g + grid[x][y - 1].risk;
                break;
            }
            // Check if targeted point is still available
            else if(!closed[x][y - 1]) {
                gNew = grid[x][y].g + grid[x][y - 1].risk;
                hNew = ComputeManhattanDist(x, y - 1, dst);
                fNew = gNew + hNew;

                if(grid[x][y - 1].f == INT_MAX || grid[x][y - 1].f > fNew) {
                    grid[x][y - 1].f = fNew;
                    grid[x][y - 1].g = gNew;
                    grid[x][y - 1].h = hNew;
                    grid[x][y - 1].parent_x = x;
                    grid[x][y - 1].parent_y = y;
                    open.insert(std::make_pair(fNew, std::make_pair(x, y - 1)));
                }
            }
        }

        // Inspect South
         if(isValid(x, y + 1)) {
            // Check if we reached destination
            if(isDestination(x, y + 1, dst)) {
                foundDst = true;
                grid[x][y + 1].parent_x = x;
                grid[x][y + 1].parent_y = y;
                grid[x][y + 1].g = grid[x][y].g + grid[x][y + 1].risk;
                break;
            }
            // Check if targeted point is still available
            else if(!closed[x][y + 1]) {
                gNew = grid[x][y].g + grid[x][y + 1].risk;
                hNew = ComputeManhattanDist(x, y + 1, dst);
                fNew = gNew + hNew;

                if(grid[x][y + 1].f == INT_MAX || grid[x][y + 1].f > fNew) {
                    grid[x][y + 1].f = fNew;
                    grid[x][y + 1].g = gNew;
                    grid[x][y + 1].h = hNew;
                    grid[x][y + 1].parent_x = x;
                    grid[x][y + 1].parent_y = y;
                    open.insert(std::make_pair(fNew, std::make_pair(x, y + 1)));
                }
            }
        }
        // Inspect East
         if(isValid(x + 1, y)) {
            // Check if we reached destination
            if(isDestination(x + 1, y, dst)) {
                foundDst = true;
                grid[x + 1][y].parent_x = x;
                grid[x + 1][y].parent_y = y;
                grid[x + 1][y].g = grid[x][y].g + grid[x + 1][y].risk;
                break;
            }
            // Check if targeted point is still available
            else if(!closed[x + 1][y]) {
                gNew = grid[x][y].g + grid[x + 1][y].risk;
                hNew = ComputeManhattanDist(x + 1, y, dst);
                fNew = gNew + hNew;

                if(grid[x + 1][y].f == INT_MAX || grid[x + 1][y].f > fNew) {
                    grid[x + 1][y].f = fNew;
                    grid[x + 1][y].g = gNew;
                    grid[x + 1][y].h = hNew;
                    grid[x + 1][y].parent_x = x;
                    grid[x + 1][y].parent_y = y;
                    open.insert(std::make_pair(fNew, std::make_pair(x + 1, y)));
                }
            }
        }
        // Inspect West
        if(isValid(x - 1, y)) {
            // Check if we reached destination
            if(isDestination(x - 1, y, dst)) {
                foundDst = true;
                grid[x - 1][y].parent_x = x;
                grid[x - 1][y].parent_y = y;
                grid[x - 1][y].g = grid[x][y].g + grid[x - 1][y].risk;
                break;
            }
            // Check if targeted point is still available
            else if(!closed[x - 1][y]) {
                gNew = grid[x][y].g + grid[x - 1][y].risk;
                hNew = ComputeManhattanDist(x - 1, y, dst);
                fNew = gNew + hNew;

                if(grid[x - 1][y].f == INT_MAX || grid[x - 1][y].f > fNew) {
                    grid[x - 1][y].f = fNew;
                    grid[x - 1][y].g = gNew;
                    grid[x - 1][y].h = hNew;
                    grid[x - 1][y].parent_x = x;
                    grid[x - 1][y].parent_y = y;
                    open.insert(std::make_pair(fNew, std::make_pair(x - 1, y)));
                }
            }
        }
    }
    if(!foundDst) {
        std::cout << "Failed to reach destination.\n";
        exit(-1);
    }

    return grid[dst.x][dst.y].g;
}

#ifdef PART_TWO
bool isValid(int x, int y) {
    return (x >= 0) && (x < fullGridW) && (y >= 0) && (y < fullGridH);
}
#else
bool isValid(int x, int y) {
    return (x >= 0) && (x < gridW) && (y >= 0) && (y < gridH);
}
#endif

bool isDestination(int x, int y, Point& d) {
    return ((x == d.x) && (y == d.y));
}

int ComputeManhattanDist(int x, int y, Point& d) {
    return abs(x - d.x) + abs(y - d.y);
}

void ReadInput(std::string filepath) {
    std::ifstream file;
    file.open(filepath);
    if(!file.is_open()) {
        std::cout << "Couldn't open file.\n";
        exit(-1);
    }

    std::string line;
    int y = 0;
    constexpr char ASCII_offset = 48; // '0' - ASCII_offset = 0
    while(!file.eof()) {
        std::getline(file, line);
        cave.push_back(std::vector<Point>(line.size()));
#ifdef PART_TWO
        cave.back().resize(line.size() * divisor);
#endif
        for(int i = 0; i < line.size(); i++) {
            int r = line[i] - ASCII_offset;
            Point p = Point(i, y, r);
            cave[y][i] = p;
        }
        y++;
    }
    gridW = line.size();
    gridH = y;
#ifdef PART_TWO
    cave.resize(y * divisor, std::vector<Point>(gridW * divisor));
#endif
}

#ifdef PART_TWO
void ComputeFullGrid(std::vector<std::vector<Point>>& grid) {
    fullGridW = gridW * 5;
    fullGridH = gridH * 5;
    for(int x = 0; x < gridW; x++) {
        for(int y = 0; y < gridH; y++) {
            for(int xo = 0; xo < 5; xo++) {
                for(int yo = 0; yo < 5; yo++){
                    if(xo == 0 && yo == 0)
                        continue;
                    int val = grid[x][y].risk + xo + yo;
                    if(val > 9) {
                        val -= 9;
                    }
                    grid[x + xo * gridW][y + yo * gridH].risk = val;
                    grid[x + xo * gridW][y + yo * gridH].x = x + xo * gridW;
                    grid[x + xo * gridW][y + yo * gridH].y = y + yo * gridH;
                }
            }
        }
    }
}
#endif