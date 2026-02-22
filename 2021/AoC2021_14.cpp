#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <assert.h>
#include <map>
#include <cmath>
#include <limits>

enum POLYMER_ENUM{
    NONE = -1,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,X,Y,Z
};

struct Pair {
    Pair() {

    }
    Pair(std::string s, std::string insert) : str(s) {
        leftPair = std::string(str[0] + insert);
        rightPair = std::string(insert + str[1]);
    }

    std::string str;
    std::string leftPair;
    std::string rightPair;
    uint64_t count = 0;
};

std::map<std::string, Pair> pairs;
std::map<std::string, uint64_t> currentCycle;
uint64_t polymerCount[26];

void SimulateCycle(int cycles);
void ReadInput(std::string path);

int main() {
    memset(polymerCount, 0, sizeof(int) * 26);
    ReadInput("AoC2021_14_input.txt");
    SimulateCycle(40);
}   

void SimulateCycle(int cycles) {
    for(int cycle = 0; cycle < cycles; cycle++) {
        std::map<std::string, uint64_t> nextCycle;
        for(auto it = currentCycle.begin(); it != currentCycle.end(); it++) {
            Pair& p = pairs[it->first];
            const u_int64_t count = it->second;
            pairs[p.leftPair].count += count;
            pairs[p.rightPair].count += count;
            nextCycle[p.leftPair] += count;
            nextCycle[p.rightPair] += count;
            p.count -= count;
        }
        currentCycle = nextCycle;
    }

    uint64_t biggest = 0;
    uint64_t smallest = std::numeric_limits<uint64_t>::max();
    
    char bigP = -1;
    char smallP = -1;

    for(auto it = pairs.begin(); it != pairs.end(); it++) {
        Pair& p = it->second;
        POLYMER_ENUM leftPol = static_cast<POLYMER_ENUM>(p.str[0] - 65);
        POLYMER_ENUM rightPol = static_cast<POLYMER_ENUM>(p.str[1] - 65);
        polymerCount[leftPol] += p.count;
        polymerCount[rightPol] += p.count;
    }
    constexpr uint64_t divisor = 2;
    for(int i = 0; i < 26; i++) {
        uint64_t count = 0;
        if(polymerCount[i] % 2 == 1) {
            count = (polymerCount[i] / divisor) + 1;
        }
        else {
            count = polymerCount[i] / divisor;
        }
        
        if(count == 0)
            continue;

            std::cout << static_cast<char>(i + 65) << ": " << count << "\n";
        if(count > biggest) {
            biggest = count;
            bigP = static_cast<char>(i + 65);
        }
        if(count < smallest) {
            smallest = count;
            smallP = static_cast<char>(i + 65);
        }
    }
    std::cout <<  bigP << " - " << smallP << "\n";
    std::cout << biggest - smallest << "\n\n";
}


void ReadInput(std::string path) {
    std::ifstream file;
    file.open(path);

    if(!file.is_open()){
        std::cout << "Failed to open file.\n";
        exit(-1);
    }
    std::string templateString;
    std::getline(file, templateString);
    constexpr int ASCII_offset = 65;
    bool polymerInserted[26];
    memset(polymerInserted, false, sizeof(bool) * 26);

    std::string line;
    std::getline(file, line);
    while(!file.eof()) {
        std::getline(file, line);
        int spacePos = line.find_first_of(' ');
        std::string pair = line.substr(0, spacePos);
        spacePos = line.find_first_of(' ', spacePos+1) + 1;
        std::string insert = line.substr(spacePos);
        if(pairs.find(pair) == pairs.end()) {
            Pair p = Pair(pair, insert);
            pairs[p.str] = p;
        }
    }
    
    for(int i = 0; i < templateString.size()-1; i++) {
        char c = templateString[i];
        std::string pair = std::string(templateString.substr(i, 2));
        pairs[pair].count++;
        currentCycle[pair]++;
    }
}