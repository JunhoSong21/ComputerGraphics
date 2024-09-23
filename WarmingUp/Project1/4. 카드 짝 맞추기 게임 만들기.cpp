#include <Windows.h>
#include <iostream>

struct Board {
	char Alphabet{};
	bool IsOpen = false;
};

void DrawingBoard(Board board[5][5]);
void InsertBoard();

int main() {
	Board board[5][5] = {};
	int TryNum = 30;
	int Score = {};

	while (1) {
		DrawingBoard(board);
		std::cout << "남은 시도 횟수 : " << TryNum << std::endl;
		InsertBoard();
	}
}

void DrawingBoard(Board board[5][5]) {
	std::cout << "  ";

	for (auto ch = 'a'; ch <= 'e'; ++ch) {
		std::cout << ch << " ";
	}

	std::cout << std::endl;

	for (auto i = 0; i < 5; ++i) {
		std::cout << i + 1 << " ";
		for (auto j = 0; j < 5; ++j) {
			if (board[i][j].IsOpen == false) {
				std::cout << "* ";
			}
			else {
				std::cout << board[i][j].Alphabet;
			}
		}

		std::cout << std::endl;
	}
}

void InsertBoard() {
	std::cout << "선택할 좌표를 입력하세요." << std::endl;
	int a, b;
	std::cout << "첫 번째 : ";
	std::cin >> a;
	std::cout << "두 번째 : ";
	std::cin >> b;
	
	a, b 인식한 후 칸 오픈;

	Sleep(2000);

	if (Board[i][j].Alphabet == Board[i][j].Alphabet) {
		a, b IsOpen = true;
		Score Up;
	}
	else {
		a, b IsOpen = false;
	}
}