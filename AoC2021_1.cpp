#include "iostream"
#include "vector"
#include "fstream"
#include "string"

std::vector<int> ReadInput(const char* path);

int main(){
    std::vector<int> input_data;
    int count = 0;
    // Get input data and convert from chars to ints
    input_data = ReadInput("/home/freetard/Programming/VSC/AdventOfCode2021/AoC2021_1_input.txt");

    // PART 1
    // Count the number of times current data has increased compared to the previous entry
    // for(int i = 1; i < input_data.size(); i++){
    //     if(input_data[i-1] < input_data[i]){
    //         ++count;
    //     }
    //}

    // PART 2
    // Three-measurement sliding window
    for(int i = 1; i < input_data.size(); i++){
        int A_count = 0;
        int B_count = 0;
        if(i + 2 >= input_data.size()){
            break;
        }
        for(int m = 0; m < 3; m++){
            A_count += input_data[i -1 + m];
            B_count += input_data[i + m];
        }
        if(A_count < B_count){
            ++count;
        }
    }

    std::cout << count << std::endl;
}


std::vector<int> ReadInput(const char* path){
    std::ifstream file(path, std::ifstream::in);
    std::vector<int> input_data;
    std::string str;
    while(!file.eof()){
        std::getline(file, str);
        input_data.push_back(std::stoi(str));
    }
    file.close();

    return input_data;
}