#include <iostream>
#include  <fstream>
#include <array>
#include <string>
#include <assert.h>
#include <list>

// #define DEBUG_AOC
// #define PART_ONE
// #define PART_TWO

struct Dumbo {
    Dumbo() {

    }
    Dumbo(int e, int x, int y) : energy(e), x(x), y(y) {

    }
    int x = -1;
    int y = -1;
    int energy = -1;
    bool flashed = false;
};

uint64_t totalFlashes = 0;
std::array<std::array<Dumbo, 10>, 10> dumbos;
std::list<Dumbo*> flashing;
std::list<Dumbo*> needReset;

void ReadInput(std::array<std::array<Dumbo, 10>, 10>& arr, std::string path);
void PartOne();
void PartTwo();
void Flash(Dumbo& d);
void CheckForFlash(Dumbo& d);
void PrintState();



int main() {
    ReadInput(dumbos, "AoC2021_11_input.txt");

#ifdef PART_ONE
    PartOne();
#endif

#ifdef PART_TWO
    PartTwo();
#endif
    return 0;
}

void ReadInput(std::array<std::array<Dumbo, 10>, 10>& arr, std::string path) {
    std::ifstream file(path);

    if(!file.is_open()) {
        std::cout << "Failed to open stream.\n";
        exit(-1);
    }
    constexpr int asciiOffset = 48;
    int lineCounter = 0;
    while(!file.eof()) {
        std::string line;
        std::getline(file, line);
        assert(line.size() == 10);
        for(int i = 0; i < line.size(); i++) {
            char c = line[i] - asciiOffset;
            Dumbo d = Dumbo(c, i, lineCounter);
            arr[lineCounter][i] = d;
        }
        lineCounter++;
    }
}

void PartOne() {
    for(int step = 0; step < 100; step++) {
        for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 10; x++) {
                Dumbo& d = dumbos[y][x];
                d.energy++;
                CheckForFlash(d);
            }
        }

        while(!flashing.empty()) {
            Dumbo* d = flashing.front();
            Flash(*d);
            flashing.pop_front();
        }

#ifdef DEBUG_AOC
        PrintState();
#endif

        while(!needReset.empty()){ 
            Dumbo* d = needReset.front();
            d->energy = 0;
            d->flashed = false;
            needReset.pop_front();
            totalFlashes++;
        }
    }
    std::cout << totalFlashes << std::endl;
}

void PartTwo() {
    uint64_t step = 0;
    while(true) {
        for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 10; x++) {
                Dumbo& d = dumbos[y][x];
                d.energy++;
                CheckForFlash(d);
            }
        }

        while(!flashing.empty()) {
            Dumbo* d = flashing.front();
            Flash(*d);
            flashing.pop_front();
        }

#ifdef DEBUG_AOC
        PrintState();
#endif

        step++;
        if(needReset.size() == 100) {
            std::cout << step << "\n";
            exit(0);
        }
        while(!needReset.empty()){ 
            Dumbo* d = needReset.front();
            d->energy = 0;
            d->flashed = false;
            needReset.pop_front();
        }
    }
}

void Flash(Dumbo& d) {
    bool loob = false;  // left out-of-bounds
    bool toob = false;  // top
    bool roob = false;  // right
    bool boob = false;  // lol

    if(d.x < 1)
        loob = true;

    if(d.x >= 9)
        roob = true;

    if(d.y < 1)
        toob = true;
    
    if(d.y >= 9)
        boob = true;

    Dumbo* neighbour = nullptr;

    // Top row
    if(!toob) {
        if(!loob) {
            neighbour = &dumbos[d.y-1][d.x-1];  // upLeft
            neighbour->energy++;
            CheckForFlash(*neighbour);
        }

        neighbour = &dumbos[d.y-1][d.x];  // up
        neighbour->energy++;
        CheckForFlash(*neighbour);

        if(!roob) {
            neighbour = &dumbos[d.y-1][d.x+1];  // upRight
            neighbour->energy++;
            CheckForFlash(*neighbour);
        }
    }

    // Middle row
    if(!loob) {
        neighbour = &dumbos[d.y][d.x-1];  // left
        neighbour->energy++;
        CheckForFlash(*neighbour);
    }

    if(!roob) {
        neighbour = &dumbos[d.y][d.x+1];  // right
        neighbour->energy++;
        CheckForFlash(*neighbour);
    }

    // Bottom row
    if(!boob) {
        if(!loob) {
            neighbour = &dumbos[d.y+1][d.x-1];  // botLeft
            neighbour->energy++;
            CheckForFlash(*neighbour);
        }

        neighbour = &dumbos[d.y+1][d.x];  // bot
        neighbour->energy++;
        CheckForFlash(*neighbour);

        if(!roob) {
            neighbour = &dumbos[d.y+1][d.x+1];  // botRight
            neighbour->energy++;
            CheckForFlash(*neighbour);
        }
    }
}

void CheckForFlash(Dumbo& d) {

    if(d.energy > 9 && !d.flashed) {
        d.flashed = true;
        flashing.push_back(&d);
        needReset.push_back(&d);
    }
}

void PrintState() {
    for(int y = 0; y < 10; y++) {
        for(int x = 0; x < 10; x++) {
            if(dumbos[y][x].flashed) {
                std::cout << '.';   // flashed this step
            } else {
                std::cout << dumbos[y][x].energy;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}
