#include <Windows.h>
#include <iostream>

struct DotBoard {
	bool IsRec = false;
	int x, y;
};

DotBoard Board[30][30];

void FirstValueInput(int &x1, int&x2, int &y1, int &y2);
void ShowBoard();
void RightMove(int& x1, int& x2, int& y1, int& y2);
void LeftMove(int& x1, int& x2, int& y1, int& y2);
void DownMove(int& x1, int& x2, int& y1, int& y2);
void UpMove(int& x1, int& x2, int& y1, int& y2);
void XminusYminus(int& x1, int& x2, int& y1, int& y2);
void XplusYplus(int& x1, int& x2, int& y1, int& y2);
void Xplus(int& x1, int& x2, int& y1, int& y2);
void Xminus(int& x1, int& x2, int& y1, int& y2);
void Yplus(int& x1, int& x2, int& y1, int& y2);
void Yminus(int& x1, int& x2, int& y1, int& y2);
void XplusYminus(int& x1, int& x2, int& y1, int& y2);
void XminusYplus(int& x1, int& x2, int& y1, int& y2);

void ExtentCalcul();
void ExtentRate();
void AllReset(bool &IsNotRes);

int main() {
	int x1, x2, y1, y2;
	bool IsNotRes = true;

	while (true) {
		FirstValueInput(x1, x2, y1, y2);
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
			case 'i': {
				Xplus(x1, x2, y1, y2);
				break;
			}
			case 'I': {
				Xminus(x1, x2, y1, y2);
				break;
			}
			case 'j': {
				Yplus(x1, x2, y1, y2);
				break;
			}
			case 'J': {
				Yminus(x1, x2, y1, y2);
				break;
			}
			case 'a': {
				XplusYminus(x1, x2, y1, y2);
				break;
			}
			case 'A': {
				XminusYplus(x1, x2, y1, y2);
				break;
			}
			case 'm': {
				ExtentCalcul();
				break;
			}
			case 'n': {
				ExtentRate();
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
			Board[i][j].IsRec = false;
		}
	}

	if (x1 < x2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}
	}
	else {
		for (int i = 0; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}

		for (int i = x1; i < 30; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
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
			Board[i][j].IsRec = false;
		}
	}

	if (x1 < x2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}
	}
	else {
		for (int i = 0; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}

		for (int i = x1; i < 30; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
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
			Board[i][j].IsRec = false;
		}
	}

	if (y1 < y2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}
	}
	else {
		for (int i = x1; i <= x2; ++i) {
			for (int j = 0; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}

		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j < 30; ++j) {
				Board[j][i].IsRec = true;
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
			Board[i][j].IsRec = false;
		}
	}

	if (y1 < y2) {
		for (int i = x1; i <= x2; ++i) {
			for (int j = y1; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}
	}
	else {
		for (int i = x1; i <= x2; ++i) {
			for (int j = 0; j <= y2; ++j) {
				Board[j][i].IsRec = true;
			}
		}

		for (int i = x1; i < x2; ++i) {
			for (int j = y1; j <= 30; ++j) {
				Board[j][i].IsRec = true;
			}
		}
	}
}

void Xplus(int& x1, int& x2, int& y1, int& y2) {
	x1 -= 1;
	x2 += 1;

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
		}
	}
}

void Xminus(int& x1, int& x2, int& y1, int& y2) {
	x1 += 1;
	x2 -= 1;

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
		}
	}
}

void Yplus(int& x1, int& x2, int& y1, int& y2) {
	y1 -= 1;
	y2 += 1;

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
		}
	}
}

void Yminus(int& x1, int& x2, int& y1, int& y2) {
	y1 += 1;
	y2 -= 1;

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
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
			Board[i][j].IsRec = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
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
			Board[i][j].IsRec = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
		}
	}
}

void XplusYminus(int& x1, int& x2, int& y1, int& y2) {
	x1 -= 1;
	x2 += 1;
	y1 += 1;
	y2 -= 1;
	
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
		}
	}
}

void XminusYplus(int& x1, int& x2, int& y1, int& y2) {
	x1 += 1;
	x2 -= 1;
	y1 -= 1;
	y2 += 1;

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec = false;
		}
	}

	for (int i = x1; i <= x2; ++i) {
		for (int j = y1; j <= y2; ++j) {
			Board[j][i].IsRec = true;
		}
	}
}

void ExtentCalcul() {
	int RecExtent = {};
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			if (Board[i][j].IsRec)
				RecExtent++;
		}
	}

	std::cout << "사각형의 면적 : " << RecExtent << std::endl;
}

void ExtentRate() {
	int RateRec = {};

	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			if (Board[i][j].IsRec)
				RateRec++;
		}
	}

	auto RecRate = (RateRec / 900.0) * 100.0;

	std::cout << "사각형 면적 비율 : " << RecRate << "%" << std::endl;
}

void AllReset(bool &IsNotRes) {
	for (auto i = 0; i < 30; ++i) {
		for (auto j = 0; j < 30; ++j) {
			Board[i][j].IsRec = false;
		}
	}
	IsNotRes = false;
}