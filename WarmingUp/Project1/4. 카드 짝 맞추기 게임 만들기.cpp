#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <windows.h>
#include <string>

const int GRID_SIZE = 5;
const int TOTAL_CELLS = GRID_SIZE * GRID_SIZE;
const int MAX_TURNS = 30;

struct Cell {
    char symbol;
    bool revealed;
    bool matched;
};

const int colors[12] = {
    12,
    14,
    10,
    11,
    13,
    9,
    15,
    8,
    7,
    6,
    5,
    3
};

void initializeGrid(Cell grid[TOTAL_CELLS]);
void printGrid(const Cell grid[TOTAL_CELLS]);
void printSolutionGrid(const Cell grid[TOTAL_CELLS]); // 정답판 출력 함수
bool getUserSelection(const Cell grid[TOTAL_CELLS], const std::string& input, int& idx1, int& idx2);
void revealCells(Cell grid[TOTAL_CELLS], int idx1, int idx2);
bool checkMatch(Cell grid[TOTAL_CELLS], int idx1, int idx2);
void resetGame(Cell grid[TOTAL_CELLS], int& score, int& turns);

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    Cell grid[TOTAL_CELLS];
    int score = 0;
    int turns = MAX_TURNS;

    initializeGrid(grid);

    while (true) {
        system("cls");
        std::cout << "남은 턴: " << turns << " | 점수: " << score << "\n";
        printGrid(grid);

        std::string command;
        std::cout << "r : 리셋  s : 정답판 공개" << std::endl;
        std::cout << "명령어나 격자 선택 : ";
        std::getline(std::cin, command);

        if (command == "s") {
            printSolutionGrid(grid);
            system("pause");
            continue;
        }
        if (command[0] == 'r') {
            resetGame(grid, score, turns);
            continue;
        }
        if (turns <= 0) {
            std::cout << "더 이상 턴이 없습니다. 게임을 리셋하거나 종료하세요.\n";
            continue;
        }

        int idx1, idx2;

        if (getUserSelection(grid, command, idx1, idx2)) {
            revealCells(grid, idx1, idx2);
            printGrid(grid);

            if (checkMatch(grid, idx1, idx2)) {
                std::cout << "매치 성공!\n";
                score += 10;
            }
            else {
                std::cout << "매치되지 않았습니다.\n";
                Sleep(3000);
                grid[idx1].revealed = false;
                grid[idx2].revealed = false;
            }
            --turns;
        }
        else {
            std::cout << "잘못된 선택입니다.\n";
        }

        bool allMatched = true;
        for (int i = 0; i < TOTAL_CELLS; ++i) {
            if (!grid[i].matched && grid[i].symbol != '*') {
                allMatched = false;
                break;
            }
        }
        if (allMatched) {
            system("cls");
            printGrid(grid);
            std::cout << "\n모든 매치를 완료했습니다! 게임 종료.\n";
            std::cout << "최종 점수: " << score << "\n";
            break;
        }
        if (turns <= 0) {
            system("cls");
            printGrid(grid);
            std::cout << "\n더 이상 턴이 없습니다. 게임 종료.\n";
            std::cout << "최종 점수: " << score << "\n";
            break;
        }
    }
    return 0;
}

void initializeGrid(Cell grid[TOTAL_CELLS]) {
    char letters[] = { 'A','B','C','D','E','F','G','H','I','J','K','L' };
    char symbols[TOTAL_CELLS];
    int index = {};

    for (auto i = 0; i < 12; ++i) {
        symbols[index++] = letters[i];
        symbols[index++] = letters[i];
    }
    symbols[index++] = '@';
    while (index < TOTAL_CELLS) {
        symbols[index++] = '*';
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(symbols, symbols + TOTAL_CELLS, g);

    for (auto i = 0; i < TOTAL_CELLS; ++i) {
        grid[i].symbol = symbols[i];
        grid[i].revealed = false;
        grid[i].matched = false;
    }
}

void printGrid(const Cell grid[TOTAL_CELLS]) {
    std::cout << " ";
    for (char c = 'a'; c < 'a' + GRID_SIZE; ++c) {
        std::cout << " " << c;
    }
    std::cout << "\n";

    for (int row = 0; row < GRID_SIZE; ++row) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < GRID_SIZE; ++col) {
            int idx = row * GRID_SIZE + col;
            if (grid[idx].revealed || grid[idx].matched) {
                if (grid[idx].matched) {
                    if (grid[idx].symbol != '@') {
                        int letterIndex = grid[idx].symbol - 'A';
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[letterIndex]); // 각 문자 색상
                    }
                    else {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    }
                }
                std::cout << grid[idx].symbol << " ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else {
                std::cout << "* ";
            }
        }
        std::cout << "\n";
    }
}

void printSolutionGrid(const Cell grid[TOTAL_CELLS]) {
    std::cout << "정답판:\n";
    std::cout << " ";
    for (char c = 'a'; c < 'a' + GRID_SIZE; ++c) {
        std::cout << " " << c;
    }
    std::cout << "\n";

    for (int row = 0; row < GRID_SIZE; ++row) {
        std::cout << row + 1 << " ";
        for (int col = 0; col < GRID_SIZE; ++col) {
            int idx = row * GRID_SIZE + col;
            std::cout << grid[idx].symbol << " "; // 항상 문자를 보여줌
        }
        std::cout << "\n";
    }
}

bool getUserSelection(const Cell grid[TOTAL_CELLS], const std::string& input, int& idx1, int& idx2) {
    std::string selections[2];
    size_t spacePos = input.find(' ');

    if (spacePos == std::string::npos) {
        return false;
    }

    selections[0] = input.substr(0, spacePos);
    selections[1] = input.substr(spacePos + 1);

    if (selections[0].length() != 2 || selections[1].length() != 2) {
        return false;
    }

    char col1 = std::tolower(selections[0][0]);
    char col2 = std::tolower(selections[1][0]);
    int row1 = selections[0][1] - '1';
    int row2 = selections[1][1] - '1';

    if (col1 < 'a' || col1 >= 'a' + GRID_SIZE || col2 < 'a' || col2 >= 'a' + GRID_SIZE ||
        row1 < 0 || row1 >= GRID_SIZE || row2 < 0 || row2 >= GRID_SIZE) {
        return false;
    }

    idx1 = row1 * GRID_SIZE + (col1 - 'a');
    idx2 = row2 * GRID_SIZE + (col2 - 'a');

    if (grid[idx1].matched || grid[idx2].matched || grid[idx1].revealed || grid[idx2].revealed) {
        std::cout << "이미 매칭된 또는 공개된 격자입니다.\n";
        return false;
    }

    return true;
}

void revealCells(Cell grid[TOTAL_CELLS], int idx1, int idx2) {
    grid[idx1].revealed = true;
    grid[idx2].revealed = true;
}

bool checkMatch(Cell grid[TOTAL_CELLS], int idx1, int idx2) {
    if (grid[idx1].symbol == grid[idx2].symbol ||
        grid[idx1].symbol == '@' || grid[idx2].symbol == '@') {

        if (grid[idx1].symbol != '@' && grid[idx2].symbol != '@') {
            grid[idx1].matched = true;
            grid[idx2].matched = true;
        }
        else if (grid[idx1].symbol == '@' || grid[idx2].symbol == '@') {
            grid[idx1].matched = true;
            grid[idx2].matched = true;

            for (int i = 0; i < TOTAL_CELLS; ++i) {
                if (grid[i].symbol == (grid[idx1].symbol != '@' ? grid[idx1].symbol : grid[idx2].symbol) && !grid[i].matched) {
                    grid[i].matched = true;
                }
            }
        }
        return true;
    }
    return false;
}

void resetGame(Cell grid[TOTAL_CELLS], int& score, int& turns) {
    initializeGrid(grid);
    score = 0;
    turns = MAX_TURNS;
}