#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <set>

#define PART_ONE
// #define PART_TWO

enum SYMBOL {
    SYMBOL_NULL = -1,
    SQUARE,
    CURLY,
    PARENTHESIS,
    CHEVRON,

    LSQUARE = 91,
    RSQUARE = 93,

    LCURLY = 123,
    RCURLY = 125,

    LPARENTHESIS = 40,
    RPARENTHESIS = 41,

    LCHEVRON = 60,
    RCHEVRON = 62
};

#ifdef PART_ONE
enum SCORE {
    P = 3,
    S = 57,
    C = 1197,
    CH = 25137
};
#endif

#ifdef PART_TWO
enum SCORE {
    P = 1,
    S,
    C,
    CH
};
#endif

void ReadInput(std::vector<std::string>& input, std::string path);

#ifdef PART_ONE
void PartOne();
int score = 0;
#endif

#ifdef PART_TWO
void PartTwo();
std::set<uint64_t> scores;
#endif

std::vector<std::string> input;

int main() {
    ReadInput(input, "AoC2021_10_input.txt");
    
#ifdef PART_ONE
    PartOne();
#endif
#ifdef PART_TWO
    PartTwo();
#endif

    return 0;
}

void ReadInput(std::vector<std::string>& input, std::string path) {
    std::ifstream file(path.c_str());

    while(!file.eof()) {
        std::string line;
        std::getline(file, line);
        input.push_back(line);
    }
}

#ifdef PART_ONE
void PartOne() {
    for(int line = 0; line < input.size(); line++) {
        std::string& str = input[line];
        std::stack<SYMBOL> symbols;
        bool corrupted = false;
        for(int i = 0; i < str.size(); i++) {
            if(corrupted)
                break;

            SYMBOL s = static_cast<SYMBOL>(str[i]);
            SYMBOL top = SYMBOL_NULL;

            if(!symbols.empty()) {
                top = symbols.top();
            }

            switch(s) {
                case LCURLY:
                    symbols.push(SYMBOL::CURLY);
                    break;
                case LSQUARE:
                    symbols.push(SYMBOL::SQUARE);
                    break;
                case LPARENTHESIS:
                    symbols.push(SYMBOL::PARENTHESIS);
                    break;
                case LCHEVRON:
                    symbols.push(SYMBOL::CHEVRON);
                    break;

                case RSQUARE:
                    if(top != SYMBOL::SQUARE) {
                        score += SCORE::S;
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;

                case RCURLY:
                    if(top != SYMBOL::CURLY) {
                        score += SCORE::C;
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;

                case RPARENTHESIS:
                    if(top != SYMBOL::PARENTHESIS) {
                        score += SCORE::P;
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;

                case RCHEVRON:
                    if(top != SYMBOL::CHEVRON) {
                        score += SCORE::CH;
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;
            }
        }
    }

    std::cout << score << std::endl;
}
#endif

#ifdef PART_TWO
void PartTwo() {
     for(int line = 0; line < input.size(); line++) {
        std::string& str = input[line];
        std::stack<SYMBOL> symbols;
        bool corrupted = false;

        for(int i = 0; i < str.size(); i++) {
            if(corrupted)
                break;

            SYMBOL s = static_cast<SYMBOL>(str[i]);
            SYMBOL top = SYMBOL_NULL;

            if(!symbols.empty()) {
                top = symbols.top();
            }

            switch(s) {
                case LCURLY:
                    symbols.push(SYMBOL::CURLY);
                    break;
                case LSQUARE:
                    symbols.push(SYMBOL::SQUARE);
                    break;
                case LPARENTHESIS:
                    symbols.push(SYMBOL::PARENTHESIS);
                    break;
                case LCHEVRON:
                    symbols.push(SYMBOL::CHEVRON);
                    break;

                case RSQUARE:
                    if(top != SYMBOL::SQUARE) {
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;

                case RCURLY:
                    if(top != SYMBOL::CURLY) {
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;

                case RPARENTHESIS:
                    if(top != SYMBOL::PARENTHESIS) {
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;

                case RCHEVRON:
                    if(top != SYMBOL::CHEVRON) {
                        corrupted = true;
                        continue;
                    }
                    else {
                        symbols.pop();
                    }
                    break;
            }
        }
        if(!corrupted) {
            uint64_t score = 0;
            while(!symbols.empty()) {
                SYMBOL s = symbols.top();
                symbols.pop();
                switch(s) {
                    case PARENTHESIS: score = score * 5 + SCORE::P; break;
                    case SQUARE: score = score * 5 + SCORE::S; break;
                    case CURLY: score = score * 5 + SCORE::C; break;
                    case CHEVRON: score = score * 5 + SCORE::CH; break;
                }
            }
            scores.insert(score);   // Duplicate scores are discarded!
        }
    }

    unsigned int middle = scores.size() / 2;
    auto it = scores.begin();
    std::advance(it, middle);
    std::cout << *it << "\n";
}
#endif