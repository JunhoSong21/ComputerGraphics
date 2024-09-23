#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h> // getch() ����� ���� ���

const int SIZE = 30;
const char PATH = '0';      // ���
const char BACKGROUND = '1'; // ���
const char OBSTACLE = 'X';  // ��ֹ�
const char PLAYER = '@';     // �÷��̾� ǥ��
const int NUM_OBSTACLES = 30;

struct Position {
    int x, y;
};

class Grid {
private:
    char board[SIZE][SIZE];
    Position player;
    Position path[SIZE * SIZE]; // ��� ����
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
                board[i][j] = BACKGROUND; // ��� ĭ�� ������� �ʱ�ȭ
            }
        }
    }

    void generateObstacles() {
        int count = 0;
        while (count < NUM_OBSTACLES) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            if (board[x][y] == BACKGROUND) {
                board[x][y] = OBSTACLE; // ��濡 ��ֹ� �߰�
                count++;
            }
        }
    }

    void createPath() {
        int x = 0, y = 0;
        board[x][y] = PATH; // ������ ����
        pathLength = 1;
        path[0] = { x, y };

        std::vector<Position> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }; // ��, ��, ��, ��
        Position lastDirection = { 1, 0 }; // ���� ������ �Ʒ���
        int stepsInCurrentDirection = 0; // ���� �������� �̵��� ĭ �� �ʱ�ȭ

        while (x < SIZE - 1 || y < SIZE - 1) {
            bool moved = false;

            for (const auto& dir : directions) {
                // ���� ������ �ƴ϶�� �̵� ī��Ʈ�� �ʱ�ȭ
                if (dir.x != lastDirection.x || dir.y != lastDirection.y) {
                    stepsInCurrentDirection = 0;
                }

                // �ִ� 5ĭ���� �̵� ����
                while (stepsInCurrentDirection < 5) {
                    int newX = x + dir.x;
                    int newY = y + dir.y;

                    // ��� üũ
                    if (newX < 0 || newX >= SIZE || newY < 0 || newY >= SIZE) {
                        break; // ��踦 ������ �ߴ�
                    }

                    // ��ֹ� �������� üũ
                    if (board[newX][newY] == OBSTACLE) {
                        break; // ��ֹ� ������ �ߴ�
                    }

                    // �̵� ���� ���� üũ
                    if (board[newX][newY] != BACKGROUND) {
                        break; // ��ΰ� �ƴ� ���� �ߴ�
                    }

                    // �̵� �� ��� ����
                    x = newX;
                    y = newY;
                    board[x][y] = PATH; // ��� ����
                    path[pathLength++] = { x, y };
                    stepsInCurrentDirection++;
                    lastDirection = dir; // ���� ���� ������Ʈ

                    // 5ĭ �̵� �� ����
                    if (stepsInCurrentDirection >= 5) {
                        break;
                    }
                }

                // �̵��� �߻��� ���
                if (stepsInCurrentDirection > 0) {
                    moved = true;
                    break; // ���� �������� ��ȯ
                }
            }

            // �̵��� �� ������ ���� ����
            if (!moved) {
                break;
            }
        }

        // �÷��̾� �ʱ� ��ġ
        player = path[0];
    }

    void printGrid() {
        system("cls"); // ȭ�� �����
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (i == player.x && j == player.y) {
                    std::cout << PLAYER << " "; // �÷��̾� ��ġ
                }
                else {
                    std::cout << board[i][j] << " "; // ���, ��ֹ�, ���
                }
            }
            std::cout << std::endl;
        }
    }

    void movePlayer(char direction) {
        int newX = player.x;
        int newY = player.y;

        switch (direction) {
        case 'w': newX--; break; // ��
        case 's': newX++; break; // �Ʒ�
        case 'a': newY--; break; // ����
        case 'd': newY++; break; // ������
        }

        // ��� Ȯ�� �� �̵�
        if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE && board[newX][newY] == PATH) {
            player.x = newX;
            player.y = newY;
        }
    }

    void resetGame() {
        initializeBoard();
        generateObstacles();
        createPath();
        player = path[0]; // �÷��̾� �ʱ� ��ġ
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    Grid grid;

    while (true) {
        grid.printGrid();

        // ��ɾ� �Է�
        char command = _getch(); // Ű �Է� ���

        if (command == 'q') {
            break; // ����
        }
        else if (command == 'r') {
            grid.resetGame(); // ���� ����
        }
        else if (command == '\r') {
            grid.resetGame(); // ���ο� ��� ����
        }
        else if (command == 'w' || command == 'a' || command == 's' || command == 'd') {
            grid.movePlayer(command); // �÷��̾� �̵�
        }
    }

    return 0;
}
