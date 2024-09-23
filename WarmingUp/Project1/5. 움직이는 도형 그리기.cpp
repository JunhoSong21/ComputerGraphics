#include <Windows.h>
#include <iostream>

struct DotBoard {
	bool IsRec = false;
	int x, y;
};

DotBoard Board[30][30];

void FirstValueInput(int &x1, int&x2, int &y1, int &y2);
void ShowBoard();

void AllReset();

int main() {
	int x1, x2, y1, y2;
	while (true) {
		FirstValueInput(x1, x2, y1, y2);
		while (true) {
			system("cls");
			ShowBoard();
			char Command = {};
			std::cout << "��ɾ �Է��ϼ��� : ";
			std::cin >> Command;

			switch (Command) {
			case 'x': {
				break;
			}
			case 'r': {
				AllReset();
				break;
			}
			case 'q': {
				std::cout << "���α׷��� �����մϴ�." << std::endl;
				exit(0);
				break;
			}
			default: {
				std::cout << "�߸��� �Է��Դϴ�.";
				break;
			}
			}
		}
	}
}

void FirstValueInput(int &x1, int &x2, int &y1, int &y2) {
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].x = i;
			Board[i][j].y = j;
		}
	}

	std::cout << "X 1 : ";
	std::cin >> x1;
	std::cout << "Y 1 : ";
	std::cin >> y1;
	std::cout << "X 2 : ";
	std::cin >> x2;
	std::cout << "Y 2 : ";
	std::cin >> y2;

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[i][j].IsRec = true;
		}
	}
}

void ShowBoard() {
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			if (Board[i][j].IsRec)
				std::cout << "# ";
			else
				std::cout << ". ";
		}
		std::cout << std::endl;
	}
}

void AllReset() {
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec = false;
		}
	}
}