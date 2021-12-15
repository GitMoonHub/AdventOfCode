#include "vector"
#include "fstream"
#include "iostream"
#include "cassert"

u_int ReadInput(std::string str, std::vector<std::string>& v);

int main() {

    std::vector<std::string>data;
    u_int binary_width = ReadInput("/home/freetard/Programming/VSC/AdventOfCode2021/AoC2021_3_input.txt", data);

// PART 1
    long gamma = 0;
    long epsilon = 0;
    std::string strGamma;
    // Compute Gamma's binary string
    for(int col = 0; col < binary_width; col++){
        u_int one = 0;
        u_int zero = 0;
    
        for(u_int i = 0; i < data.size(); i++){
            char c = data[i][col];
            if(c == '1'){
                ++one;
            }else{
                ++zero;
            }
        }
        if(one > zero){
            strGamma.append("1");
        }else{
            strGamma.append("0");
        }
    }
    u_int8_t bitshift = binary_width - 1;
    // Compute Both values
    for(int i = 0; i < strGamma.length(); i++){
        char c = strGamma[i];
        if(c == '1'){
            gamma += 1 << (bitshift - i);
        }
        else{
            epsilon += 1 << (bitshift - i);
        }
    }

    std::cout << gamma * epsilon << std::endl;

// PART 2
    std::vector<std::string>OGRM = data;    // Oxygen Generator Main
    std::vector<std::string>OGR1;           // OGR Ones
    std::vector<std::string>OGR0;           // OGR Zeroes
    std::vector<std::string>CSRM = data;    // CO2 Scrubbing Main
    std::vector<std::string>CSR1;           // CSR Ones
    std::vector<std::string>CSR0;           // CSR Zeroes
    std::string OGR;                        // OGR result
    std::string CSR;                           // CSR result

    // OGR
    for(int col = 0; col < binary_width; col++){
        // Reset variables for new bit column
        int one = 0;
        int zero = 0;
        OGR0.clear();
        OGR1.clear();
        for(int i = 0; i < OGRM.size(); i++){
            char c = OGRM[i][col];
            
            if(c == '1') {
                // Store the current value in the vector for values containing a "one"
                OGR1.push_back(OGRM[i]);
                ++one;
            } else {
                // Store the current value in the vector for values containing a "zero"
                OGR0.push_back(OGRM[i]);
                ++zero;
            }
        }

        // OGR wants the most common bit and favors the "one" if both are equal
        if(one == zero || one > zero){
            OGRM = OGR1;
        }
        else {
            OGRM = OGR0;
        }
        // If the main containers have only one entry left, this is the rating.
        if(OGRM.size() == 1){
            OGR = OGRM[0];
        }
    }

    // CSR
    for(int col = 0; col < binary_width; col++) {
        // Reset variables for new bit column
        int one = 0;
        int zero = 0;
        CSR0.clear();
        CSR1.clear();
        for(int i = 0; i < CSRM.size(); i++){
            char c = CSRM[i][col];
            if(c == '1'){
                // Store the current value in the vector for values containing a "one"
                CSR1.push_back(CSRM[i]);
                ++one;
            }
            else {
                // Store the current value in the vector for values containing a "zero"
                CSR0.push_back(CSRM[i]);
                ++zero;
            }
        }
        // CSR wants the least common bit and favors the "zero" if both are equal
        if(one == zero || one > zero){
            CSRM = CSR0;
        }
        else {
            CSRM = CSR1;
        }
        // If the main containers have only one entry left, this is the rating.
        if(CSRM.size() == 1){
            CSR = CSRM[0];
        }
    }

    // Compute the ratings
    long CS = 0;
    long OG = 0;
    u_int8_t bitshift = binary_width - 1;
    for(int i = 0; i < CSR.length(); i++){
        char ccs = CSR[i];
        char cog = OGR[i];
        if(ccs == '1'){
            CS += 1 << (bitshift - i);
        }
        if(cog == '1'){
            OG += 1 << (bitshift - i);
        }
    }

    std::cout << CS * OG << std::endl;
}


u_int ReadInput(std::string str, std::vector<std::string>& v) {
    std::ifstream file(str.c_str(), std::ifstream::in);
    std::string line;
    while(!file.eof()){
        std::getline(file, line);
        v.push_back(line);
    }
    return line.size();
}