#include <Windows.h>
#include <iostream>

struct Board {
	char Alphabet{};
	bool IsOpen = false;
};

void DrawingBoard(Board board[5][5]);

int main() {
	Board board[5][5] = {};
	DrawingBoard(board);
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