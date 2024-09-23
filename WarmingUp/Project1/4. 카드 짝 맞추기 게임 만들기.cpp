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
		std::cout << "���� �õ� Ƚ�� : " << TryNum << std::endl;
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
	std::cout << "������ ��ǥ�� �Է��ϼ���." << std::endl;
	int a, b;
	std::cout << "ù ��° : ";
	std::cin >> a;
	std::cout << "�� ��° : ";
	std::cin >> b;
	
	a, b �ν��� �� ĭ ����;

	Sleep(2000);

	if (Board[i][j].Alphabet == Board[i][j].Alphabet) {
		a, b IsOpen = true;
		Score Up;
	}
	else {
		a, b IsOpen = false;
	}
}