#include <Windows.h>
#include <iostream>

struct DotBoard {
	bool IsRec1 = false;
	bool IsRec2 = false;
	int x, y;
};

DotBoard Board[30][30];

void FirstValueInput(int& x1, int& x2, int& y1, int& y2, int& x3, int& x4, int& y3, int& y4);
void ShowBoard();
void RightMove(int& x1, int& x2, int& y1, int& y2);
void LeftMove(int& x1, int& x2, int& y1, int& y2);
void DownMove(int& x1, int& x2, int& y1, int& y2);
void UpMove(int& x1, int& x2, int& y1, int& y2);
void XminusYminus(int& x1, int& x2, int& y1, int& y2);
void XplusYplus(int& x1, int& x2, int& y1, int& y2);
void RightMove2(int& x3, int& x4, int& y3, int& y4);
void LeftMove2(int& x3, int& x4, int& y3, int& y4);
void DownMove2(int& x3, int& x4, int& y3, int& y4);
void UpMove2(int& x3, int& x4, int& y3, int& y4);
void XminusYminus2(int& x3, int& x4, int& y3, int& y4);
void XplusYplus2(int& x3, int& x4, int& y3, int& y4);
void AllReset(bool& IsNotRes);

int main() {
	int x1, x2, y1, y2;
	int x3, x4, y3, y4;
	bool IsNotRes = true;

	while (true) {
		FirstValueInput(x1, x2, y1, y2, x3, x4, y3, y4);
		IsNotRes = true;
		while (IsNotRes) {
			ShowBoard();
			char Command = {};
			std::cout << "명령어를 입력하세요 : ";
			std::cin >> Command;

			switch (Command) {
			case 'x': {
				RightMove(x1, x2, y1, y2);
				break;
			}
			case 'X': {
				LeftMove(x1, x2, y1, y2);
				break;
			}
			case 'y': {
				DownMove(x1, x2, y1, y2);
				break;
			}
			case 'Y': {
				UpMove(x1, x2, y1, y2);
				break;
			}
			case 's': {
				XminusYminus(x1, x2, y1, y2);
				break;
			}
			case 'S': {
				XplusYplus(x1, x2, y1, y2);
				break;
			}
			case 'w': {
				RightMove2(x3, x4, y3, y4);
				break;
			}
			case 'W': {
				LeftMove2(x3, x4, y3, y4);
				break;
			}
			case 'a': {
				DownMove2(x3, x4, y3, y4);
				break;
			}
			case 'A': {
				UpMove2(x3, x4, y3, y4);
				break;
			}
			case 'd': {
				XminusYminus2(x3, x4, y3, y4);
				break;
			}
			case 'D': {
				XplusYplus2(x3, x4, y3, y4);
				break;
			}
			case 'r': {
				AllReset(IsNotRes);
				break;
			}
			case 'q': {
				std::cout << "프로그램을 종료합니다." << std::endl;
				exit(0);
				break;
			}
			default: {
				std::cout << "잘못된 입력입니다.";
				break;
			}
			}
		}
	}
}

void FirstValueInput(int& x1, int& x2, int& y1, int& y2, int& x3, int& x4, int& y3, int& y4) {
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
	std::cout << "X 3 : ";
	std::cin >> x3;
	std::cout << "Y 3 : ";
	std::cin >> y3;
	std::cout << "X 4 : ";
	std::cin >> x4;
	std::cout << "Y 4 : ";
	std::cin >> y4;

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec1 = true;
		}
	}
	for (int i = x3; i <= x4; ++i) {
		for (int j = y3; j <= y4; ++j) {
			Board[j][i].IsRec2 = true;
		}
	}
}

void ShowBoard() {
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			if (Board[i][j].IsRec1 && Board[i][j].IsRec2) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				std::cout << "# ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else if (Board[i][j].IsRec1 && !Board[i][j].IsRec2) {
				std::cout << "0 ";
			}
			else if (!Board[i][j].IsRec1 && Board[i][j].IsRec2) {
				std::cout << "X ";
			}
			else
				std::cout << ". ";
		}
		std::cout << std::endl;
	}
}

void RightMove(int& x1, int& x2, int& y1, int& y2) {
	if (x2 == 29) {
		x1 += 1;
		x2 = 0;
	}
	else if (x1 == 29) {
		x1 = 0;
		x2 += 1;
	}
	else if (x2 < 29 || x2 < x1) {
		x1 += 1;
		x2 += 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
		}
	}

	if (x1 < x2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
	else {
		for (int i = 0; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}

		for (int i = x1; i < 30; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
}

void LeftMove(int& x1, int& x2, int& y1, int& y2) {
	if (x2 == 0) {
		x1 -= 1;
		x2 = 29;
	}
	else if (x1 == 0) {
		x1 = 29;
		x2 -= 1;
	}
	else if (x2 <= 29 || x2 < x1) {
		x1 -= 1;
		x2 -= 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
		}
	}

	if (x1 < x2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
	else {
		for (int i = 0; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}

		for (int i = x1; i < 30; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
}

void DownMove(int& x1, int& x2, int& y1, int& y2) {
	if (y2 == 29) {
		y1 += 1;
		y2 = 0;
	}
	else if (y1 == 29) {
		y1 = 0;
		y2 += 1;
	}
	else if (y2 < 29 || y2 < y1) {
		y1 += 1;
		y2 += 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
		}
	}

	if (y1 < y2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
	else {
		for (int i = x1; i <= x2; ++i) {
			for (int j = 0; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}

		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j < 30; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
}

void UpMove(int& x1, int& x2, int& y1, int& y2) {
	if (y2 == 0) {
		y1 -= 1;
		y2 = 29;
	}
	else if (y1 == 0) {
		y1 = 29;
		y2 -= 1;
	}
	else if (y2 <= 29 || y2 < y1) {
		y1 -= 1;
		y2 -= 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
		}
	}

	if (y1 < y2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
	else {
		for (int i = x1; i <= x2; ++i) {
			for (int j = 0; j <= y2; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}

		for (int i = x1; i < x2; ++i) {
			for (int j = y1; j <= 30; ++j) {
				Board[j][i].IsRec1 = true;
			}
		}
	}
}

void XminusYminus(int& x1, int& x2, int& y1, int& y2) {
	if (x1 > 0)
		x1 -= 1;
	if (x2 < 29)
		x2 += 1;
	if (y1 > 0)
		y1 -= 1;
	if (y2 < 29)
		y2 += 1;

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec1 = true;
		}
	}
}

void XplusYplus(int& x1, int& x2, int& y1, int& y2) {
	if (x1 + 1 < x2) {
		x1 += 1;
		x2 -= 1;
	}
	if (y1 + 1 < y2) {
		y1 += 1;
		y2 -= 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec1 = true;
		}
	}
}

void RightMove2(int& x3, int& x4, int& y3, int& y4) {
	if (x4 == 29) {
		x3 += 1;
		x4 = 0;
	}
	else if (x3 == 29) {
		x3 = 0;
		x4 += 1;
	}
	else if (x4 < 29 || x4 < x3) {
		x3 += 1;
		x4 += 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec2 = false;
		}
	}

	if (x3 < x4) {
		for (int i = x3; i <= x4; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
	else {
		for (int i = 0; i <= x4; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}

		for (int i = x3; i < 30; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
}

void LeftMove2(int& x3, int& x4, int& y3, int& y4) {
	if (x4 == 0) {
		x3 -= 1;
		x4 = 29;
	}
	else if (x3 == 0) {
		x3 = 29;
		x4 -= 1;
	}
	else if (x4 <= 29 || x4 < x3) {
		x3 -= 1;
		x4 -= 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec2 = false;
		}
	}

	if (x3 < x4) {
		for (int i = x3; i <= x4; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
	else {
		for (int i = 0; i <= x4; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}

		for (int i = x3; i < 30; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
}

void DownMove2(int& x3, int& x4, int& y3, int& y4) {
	if (y4 == 29) {
		y3 += 1;
		y4 = 0;
	}
	else if (y3 == 29) {
		y3 = 0;
		y4 += 1;
	}
	else if (y4 < 29 || y4 < y3) {
		y3 += 1;
		y4 += 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec2 = false;
		}
	}

	if (y3 < y4) {
		for (int i = x3; i <= x4; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
	else {
		for (int i = x3; i <= x4; ++i) {
			for (int j = 0; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}

		for (int i = x3; i <= x4; ++i) {
			for (int j = y3; j < 30; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
}

void UpMove2(int& x3, int& x4, int& y3, int& y4) {
	if (y4 == 0) {
		y3 -= 1;
		y4 = 29;
	}
	else if (y3 == 0) {
		y3 = 29;
		y4 -= 1;
	}
	else if (y4 <= 29 || y4 < y3) {
		y3 -= 1;
		y4 -= 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec2 = false;
		}
	}

	if (y3 < y4) {
		for (int i = x3; i <= x4; ++i) {
			for (int j = y3; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
	else {
		for (int i = x3; i <= x4; ++i) {
			for (int j = 0; j <= y4; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}

		for (int i = x3; i < x4; ++i) {
			for (int j = y3; j <= 30; ++j) {
				Board[j][i].IsRec2 = true;
			}
		}
	}
}

void XminusYminus2(int& x3, int& x4, int& y3, int& y4) {
	if (x3 > 0)
		x3 -= 1;
	if (x4 < 29)
		x4 += 1;
	if (y3 > 0)
		y3 -= 1;
	if (y4 < 29)
		y4 += 1;

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec2 = false;
		}
	}

	for (int i = x3; i <= x4; ++i) {
		for (int j = y3; j <= y4; ++j) {
			Board[j][i].IsRec2 = true;
		}
	}
}

void XplusYplus2(int& x3, int& x4, int& y3, int& y4) {
	if (x3 + 1 < x4) {
		x3 += 1;
		x4 -= 1;
	}
	if (y3 + 1 < y4) {
		y3 += 1;
		y4 -= 1;
	}

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec2 = false;
		}
	}

	for (int i = x3; i <= x4; ++i) {
		for (int j = y3; j <= y4; ++j) {
			Board[j][i].IsRec2 = true;
		}
	}
}

void AllReset(bool& IsNotRes) {
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec1 = false;
			Board[i][j].IsRec2 = false;
		}
	}
	IsNotRes = false;
}