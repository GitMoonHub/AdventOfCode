#include "fstream"
#include "string"
#include "iostream"

int main() {
    
    std::string PATH = "/home/freetard/Programming/VSC/AdventOfCode2021/AoC2021_2_input.txt";
    std::ifstream file(PATH.c_str(), std::ifstream::in);
    std::string str;
    int forward = 0;
    int depth = 0;
    int aim = 0;
    while(!file.eof()){
        std::getline(file, str);
        std::size_t delim = str.find_first_of(" ");
        std::string command = str.substr(0, delim);
        int units = std::stoi(str.substr(delim, std::string::npos));

        // PART 1
        // if(command.compare("forward") == 0){
        //     forward += units;
        // }else if(command.compare("down") == 0){
        //     depth += units;
        // }else if(command.compare("up") == 0){
        //     depth -= units;
        // }

        // PART 2
        if(command.compare("forward") == 0){
            forward += units;
            depth += units * aim;
        }else if(command.compare("down") == 0){
            aim += units;
        }else if(command.compare("up") == 0){
            aim -= units;
        }

    }
    file.close();
    std::cout << forward * depth << std::endl;
}