#include <iostream>
#include <fstream>
#include <array>
#include <tuple>
#include <vector>
#include <string>
#include <assert.h>

#define PART_ONE
// #define PART_TWO
// #define DEBUG_AOC

enum DIRECTION {
    NONE,
    LEFT,
    TOP,
    RIGHT,
    BOTTOM
};

struct Point {
    Point();
    Point(int v, int x, int y): value(v), x(x), y(y) {
        
    }
    int value = -1;
    int x = -1;
    int y = -1;
    int basinId = -1;
    bool linked = false;
};

struct Basin {
    Basin();
    Basin(bool b) {
        if(b) {
            id = idCounter;
            idCounter++;
        }
    }
    static int idCounter;
    int id = -1;
    int size = 1;
};

int Basin::idCounter = 0;

void ReadInput(std::vector<std::vector<Point>>& v, std::string path);
#ifdef PART_ONE
void LowestPointFromRoot(Point& p, DIRECTION from);
void PartOne();
#endif
#ifdef PART_TWO
int LowestPointFromRoot(Point& p, DIRECTION from);
void PartTwo();
#endif

void TopDownMergeSort(std::vector<Basin>& A, std::vector<Basin>& B);
void TopDownSplitMerge(std::vector<Basin>& B, int iBegin, int iEnd, std::vector<Basin>& A);
void TopDownMerge(std::vector<Basin>& B, int iBegin, int iMiddle, int iEnd, std::vector<Basin>& A);
#ifdef DEBUG_AOC
void PrintBasins();
#endif

std::vector<std::vector<Point>> points;
std::vector<Basin> basins;
int riskLevelTotal = 0;


int main() {
    ReadInput(points, "AoC2021_9_input.txt");
    // ReadInput(points, "AoC2021_9_testFile.txt");
    #ifdef PART_ONE
    PartOne();
    #endif
    #ifdef PART_TWO
    PartTwo();
    #endif
}

void ReadInput(std::vector<std::vector<Point>>& v, std::string path) {
    std::ifstream file;
    file.open(path);
    int lineCounter = 0;
    while(!file.eof()){
        std::string line;
        std::getline(file, line);
        std::vector<Point> v2;
        v2.reserve(line.size());
        constexpr int asciiOffset = 48;
        for(int i = 0; i < line.length(); i++) {
            char c = line[i];
            Point p = Point(c - asciiOffset, i, lineCounter);
            v2.push_back(p);
        }
        v.push_back(v2);
        ++lineCounter;
    }
}
#ifdef PART_ONE
void LowestPointFromRoot(Point& p, DIRECTION from) {

    const bool leftInBound = p.x > 0 ? true : false;
    const bool topInBound = p.y > 0 ? true : false;
    const bool rightInBound = p.x < points[p.y].size() - 1 ? true : false;
    const bool botInBound = p.y < points.size() - 1 ? true : false;

    DIRECTION to = NONE;
    int lowestValue = 9999999;

    if(p.value == 0 && !p.linked) {
        riskLevelTotal++;
        const int x = p.x;
        const int y = p.y;
        
        if(leftInBound)
            points[y][x-1].linked = true;
        
        if(topInBound)
            points[y-1][x].linked = true;
        
        if(rightInBound)
            points[y][x+1].linked = true;
        
        if(botInBound)
            points[y+1][x].linked = true;

        p.linked = true;
        return;
    }

    // Find where the smallest neighbour is
    
    Point* nextPoint = nullptr;
    if(from != DIRECTION::LEFT && leftInBound) {
        nextPoint = &points[p.y][p.x - 1];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::LEFT;
                lowestValue = nextPoint->value;
            }
        }
    }

    if(from != DIRECTION::TOP && topInBound) {
        nextPoint = &points[p.y - 1][p.x];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::TOP;
                lowestValue = nextPoint->value;
            }
        }
    }

    if(from != DIRECTION::RIGHT && rightInBound) {
        nextPoint = &points[p.y][p.x + 1];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::RIGHT;
                lowestValue = nextPoint->value;
            }
        }
    }

    if(from != DIRECTION::BOTTOM && botInBound) {
        nextPoint = &points[p.y + 1][p.x];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::BOTTOM;
                lowestValue = nextPoint->value;
            }
        }
    }

    // Find if the smallest neighbour is already part of a chain to a lowest point
    // If no neighbour had a lower value than the current point, we found a deadend
    // If the smallest neighbour in the given direction is linked, then our current point joined a solved chain
    // If the smallest neighbour in the given direction in unlinked, we found the next point in our current chain
    nextPoint = nullptr;
    switch(to) {
        case NONE: {

            assert(!p.linked);
            
            Point* left = nullptr;
            Point* top = nullptr;
            Point* right = nullptr;
            Point* bot = nullptr;
            if(leftInBound)
                left = &points[p.y][p.x-1];

            if(topInBound)
                top = &points[p.y-1][p.x];

            if(rightInBound)
                right = &points[p.y][p.x+1];

            if(botInBound)
                bot = &points[p.y+1][p.x];

            p.linked = true;

            if(left && left->value == p.value && left->linked)
                return;

            if(top && top->value == p.value && top->linked)
                return;

            if(right && right->value == p.value && right->linked)
                return;

            if(bot && bot->value == p.value && bot->linked)
                return;

            // Found a lowest point
            riskLevelTotal += 1 + p.value;
            
            if(leftInBound)
                points[p.y][p.x-1].linked = true;

            if(topInBound)
                points[p.y-1][p.x].linked = true;
            
            if(rightInBound)
                points[p.y][p.x+1].linked = true;
            
            if(botInBound)
                points[p.y+1][p.x].linked = true;

            return;
        }

        case LEFT:
            assert(leftInBound);
            nextPoint = &points[p.y][p.x-1];
            p.linked = true;
            if(!nextPoint->linked) {
                LowestPointFromRoot(*nextPoint, DIRECTION::RIGHT);
            }
            return;

        case TOP: 
            assert(topInBound);
            nextPoint = &points[p.y-1][p.x];
            p.linked = true;
            if(!nextPoint->linked) {
                LowestPointFromRoot(*nextPoint, DIRECTION::BOTTOM);
            }
            return;

        case RIGHT: 
            assert(rightInBound);
            nextPoint = &points[p.y][p.x+1];
            p.linked = true;
            if(!nextPoint->linked) {
                LowestPointFromRoot(*nextPoint, DIRECTION::LEFT);
            }
            return;

        case BOTTOM: 
            assert(botInBound);
            nextPoint = &points[p.y+1][p.x];
            p.linked = true;
            if(!nextPoint->linked) {
                LowestPointFromRoot(*nextPoint, DIRECTION::TOP);
            }
            return;        
    }
}
#endif

#ifdef PART_TWO
int LowestPointFromRoot(Point& p, DIRECTION from) {

    const bool leftInBound = p.x > 0 ? true : false;
    const bool topInBound = p.y > 0 ? true : false;
    const bool rightInBound = p.x < points[p.y].size() - 1 ? true : false;
    const bool botInBound = p.y < points.size() - 1 ? true : false;

    DIRECTION to = NONE;
    int lowestValue = 9999999;

    if(p.value == 9)
        return -1;

    if(p.value == 0 && !p.linked) {
        Basin basin = Basin(true);
        p.basinId = basin.id;
        
        const int x = p.x;
        const int y = p.y;
        
        if(leftInBound && from != DIRECTION::LEFT && points[y][x-1].value < 9) {
            points[y][x-1].linked = true;
            points[y][x-1].basinId = basin.id;
            basin.size++;
        }
        
        if(topInBound && from != DIRECTION::TOP && points[y-1][x].value < 9) {
            points[y-1][x].linked = true;
            points[y-1][x].basinId = basin.id;
            basin.size++;
        }
        
        if(rightInBound && from != DIRECTION::RIGHT && points[y][x+1].value < 9) {
            points[y][x+1].linked = true;
            points[y][x+1].basinId = basin.id;
            basin.size++;
        }
        
        if(botInBound && from != DIRECTION::BOTTOM && points[y+1][x].value < 9) {
            points[y+1][x].linked = true;
            points[y+1][x].basinId = basin.id;
            basin.size++;
        }

        p.linked = true;
        basins.push_back(basin);
        return basin.id;
    }

    // Find where the smallest neighbour is
    
    Point* nextPoint = nullptr;
    if(from != DIRECTION::LEFT && leftInBound) {
        nextPoint = &points[p.y][p.x - 1];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::LEFT;
                lowestValue = nextPoint->value;
            }
        }
    }

    if(from != DIRECTION::TOP && topInBound) {
        nextPoint = &points[p.y - 1][p.x];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::TOP;
                lowestValue = nextPoint->value;
            }
        }
    }

    if(from != DIRECTION::RIGHT && rightInBound) {
        nextPoint = &points[p.y][p.x + 1];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::RIGHT;
                lowestValue = nextPoint->value;
            }
        }
    }

    if(from != DIRECTION::BOTTOM && botInBound) {
        nextPoint = &points[p.y + 1][p.x];
        if(nextPoint->value < p.value) {
            if(lowestValue > nextPoint->value) {
                to = DIRECTION::BOTTOM;
                lowestValue = nextPoint->value;
            }
        }
    }

    nextPoint = nullptr;
    switch(to) {
        case NONE: {

            assert(!p.linked);
            
            Point* left = nullptr;
            Point* top = nullptr;
            Point* right = nullptr;
            Point* bot = nullptr;
            if(leftInBound)
                left = &points[p.y][p.x-1];

            if(topInBound)
                top = &points[p.y-1][p.x];

            if(rightInBound)
                right = &points[p.y][p.x+1];

            if(botInBound)
                bot = &points[p.y+1][p.x];

            p.linked = true;
            
            Basin basin = Basin(true);
            p.basinId = basin.id;

            basins.push_back(basin);
            return basin.id;
        }

        case LEFT: {

            assert(leftInBound);
            nextPoint = &points[p.y][p.x-1];
            p.linked = true;
            int basinId = -1;
            if(!nextPoint->linked) {
                basinId = LowestPointFromRoot(*nextPoint, DIRECTION::RIGHT);
            }
            else {
                basinId = nextPoint->basinId;
            }
            assert(basinId != -1);
            p.basinId = basinId;
            basins[basinId].size++;
            
            return basinId;
        }

        case TOP: {

            assert(topInBound);
            nextPoint = &points[p.y-1][p.x];
            p.linked = true;
            int basinId = -1;
            if(!nextPoint->linked) {
                basinId = LowestPointFromRoot(*nextPoint, DIRECTION::BOTTOM);
            }
            else {
                basinId = nextPoint->basinId;
            }
            assert(basinId != -1);
            p.basinId = basinId;
            basins[basinId].size++;
            
            return basinId;
        }

        case RIGHT: {

            assert(rightInBound);
            nextPoint = &points[p.y][p.x+1];
            p.linked = true;
            int basinId = -1;
            if(!nextPoint->linked) {
                basinId = LowestPointFromRoot(*nextPoint, DIRECTION::LEFT);
            }
            else {
                basinId = nextPoint->basinId;
            }
            assert(basinId != -1);
            p.basinId = basinId;
            basins[basinId].size++;
            
            return basinId;
        }

        case BOTTOM: {

            assert(botInBound);
            nextPoint = &points[p.y+1][p.x];
            p.linked = true;
            int basinId = -1;
            if(!nextPoint->linked) {
                basinId = LowestPointFromRoot(*nextPoint, DIRECTION::TOP);
            }
            else {
                basinId = nextPoint->basinId;
            }
            assert(basinId != -1);
            p.basinId = basinId;
            basins[basinId].size++;
            
            return basinId;
        }
    }
    return -1;  // Shouldn't ever run
}
#endif
#ifdef PART_ONE
void PartOne() {
    const int ySize = points.size();
    for(int y = 0; y < ySize; y++) {
        const int xSize = points[y].size();
        for(int x = 0; x < xSize; x++) {
            Point& p = points[y][x];
            if(p.linked)
                continue;

            LowestPointFromRoot(p, DIRECTION::NONE);
        }
    }
    std::cout << riskLevelTotal << std::endl;
}
#endif
#ifdef PART_TWO
void PartTwo() {
    const int ySize = points.size();
    for(int y = 0; y < ySize; y++) {
        const int xSize = points[y].size();
        for(int x = 0; x < xSize; x++) {
            Point& p = points[y][x];
            if(p.linked)
                continue;

            LowestPointFromRoot(p, DIRECTION::NONE);
            #ifdef DEBUG_AOC
            PrintBasins();
            #endif
        }
    }

    std::vector<Basin> prime;
    TopDownMergeSort(basins, prime);

    int product = -1;
    for(int i = 0; i < 3; i++) {
        int val = basins[i].size;
        product *= val;
    }
    std::cout << product << std::endl;
}
#endif

#ifdef DEBUG_AOC
void PrintBasins() {
    const int ySize = points.size();
    for(int y = 0; y < ySize; y++) {
        const int xSize = points[y].size();
        for(int x = 0; x < xSize; x++) {
            Point& p = points[y][x];
            switch(p.basinId) {
                case -1: std::cout << "."; break;
                default: std::cout << p.basinId; break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}
#endif

// https://en.wikipedia.org/wiki/Merge_sort#Top-down_implementation
void TopDownMergeSort(std::vector<Basin>& A, std::vector<Basin>& B) {
    B = A;
    size_t size = A.size();
    TopDownSplitMerge(A, 0, size, B);
}

void TopDownSplitMerge(std::vector<Basin>& B, int iBegin, int iEnd, std::vector<Basin>& A) {
    // If size = 1, the array is sorted
    if(iEnd - iBegin <= 1)
        return;

    int iMiddle = (iEnd + iBegin) / 2; 
    TopDownSplitMerge(A, iBegin, iMiddle, B);
    TopDownSplitMerge(A, iMiddle, iEnd, B);

    TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

void TopDownMerge(std::vector<Basin>& B, int iBegin, int iMiddle, int iEnd, std::vector<Basin>& A) {
    int i = iBegin;
    int j = iMiddle;

    for (int k = iBegin; k < iEnd; k++) {
        
        if (i < iMiddle && (j >= iEnd || A[i].size >= A[j].size)) {
            B[k] = A[i];
            i = i + 1;
        } else {
            B[k] = A[j];
            j = j + 1;
        }
    }
}