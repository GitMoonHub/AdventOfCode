#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <stack>

#define PART_ONE
#define PART_TWO

struct Cave {
    Cave(){

    }
    Cave(std::string s) :id(s) {
        isBigCave(id);
    }
private:
    void isBigCave(const std::string& id) {
        char c = id[0];  
        isBig = 'A' <= c && c <= 'Z';
    }    
public:
    std::string id;
    bool isBig = false;
#if defined PART_ONE
    bool visited = false;
#elif defined PART_TWO
    int visited = 0;
#endif
    std::list<std::string> linkedCaves;
};


struct Path {
    Path() {

    }
    bool twice = false;
    std::map<std::string, Cave> caves;
};


void ReadInput(std::map<std::string, Cave>& caves, std::string path);
void EvalCave(std::string& id);
#if defined PART_ONE
void PartOne();
#elif defined PART_TWO
void PartTwo();
#endif
Path path;
int paths = 0;

int main() {
    
    ReadInput(path.caves, "AoC2021_12_input.txt");

#if defined PART_ONE
    PartOne();    
#elif defined PART_TWO
    PartTwo();
#endif

    return 0;
    
}

void ReadInput(std::map<std::string, Cave>& caves, std::string path) {
    std::ifstream file(path);

    if(!file.is_open()) {
        std::cout << "Failed to open the stream.\n";
        exit(-1);
    }

    while(!file.eof()) {
        std::string line;
        std::getline(file, line);

        std::string source;
        std::string destination;

        int delim = line.find_first_of('-');
        source = line.substr(0, delim);
        destination = line.substr(delim+1);
        
        auto it = caves.find(source);
        if(it == caves.end()) {
            Cave cave = Cave(source);
            caves.insert(std::pair<std::string, Cave>(cave.id, cave));
        }
        
        it = caves.find(destination);
        if(it ==  caves.end()) {
            Cave cave = Cave(destination);
            caves.insert(std::pair<std::string, Cave>(cave.id, cave));
        }
        
        Cave& src = (*caves.find(source)).second;
        Cave& dest = (*caves.find(destination)).second;
        src.linkedCaves.push_back(dest.id);
        dest.linkedCaves.push_back(src.id);
    }
}

#ifdef PART_ONE
void PartOne() {

    Cave start = path.caves["start"];
    path.caves.erase("start");

    for(auto it = start.linkedCaves.begin(); it != start.linkedCaves.end(); it++) {
        EvalCave(*it);
    }
    std::cout << paths << "\n";
}
#endif

#ifdef PART_TWO
void PartTwo(){
    
    Cave start = path.caves["start"];
    path.caves.erase("start");

    for(auto it = start.linkedCaves.begin(); it != start.linkedCaves.end(); it++) {
        EvalCave(*it);
    }
    std::cout << paths << "\n";
}
#endif

#if defined PART_ONE
void EvalCave(std::string& id) {

    Cave* cave = &path.caves.at(id);

    if(cave->visited && !cave->isBig)
        return;

    if(cave->id == "end") {
        paths++;
        return;
    }

    cave = &path.caves.at(id);
    cave->visited = true;
    std::stack<std::string> actions;
    actions.push(id);

    for(auto it = cave->linkedCaves.begin(); it != cave->linkedCaves.end(); it++) {     
        if(*it == "start"){
            continue;
        }
        EvalCave(*it);
    }

    while(!actions.empty()) {
        path.caves.at(actions.top()).visited = false;
        actions.pop();
    }
}

#elif defined PART_TWO
void EvalCave(std::string& id) {

    Cave* cave = &path.caves.at(id);

    if(cave->visited >= 1 && !cave->isBig && path.twice)
        return;

    if(cave->id == "end") {
        paths++;
        return;
    }

    cave->visited++;
    std::stack<std::string> actions;
    actions.push(id);

    if(cave->visited == 2 && !cave->isBig) {
        path.twice = true;
        actions.push("twice");
    }

    for(auto it = cave->linkedCaves.begin(); it != cave->linkedCaves.end(); it++) {   
        if(*it == "start")
            continue;
        EvalCave(*it);
    }

    while(!actions.empty()) {
        if(actions.top() == "twice") {
            path.twice = false;
        }
        else {
            path.caves.at(actions.top()).visited--;
        }
        actions.pop();
    }
}
#endif