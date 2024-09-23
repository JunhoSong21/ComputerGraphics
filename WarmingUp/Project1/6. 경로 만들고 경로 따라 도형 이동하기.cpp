#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h> // getch() 사용을 위한 헤더

const int SIZE = 30;
const char PATH = '0';      // 경로
const char BACKGROUND = '1'; // 배경
const char OBSTACLE = 'X';  // 장애물
const char PLAYER = '@';     // 플레이어 표시
const int NUM_OBSTACLES = 30;

struct Position {
    int x, y;
};

class Grid {
private:
    char board[SIZE][SIZE];
    Position player;
    Position path[SIZE * SIZE]; // 경로 저장
    int pathLength;

public:
    Grid() {
        initializeBoard();
        generateObstacles();
        createPath();
    }

    void initializeBoard() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                board[i][j] = BACKGROUND; // 모든 칸을 배경으로 초기화
            }
        }
    }

    void generateObstacles() {
        int count = 0;
        while (count < NUM_OBSTACLES) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            if (board[x][y] == BACKGROUND) {
                board[x][y] = OBSTACLE; // 배경에 장애물 추가
                count++;
            }
        }
    }

    void createPath() {
        int x = 0, y = 0;
        board[x][y] = PATH; // 시작점 설정
        pathLength = 1;
        path[0] = { x, y };

        std::vector<Position> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }; // 하, 우, 상, 좌
        Position lastDirection = { 1, 0 }; // 시작 방향은 아래쪽
        int stepsInCurrentDirection = 0; // 현재 방향으로 이동한 칸 수 초기화

        while (x < SIZE - 1 || y < SIZE - 1) {
            bool moved = false;

            for (const auto& dir : directions) {
                // 현재 방향이 아니라면 이동 카운트를 초기화
                if (dir.x != lastDirection.x || dir.y != lastDirection.y) {
                    stepsInCurrentDirection = 0;
                }

                // 최대 5칸까지 이동 가능
                while (stepsInCurrentDirection < 5) {
                    int newX = x + dir.x;
                    int newY = y + dir.y;

                    // 경계 체크
                    if (newX < 0 || newX >= SIZE || newY < 0 || newY >= SIZE) {
                        break; // 경계를 넘으면 중단
                    }

                    // 장애물 만났는지 체크
                    if (board[newX][newY] == OBSTACLE) {
                        break; // 장애물 만나면 중단
                    }

                    // 이동 가능 여부 체크
                    if (board[newX][newY] != BACKGROUND) {
                        break; // 경로가 아닌 곳은 중단
                    }

                    // 이동 및 경로 설정
                    x = newX;
                    y = newY;
                    board[x][y] = PATH; // 경로 설정
                    path[pathLength++] = { x, y };
                    stepsInCurrentDirection++;
                    lastDirection = dir; // 현재 방향 업데이트

                    // 5칸 이동 후 종료
                    if (stepsInCurrentDirection >= 5) {
                        break;
                    }
                }

                // 이동이 발생한 경우
                if (stepsInCurrentDirection > 0) {
                    moved = true;
                    break; // 다음 방향으로 전환
                }
            }

            // 이동할 수 없으면 루프 종료
            if (!moved) {
                break;
            }
        }

        // 플레이어 초기 위치
        player = path[0];
    }

    void printGrid() {
        system("cls"); // 화면 지우기
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (i == player.x && j == player.y) {
                    std::cout << PLAYER << " "; // 플레이어 위치
                }
                else {
                    std::cout << board[i][j] << " "; // 경로, 장애물, 배경
                }
            }
            std::cout << std::endl;
        }
    }

    void movePlayer(char direction) {
        int newX = player.x;
        int newY = player.y;

        switch (direction) {
        case 'w': newX--; break; // 위
        case 's': newX++; break; // 아래
        case 'a': newY--; break; // 왼쪽
        case 'd': newY++; break; // 오른쪽
        }

        // 경로 확인 후 이동
        if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE && board[newX][newY] == PATH) {
            player.x = newX;
            player.y = newY;
        }
    }

    void resetGame() {
        initializeBoard();
        generateObstacles();
        createPath();
        player = path[0]; // 플레이어 초기 위치
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    Grid grid;

    while (true) {
        grid.printGrid();

        // 명령어 입력
        char command = _getch(); // 키 입력 대기

        if (command == 'q') {
            break; // 종료
        }
        else if (command == 'r') {
            grid.resetGame(); // 게임 리셋
        }
        else if (command == '\r') {
            grid.resetGame(); // 새로운 경로 생성
        }
        else if (command == 'w' || command == 'a' || command == 's' || command == 'd') {
            grid.movePlayer(command); // 플레이어 이동
        }
    }

    return 0;
}
