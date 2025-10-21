#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <bitset>
#include <array>
#include <cmath>
#include <map>

#define PART_ONE
#define PART_TWO

enum UNIQUE_SEGMENTS {
    UNIQUE_ONE = 2,
    UNIQUE_SEVEN = 3,
    UNIQUE_FOUR = 4,
    UNIQUE_EIGHT = 7
};

enum NUMBERS {
    NONE = -1,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
};

enum SEGMENT {
    UNKNOWN,
    TOP,
    TOP_LEFT,
    TOP_RIGHT,
    MIDDLE,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    BOTTOM
};

struct Connection {
    NUMBERS number = NUMBERS::NONE;
    std::bitset<7> segments;
};

std::vector<std::multimap<int, Connection>> connections;
std::vector<std::array<Connection, 4>> display;
#ifdef PART_ONE
int uniqueCounter = 0;
#endif

unsigned int totalValue = 0;
#ifdef PART_TWO
void Decode();
#endif
void ReadInput(std::string path);

int main() {

    ReadInput("AoC2021_8_input.txt");
#ifdef PART_TWO
    Decode();
#endif

}
#ifdef PART_TWO
void Decode() {
    Connection* zero = nullptr;
    Connection* one = nullptr;
    Connection* two = nullptr;
    Connection* three = nullptr;
    Connection* four = nullptr;
    Connection* five = nullptr;
    Connection* six = nullptr;
    Connection* seven = nullptr;
    Connection* eight = nullptr;
    Connection* nine = nullptr;

    for(int line = 0; line < connections.size(); line++) {
        std::multimap<int, Connection>& mmap = connections[line];
        std::multimap<int, Connection>::iterator it = mmap.find(1);
        assert(it != mmap.end());

        seven = &mmap.find(7)->second;
        one = &mmap.find(1)->second;
        four = &mmap.find(4)->second;
        eight = &mmap.find(8)->second;

        // Decode the signals
        std::bitset<7> bitset;
        bitset.reset();

        auto range = mmap.equal_range(-1);
        // Find SIX
        for(auto i = range.first; i != range.second; i++) {
            
            Connection& conn = i->second;
            
            assert(conn.number == NUMBERS::NONE);

            // If segments count isn't 6 the logic would be faulty
            if(conn.segments.count() != 6) {
                continue;
            }
            bitset.reset();

            // ONE & SIX isolates the bottom-right segment
            bitset = one->segments & conn.segments;

            if(bitset.count() == 1) {                
                conn.number = NUMBERS::SIX;
                six = &conn;
                
                mmap.insert(std::pair<int, Connection>(conn.number, conn));
                mmap.erase(i);                              
                break;
            }
        }

        range = mmap.equal_range(-1);

        // Find THREE
        for(auto i = range.first; i != range.second; i++) {
            Connection& conn = i->second;

            assert(conn.number == NUMBERS::NONE);

            // If segments count isn't 5 the logic would be faulty
            if(conn.segments.count() != 5) {
                continue;
            }

            bitset.reset();
            bitset = seven->segments ^ conn.segments;   // bitset = N (middle/bottom)
            if(bitset.count() == 2) {
                conn.number = NUMBERS::THREE;
                three = &conn;
                mmap.insert(std::pair<int, Connection>(conn.number, conn));
                mmap.erase(i);
                break;
            }

        }
    

        // Find NINE and ZERO
        range = mmap.equal_range(-1);
        for(auto i = range.first; i != range.second;) {
            Connection& conn = i->second;

            if(conn.segments.count() != 6) {
                i++;
                continue;
            }

            bitset.reset();
            bitset = four->segments & conn.segments;

            if(bitset.count() == 4) {
                conn.number = NUMBERS::NINE;
                nine = &conn;
                mmap.insert(std::pair<int, Connection>(conn.number, conn));
                i = mmap.erase(i);
                range = mmap.equal_range(-1);   // Update the range
                continue;
            }
            else if(bitset.count() == 3) {
                conn.number = NUMBERS::ZERO;
                zero = &conn;
                mmap.insert(std::pair<int, Connection>(conn.number, conn));
                i = mmap.erase(i);
                range = mmap.equal_range(-1);
                continue;;
            }
            i++;
        }

        // Find TWO and FIVE
        range = mmap.equal_range(-1);
        for(auto i = range.first; i != range.second;) {
            Connection& conn = i->second;

            assert(conn.number == NUMBERS::NONE);

            bitset.reset();
            bitset = four->segments & conn.segments;

            if(bitset.count() == 2) {
                conn.number = NUMBERS::TWO;
                two = &conn;
                mmap.insert(std::pair<int, Connection>(conn.number, conn));
                i = mmap.erase(i);
                range = mmap.equal_range(-1);   // Update the range
                continue;
            }
            else {
                conn.number = NUMBERS::FIVE;
                five = &conn;
                mmap.insert(std::pair<int, Connection>(conn.number, conn));
                i = mmap.erase(i);
                range = mmap.equal_range(-1);
                continue;
            }
            i++;
        }

        int numberSum = 0;
        int factorCoefficient = 3;
        for(int i = 0; i < display[line].size(); i++) {
            Connection& conn = display[line][i];
            const int factor = pow(10, factorCoefficient);
            if(conn.number == NONE) {
                if(conn.segments == connections[line].find(1)->second.segments) {
                    numberSum += 1 * factor;
                }
                else if(conn.segments == connections[line].find(2)->second.segments) {
                    numberSum += 2 * factor;
                }
                else if(conn.segments == connections[line].find(3)->second.segments) {
                    numberSum += 3 * factor;
                }
                else if(conn.segments == connections[line].find(4)->second.segments) {
                    numberSum += 4 * factor;
                }
                else if(conn.segments == connections[line].find(5)->second.segments) {
                    numberSum += 5 * factor;
                }
                else if(conn.segments == connections[line].find(6)->second.segments) {
                    numberSum += 6 * factor;
                }
                else if(conn.segments == connections[line].find(7)->second.segments) {
                    numberSum += 7 * factor;
                }
                else if(conn.segments == connections[line].find(8)->second.segments) {
                    numberSum += 8 * factor;
                }
                else if(conn.segments == connections[line].find(9)->second.segments) {
                    numberSum += 9 * factor;
                }
            }
            else if(conn.number == NUMBERS::ONE) {
                numberSum += 1 * factor;
            }
            else if(conn.number == NUMBERS::FOUR) {
                numberSum += 4 * factor;
            }
            else if(conn.number == NUMBERS::SEVEN) {
                numberSum += 7 * factor;
            }
            else if(conn.number == NUMBERS::EIGHT) {
                numberSum += 8 * factor;
            }
            factorCoefficient--;
        }
        totalValue += numberSum;
    }
    std::cout << totalValue << std::endl;
}
#endif

void ReadInput(std::string path) {
    std::ifstream file(path.c_str());
    
    std::string line;
#ifdef PART_ONE
    while(!file.eof()){
        std::getline(file, line);
        std::string str;
        int delimPos = line.find_first_of('|');
        assert(delimPos != std::string::npos);
        int start = 1;
        int end = 1;
        while(end != std::string::npos){
            start = line.find_first_not_of(' ', delimPos + 1);
            if(start == std::string::npos)
                break;
            end = line.find_first_of(' ', start + 1);
            str = line.substr(start);
            int size = end - start;
            if(end == std::string::npos)
                size = str.size();
            delimPos = end;
            switch(size) {
                case UNIQUE_ONE:
                case UNIQUE_FOUR:
                case UNIQUE_SEVEN:
                case UNIQUE_EIGHT:
                ++uniqueCounter;
                break;
            }
        }

    }
    std::cout << uniqueCounter << std::endl;
    file.close();
#endif

#ifdef PART_TWO
    int lineCounter = 0;
    while(!file.eof()){
        std::getline(file, line);
        std::string str;
        connections.push_back(std::multimap<int, Connection>());
        display.push_back(std::array<Connection, 4>());
        int start = 0;
        int end = -1;
        const int delim = line.find_first_of('|');
        int numberCounter = 0;
        bool passedDelim = false;

        // Signals
        while(true){

            end = line.find_first_of(' ', start);
            if(end > delim && !passedDelim) {
                start = end + 1;
                passedDelim = true;
                continue;
            }
                
            str = line.substr(start, end - start);
            char c = 0;
            Connection conn;
            conn.segments.reset();

            switch(str.size()) {
                case UNIQUE_SEGMENTS::UNIQUE_ONE:
                    conn.number = NUMBERS::ONE;
                    break;
                case UNIQUE_SEGMENTS::UNIQUE_SEVEN:
                    conn.number = NUMBERS::SEVEN;
                    break;
                case UNIQUE_SEGMENTS::UNIQUE_FOUR:
                    conn.number = NUMBERS::FOUR;
                    break;
                case UNIQUE_SEGMENTS::UNIQUE_EIGHT:
                    conn.number = NUMBERS::EIGHT;
                    break;
            }
            constexpr int asciiOffset = 97;
            for(int i = 0; i < str.size(); i++) {
                c = str[i];
                int index = c - asciiOffset;
                assert(index >= 0);
                conn.segments.set(index, true);
            }

            if(end > delim || end == -1) {            
                display[lineCounter][numberCounter] = conn;
                numberCounter++;
            }
            else {
                connections[lineCounter].insert(std::pair<int, Connection>(static_cast<int>(conn.number), conn));
            }
            
            if(end == -1)
                break;
            start = end + 1;
        }
            lineCounter++;
    }
    file.close();
#endif
}
