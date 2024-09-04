#include <iostream>
#include <time.h>
#include <stdlib.h>

void LoadingMakingMatrixs();
void ShowMatrixs();
int SelectCommand();

int Amatrix[16];
int Bmatrix[16];

int main(void) {
	while (1) {
		LoadingMakingMatrixs();
		ShowMatrixs();

		while (1) {
			SelectCommand();
		}
	}
}

void LoadingMakingMatrixs() { // 행렬 생성하는 함수
	std::cout << "새로운 2개의 행렬 생성중..." << std::endl;
	
	for (int i = 0; i < 2; ++i) { // 두개의 행렬 랜덤으로 생성하는 코드
		int MatrixNum[16];
		int SelectTWO[2];

		for (int j = 0; j < 16; ++j) {
			MatrixNum[j] = rand() % 2;
		}

		for (int j = 0; j < 2; ++j) {
			SelectTWO[j] = rand() % 16;
		}

		while (1) {
			if (SelectTWO[0] == SelectTWO[1]) {
				SelectTWO[1] = rand() % 16;
			}
			else {
				break;
			}
		}

		for (int j = 0; j < 2; ++j) {
			MatrixNum[SelectTWO[j]] = 2;
		}

		if (i == 0) {
			for (int j = 0; j < 16; ++j) {
				Amatrix[j] = MatrixNum[j];
			}
		}
		else if (i == 1) {
			for (int j = 0; j < 16; ++j) {
				Bmatrix[j] = MatrixNum[j];
			}
		}
	}

	std::cout << "2개의 행렬 생성 완료" << std::endl;
}

void ShowMatrixs() {
	int i = 0;

	while (i < 13) {
		std::cout << "|" << Amatrix[i] << " " << Amatrix[i + 1] << " " << Amatrix[i + 2] << " " << Amatrix[i + 3]
			<< "||" << Bmatrix[i] << " " << Bmatrix[i + 1] << " " << Bmatrix[i + 2] << " " << Bmatrix[i + 3] << "|" << std::endl;
		i += 4;
	}

	std::cout << std::endl;
}

int SelectCommand() {
	std::cout << "명령어를 입력하세요 : ";
	char CommandOpt;
	std::cin >> CommandOpt;

	int NewMatrix[16];

	if (CommandOpt == 109) { // 행렬의 곱셈

	}
	else if (CommandOpt == 97) { // 행렬의 덧셈
		for (int j = 0; j < 16; ++j) {
			NewMatrix[j] = Amatrix[j] + Bmatrix[j];
		}
		std::cout << "행렬의 덧셈" << std::endl;
	}
	else if (CommandOpt == 100) { // 행렬의 뺄셈
		for (int j = 0; j < 16; ++j) {
			NewMatrix[j] = Amatrix[j] - Bmatrix[j];
		}
		std::cout << "행렬의 뺄셈" << std::endl;
	}
	else if (CommandOpt == 113) {
		
	}

	for (int i = 0; i < 13; i += 4) {
		std::cout << "|" << NewMatrix[i] << " " << NewMatrix[i + 1] << " " << NewMatrix[i + 2] << " " << NewMatrix[i + 3]
			<< "|" << std::endl;
	}
}