#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cmath>

#define PART_ONE
// #define PART_TWO

#ifdef PART_TWO
void PreComputeFuelCost();
#endif
void FindPosition();
void ReadInput(std::string path);
void Heapsort(std::vector<int>& v);
void Heapify(std::vector<int>& v, int size, int index);

#ifdef PART_ONE
bool isEven = false;    // Amount of crabs
#endif

std::vector<int> crabs;

#ifdef PART_TWO
std::vector<int> fuelCost;
double floatingMean = 0.0;
#endif

int main() {
    ReadInput("AoC2021_7_input.txt");

#ifdef PART_ONE
    if(crabs.size() % 2) {
        isEven = true;
    }
#endif

    Heapsort(crabs);
    
#ifdef PART_TWO
    PreComputeFuelCost();
#endif

    FindPosition();

}

#ifdef PART_TWO
void PreComputeFuelCost() {
    size_t size = crabs.size();
    int range = crabs[size - 1] + crabs[0];
    fuelCost.resize(range);
    int n = 0;
    for(int i = 0; i < range; i++) {
        fuelCost[i] =  i + n;
        n = fuelCost[i];
    }
}
#endif

#ifdef PART_ONE
void FindPosition() {
    size_t index = crabs.size()/2;
    int median = crabs[index];
    u_int64_t fuel = 0;

    for(int i = 0; i < crabs.size(); i++) {
        fuel += abs(crabs[i] - median);

    }

    if(isEven) {
        u_int64_t evenFuel = 0;
        int evenMedian = crabs[index + 1];

        for(int i = 0; i < crabs.size(); i++) {

            evenFuel += abs(crabs[i] - evenMedian);

        }
        if(evenFuel < fuel) {
            fuel = evenFuel;
            median = evenMedian;
        }
    }
    std::cout << "Fuel: " << fuel << "\t";
    std::cout << "Position: " << median << "\n";

}
#endif

#ifdef PART_TWO
void FindPosition() {
    
    u_int64_t mean = llround(floatingMean);
    u_int64_t fuel = 0;
    for(int i = 0; i < crabs.size(); i++) {
        int dist = abs(crabs[i] - mean);
        fuel += fuelCost[dist];
    }
    
    // This covers a case where the system rounded up from a x.5..f value
    // and would give a wrong answer because of it.
    if(mean != static_cast<u_int64_t>(floatingMean)) {
        u_int64_t hwFuel = 0;
        u_int64_t hwMean = static_cast<u_int64_t>(floatingMean);
        for(int i = 0; i < crabs.size(); i++) {
            int dist = abs(crabs[i] - hwMean);
            hwFuel += fuelCost[dist];
        }

        if(hwFuel < fuel) {
            fuel = hwFuel;
            mean = hwMean;
        }
    }
    std::cout << "Fuel: " << fuel << "\t";
    std::cout << "Position: " << mean << "\n";  
    
}
#endif

void ReadInput(std::string path) {
    std::ifstream file(path.c_str());
    char c;

    if(!file.is_open()) {
        std::cout << "Coudln't read file.\n";
        exit(-1);
    }
    std::string str;
    int max = -1;
    int i = -1;
    while(file.get(c)) {
        i = -1;
        if(c == ',') {            
            i = std::stoi(str);
#ifdef PART_TWO
            floatingMean += i;
#endif
            crabs.push_back(i);
            str = std::string();
            continue;
        }
        str.append(1, c);
    }
    i = std::stoi(str);
    crabs.push_back(i);
#ifdef PART_TWO
    floatingMean += i;
    floatingMean /= crabs.size();
#endif
}

// https://www.geeksforgeeks.org/dsa/heap-sort/
void Heapsort(std::vector<int>& v)  {
    int size = v.size();

    for(int i = size / 2 - 1; i >= 0; i--) {
        Heapify(v, size, i);
    }

    for(int i = size - 1; i > 0; i--) {
        std::swap(v[0], v[i]);
        Heapify(v, i, 0);
    }

}

void Heapify(std::vector<int>& v, int size, int index) {
    int largest = index;

    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if(leftChild < size && v[leftChild] > v[largest]) {
        largest = leftChild;
    }

    if(rightChild < size && v[rightChild] > v[largest]) {
        largest = rightChild;
    }

    if(largest != index) {
        std::swap(v[index], v[largest]);
        Heapify(v, size, largest);
    }
}