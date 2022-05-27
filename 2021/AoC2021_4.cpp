#include "iostream"
#include "string"
#include "vector"
#include "regex"
#include "fstream"
#include "cassert"

struct Board {
    Board(std::vector<int>& v){
        grid = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
        // i for rows
        // n for columns
        for(int i = 0; i < size; i++){
            for(int n = 0; n < size; n++){
                grid[i][n] = v[i*size + n]; 
            }
        }

        marks = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
        row_score = std::vector<int>(5, 0);
        col_score = row_score;
    }
    Board(){

    }

    Board& operator=(Board b){
        // No idea what I'm doing here
        grid = b.grid;
        marks = b.marks;
        row_score = b.row_score;
        col_score = b.col_score;
        return *this;
    }

    const u_int8_t size = 5;
    bool won = false;

    std::vector<std::vector<int>> grid;
    std::vector<std::vector<bool>> marks;

    std::vector<int>row_score;
    std::vector<int>col_score;
};

void ReadInput(std::string str, std::vector<Board>& boards, std::vector<int>& numbers);
std::ostream& operator<<(std::ostream& os, const Board& b);


int main() {
    std::vector<Board> boards;
    std::vector<int> numbers;
    std::string PATH;
    // Get input data
    ReadInput(PATH, boards, numbers);

    // PART ONE
    int number = -1;
    int winning_board = -1;
    // Play out the bingo game
    for(int i = 0; i < numbers.size(); i++){
        // Draw number
        number = numbers[i];
        
        // Mark the boards
        for(int b = 0; b < boards.size(); b++){
            // Get a board
            Board& board = boards[b];
            // Iterate through the rows
            for(int row = 0; row < board.size; row++){
                // Iterate through the row's column
                for(int col = 0; col < board.size; col++){
                    if(board.grid[row][col] == number){
                        
                        board.marks[row][col] = true;
                        board.row_score[row] += 1;
                        board.col_score[col] += 1;
                        
                        if(board.row_score[row] == 5){
                            winning_board = b;
                            goto ComputeResult;  // Lol
                        }
                        if(board.col_score[col] == 5){
                            winning_board = b;
                            goto ComputeResult;
                        }
                    }
                }
            }
        }
    }
    
ComputeResult:
    // Compute the winning board's score
    Board& b = boards[winning_board];
    int score = 0;
    for(int row = 0; row < b.size; row++){
        for(int col = 0; col < b.size; col++){
            if(b.marks[row][col] == false){
                score += b.grid[row][col];
            }
        }
    }
    score *= number;
    std::cout << "Result: " << score << std::endl;
    exit(0);


    //################################################################################

    // PART TWO
    int number = -1;
    int winning_board = -1;
    int last_wb_number = -1;
    int how_many_wins = 0;
    
    Board last_wb = Board();

    // Play out the bingo game to find the last winning board
    for(int i = 0; i < numbers.size(); i++){
        // Draw number
        number = numbers[i];
        
        // Mark the boards
        for(int b = 0; b < boards.size(); b++){
            // Get a board
            Board& board = boards[b];
            // Iterate through the rows
            for(int row = 0; row < board.size; row++){
                // Iterate through the row's column
                for(int col = 0; col < board.size; col++){
                    if(board.grid[row][col] == number){
                        
                        board.marks[row][col] = true;
                        board.row_score[row] += 1;
                        board.col_score[col] += 1;
                        
                        if(board.row_score[row] == 5){
                            // If current winner hasn't already won, tag him as having won
                            if(!board.won){
                                how_many_wins++;
                                board.won = true;
                            }
                            winning_board = b;
                            last_wb_number = number;
                            last_wb = boards[b];
                        }
                        if(board.col_score[col] == 5){
                            if(!board.won){
                                how_many_wins++;
                                board.won = true;
                            }
                            winning_board = b;
                            last_wb_number = number;
                            last_wb = boards[b];
                            //goto LoopExit;
                        }
                        // If we have the same amount of winners as the amount of players,
                        // the current winner was the last winner of the game
                        if(how_many_wins == boards.size()){
                            goto ComputeResult;
                        }
                    }
                }
            }
        }
    }

    ComputeResult:
    // Compute the last winning board's score
    int score = 0;
    for(int row = 0; row < last_wb.size; row++){
        for(int col = 0; col < last_wb.size; col++){
            if(last_wb.marks[row][col] == false){
                score += last_wb.grid[row][col];
            }
        }
    }
    score *= last_wb_number;
    std::cout << "Result: " << score << std::endl;
}


void ReadInput(std::string str, std::vector<Board>& boards, std::vector<int>& numbers) {
    std::ifstream file(str.c_str());
    std::string line;
    std::smatch m;
    std::regex regex("([0-9]+)");
    std::getline(file, line);
    
    // Match the bingo numbers on the first line of the input file
    // and store them in the numbers vector
    std::string s = line;
    while(std::regex_search(s, m, regex)){
        numbers.push_back(std::stoi(m.str()));
        s = m.suffix();
    }

    while(!file.eof()){
        std::getline(file, line);
        // Check if line is only a newline/empty
        if(line.empty()){
            //std::cout << "Empty line found\n";
            continue;
        }
       // std::cout << "Line before finding a board: " << line << std::endl;
        // We hit something else than a newline
        // Grids are 5x5 so we get the next 4 lines of the current grid
        //std::cout << "First line: " << line << std::endl;
        std::string strBoard(line);
        strBoard.append(" ");
        for(int i = 0; i < 4; i++){
            std::getline(file, line);
            strBoard.append(line + " ");
        }

        // std::cout << "StringBoard:\n" << strBoard << std::endl;
        // exit(-1);
        //std::cout << "strBoard: " << strBoard << std::endl;
        // Store the grid values in a vector
        s = strBoard;
        std::vector<int> bv;
        while(std::regex_search(s, m, regex)){
            bv.push_back(std::stoi(m.str()));
            s = m.suffix();
        }
        // Push a new board into our board vector
        boards.push_back(Board(bv));
    }
    
}

std::ostream& operator<<(std::ostream& os, const Board& b) {
    os << "Board:\n";
    for(int r = 0; r < b.size; r++){
        os << "\n    ";
        for(int c = 0; c < b.size; c++){
            os.width(3);
            os.fill(' ');
            os << std::right << b.grid[r][c];
        }
    }

    os << "\n\n";
    
    for(int r = 0; r < b.size; r++){
        os << "\n    ";
        for(int c = 0; c < b.size; c++){
            os.width(3);
            os.fill(' ');
            char ch = (b.marks[r][c] ? 'T' : 'F');
            os << std::right << ch;
        }
    }

    os << "\n\n";

    return os;
}
