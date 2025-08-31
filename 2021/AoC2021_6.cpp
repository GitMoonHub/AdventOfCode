#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>

#define PART_ONE
// #define PART_TWO

class LanternFish {

public:
    int days = -1;

    LanternFish(int d) {
        days = d;
    }

    bool Tick() {
        --days;
        if(days == -1) {
            days = 6;
            return true;
        }
        return false;
    }
};

class NewbornCluster {
public:
    NewbornCluster(u_int64_t c) {
        count = c;
    }
    bool Tick() {
        --days;
        if(days == -1) {
            return true;
        }
        return false;
    }
    u_int64_t days = 8;
    u_int64_t count = 0;
};

void ReadInput(std::string path);
void SimulateTheOcean(int days);

#ifdef PART_ONE
std::vector<LanternFish> fishes;
std::vector<LanternFish> newborns;
#endif
#ifdef PART_TWO
u_int64_t cycle_0 = 0;
u_int64_t cycle_1 = 0;
u_int64_t cycle_2 = 0;
u_int64_t cycle_3 = 0;
u_int64_t cycle_4 = 0;
u_int64_t cycle_5 = 0;
u_int64_t cycle_6 = 0;
std::list<NewbornCluster> newborns;
#endif
int cycle = 0;

int main() {

    
    ReadInput("AoC2021_6_input.txt");
    SimulateTheOcean(80);
    return 0;
}


void ReadInput(std::string path) {
    std::ifstream file(path.c_str());
    char c;

    if(!file.is_open()) {
        std::cout << "Coudln't read file.\n";
        exit(-1);
    }

    while(file.get(c)) {
        if(c == ',') {
            continue;
        }
        int days = c - '0'; // ASCII value substraction to get integer value
        #ifdef PART_ONE
        fishes.push_back(LanternFish(days));
        #endif
        #ifdef PART_TWO
        switch(days) {
            case 0: cycle_0++; break;
            case 1: cycle_1++; break;
            case 2: cycle_2++; break;
            case 3: cycle_3++; break;
            case 4: cycle_4++; break;
            case 5: cycle_5++; break;
            case 6: cycle_6++; break;
            default: exit(-1);
        }
        #endif
    }
}

void SimulateTheOcean(int days) {

#ifdef PART_ONE
    for(int d = 0; d < days; d++) {
        for(int i = 0; i < fishes.size(); i++) {
            LanternFish& fish = fishes[i];

            if(fish.Tick()) {
                newborns.push_back(LanternFish(8));
            }
        }
        for(int f = 0; f < newborns.size(); f++) {
            fishes.push_back(newborns[f]);
        }
        newborns.clear();
    }

    std::cout << fishes.size() << std::endl;
#endif
#ifdef PART_TWO
    for(int d = 0; d < days; d++) {
    u_int64_t newbornsTemp;

        // Tick down the first cycle of the newborns
        if(!newborns.empty()) {
            for(std::list<NewbornCluster>::iterator it = newborns.begin(); it != newborns.end();) {
                NewbornCluster& cluster = *it;

                // If end-of-cycle newborn, sort into cycle group
                if(cluster.Tick()) {                

                    switch(cycle) {
                        case 0: cycle_0 += cluster.count;  break;
                        case 1: cycle_1 += cluster.count;  break;
                        case 2: cycle_2 += cluster.count;  break;
                        case 3: cycle_3 += cluster.count;  break;
                        case 4: cycle_4 += cluster.count;  break;
                        case 5: cycle_5 += cluster.count;  break;
                        case 6: cycle_6 += cluster.count;  break;
                    }
                    it = newborns.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        u_int64_t* c = nullptr;
        switch(cycle) {            
            case 0: c = &cycle_0; break;
            case 1: c = &cycle_1; break;
            case 2: c = &cycle_2; break;
            case 3: c = &cycle_3; break;
            case 4: c = &cycle_4; break;
            case 5: c = &cycle_5; break;
            case 6: c = &cycle_6; cycle = -1; break;
        }

        if(c == nullptr)
            newbornsTemp = 0;
        else
            newbornsTemp = *c;

        // Create the new newborns
        if(newbornsTemp) {
            newborns.push_back(NewbornCluster(newbornsTemp));
        }
        cycle++;
    }

    u_int64_t total = cycle_0 + cycle_1 + cycle_2 + cycle_3 + cycle_4 + cycle_5 + cycle_6;
    for(std::list<NewbornCluster>::iterator it = newborns.begin(); it != newborns.end();) {
        NewbornCluster cluster = *it;
        total += cluster.count;
        it = newborns.erase(it);
    }
    std::cout << total << std::endl;
#endif
}