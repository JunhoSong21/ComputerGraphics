#include <iostream>
#include <time.h>
#include <stdlib.h>

void LoadingMakingMatrixs();
void ShowMatrixs();
int SelectCommand();

int Amatrix[16];
int Bmatrix[16];
int oen = 0;

int main() {
	LoadingMakingMatrixs();
	ShowMatrixs();

	while (true) {
		SelectCommand();
	}
}

void LoadingMakingMatrixs() { // 행렬 생성하는 함수
	std::cout << "새로운 2개의 행렬 생성중..." << std::endl;
	
	for (auto i = 0; i < 2; ++i) { // 두개의 행렬 랜덤으로 생성하는 코드
		int MatrixNum[16];
		int SelectTWO[2];

		for (auto j = 0; j < 16; ++j) {
			MatrixNum[j] = rand() % 2;
		}

		for (auto j = 0; j < 2; ++j) {
			SelectTWO[j] = rand() % 16;
		}

		while (true) {
			if (SelectTWO[0] == SelectTWO[1]) {
				SelectTWO[1] = rand() % 16;
			}
			else {
				break;
			}
		}

		for (auto j = 0; j < 2; ++j) {
			MatrixNum[SelectTWO[j]] = 2;
		}

		if (i == 0) {
			for (int j = 0; j < 16; ++j) {
				Amatrix[j] = MatrixNum[j];
			}
		}
		else if (i == 1) {
			for (auto j = 0; j < 16; ++j) {
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
		for (auto i = 0; i < 4; ++i) {
			NewMatrix[i] = (Amatrix[0] * Bmatrix[0 + i]) + (Amatrix[1] * Bmatrix[4 + i]) + (Amatrix[2] * Bmatrix[8 + i]) + (Amatrix[3] * Bmatrix[12 + i]);
		}
		for (auto i = 4; i < 8; ++i) {
			NewMatrix[i] = (Amatrix[4] * Bmatrix[0 + i - 4]) + (Amatrix[5] * Bmatrix[4 + i - 4]) + (Amatrix[6] * Bmatrix[8 + i - 4]) + (Amatrix[7] * Bmatrix[12 + i - 4]);
		}
		for (auto i = 8; i < 12; ++i) {
			NewMatrix[i] = (Amatrix[8] * Bmatrix[0 + i - 8]) + (Amatrix[9] * Bmatrix[4 + i - 8]) + (Amatrix[10] * Bmatrix[8 + i - 8]) + (Amatrix[11] * Bmatrix[12 + i - 8]);
		}
		for (auto i = 12; i < 16; ++i) {
			NewMatrix[i] = (Amatrix[12] * Bmatrix[0 + i - 12]) + (Amatrix[13] * Bmatrix[4 + i - 12]) + (Amatrix[14] * Bmatrix[8 + i - 12]) + (Amatrix[15] * Bmatrix[12 + i - 12]);
		}
		std::cout << "행렬의 곱셈" << std::endl;
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
	else if (CommandOpt == 114) { // 행렬식의 값
		int AmatrixDet =
			(Amatrix[0] * ((Amatrix[5] * (Amatrix[10] * Amatrix[15] - Amatrix[11] * Amatrix[14]))
				- (Amatrix[6] * (Amatrix[9] * Amatrix[15] - Amatrix[11] * Amatrix[13]))
				+ (Amatrix[7] * (Amatrix[9] * Amatrix[14] - Amatrix[10] * Amatrix[13]))))
			- (Amatrix[4] * (Amatrix[4] * (Amatrix[10] * Amatrix[15] - Amatrix[11] * Amatrix[13]))
				- (Amatrix[5] * (Amatrix[8] * Amatrix[15] - Amatrix[11] * Amatrix[12]))
				+ (Amatrix[7] * (Amatrix[8] * Amatrix[13] - Amatrix[9] * Amatrix[12])))
			+ (Amatrix[2] * (Amatrix[4] * (Amatrix[9] * Amatrix[15] - Amatrix[11] * Amatrix[13]))
				- (Amatrix[5] * (Amatrix[8] * Amatrix[15] - Amatrix[11] * Amatrix[12]))
				+ (Amatrix[7] * (Amatrix[8] * Amatrix[13] - Amatrix[9] * Amatrix[12])))
			- (Amatrix[3] * (Amatrix[4] * (Amatrix[9] * Amatrix[14] - Amatrix[10] * Amatrix[13]))
				- (Amatrix[5] * (Amatrix[8] * Amatrix[14] - Amatrix[10] * Amatrix[12]))
				+ (Amatrix[6] * (Amatrix[8] * Amatrix[13] - Amatrix[9] * Amatrix[12])));
		int BmatrixDet =
			(Bmatrix[0] * ((Bmatrix[5] * (Bmatrix[10] * Bmatrix[15] - Bmatrix[11] * Bmatrix[14]))
				- (Bmatrix[6] * (Bmatrix[9] * Bmatrix[15] - Bmatrix[11] * Bmatrix[13]))
				+ (Bmatrix[7] * (Bmatrix[9] * Bmatrix[14] - Bmatrix[10] * Bmatrix[13]))))
			- (Bmatrix[4] * (Bmatrix[4] * (Bmatrix[10] * Bmatrix[15] - Bmatrix[11] * Bmatrix[13]))
				- (Bmatrix[5] * (Bmatrix[8] * Bmatrix[15] - Bmatrix[11] * Bmatrix[12]))
				+ (Bmatrix[7] * (Bmatrix[8] * Bmatrix[13] - Bmatrix[9] * Bmatrix[12])))
			+ (Bmatrix[2] * (Bmatrix[4] * (Bmatrix[9] * Bmatrix[15] - Bmatrix[11] * Bmatrix[13]))
				- (Bmatrix[5] * (Bmatrix[8] * Bmatrix[15] - Bmatrix[11] * Bmatrix[12]))
				+ (Bmatrix[7] * (Bmatrix[8] * Bmatrix[13] - Bmatrix[9] * Bmatrix[12])))
			- (Bmatrix[3] * (Bmatrix[4] * (Bmatrix[9] * Bmatrix[14] - Bmatrix[10] * Bmatrix[13]))
				- (Bmatrix[5] * (Bmatrix[8] * Bmatrix[14] - Bmatrix[10] * Bmatrix[12]))
				+ (Bmatrix[6] * (Bmatrix[8] * Bmatrix[13] - Bmatrix[9] * Bmatrix[12])));

		std::cout << "행렬식의 값" << std::endl;
		std::cout << "첫 번째 행렬 : " << AmatrixDet << "\n" << "두 번째 행렬 : " << BmatrixDet << std::endl;
		return 0;
	}
	else if (CommandOpt == 116) { // 전치행렬과 그 행렬식의 값
		int AmatrixTrans[16], BmatrixTrans[16];
		for (auto i = 0; i < 16; i += 5) {
			AmatrixTrans[i] = Amatrix[i];
			BmatrixTrans[i] = Bmatrix[i];
		}
		AmatrixTrans[1] = Amatrix[4];
		AmatrixTrans[2] = Amatrix[8];
		AmatrixTrans[3] = Amatrix[12];
		AmatrixTrans[4] = Amatrix[1];
		AmatrixTrans[6] = Amatrix[9];
		AmatrixTrans[7] = Amatrix[13];
		AmatrixTrans[8] = Amatrix[2];
		AmatrixTrans[9] = Amatrix[6];
		AmatrixTrans[11] = Amatrix[14];
		AmatrixTrans[12] = Amatrix[3];
		AmatrixTrans[13] = Amatrix[7];
		AmatrixTrans[14] = Amatrix[11];

		BmatrixTrans[1] = Bmatrix[4];
		BmatrixTrans[2] = Bmatrix[8];
		BmatrixTrans[3] = Bmatrix[12];
		BmatrixTrans[4] = Bmatrix[1];
		BmatrixTrans[6] = Bmatrix[9];
		BmatrixTrans[7] = Bmatrix[13];
		BmatrixTrans[8] = Bmatrix[2];
		BmatrixTrans[9] = Bmatrix[6];
		BmatrixTrans[11] = Bmatrix[14];
		BmatrixTrans[12] = Bmatrix[3];
		BmatrixTrans[13] = Bmatrix[7];
		BmatrixTrans[14] = Bmatrix[11];

		std::cout << "전치행렬과 그 행렬식의 값" << std::endl;
		int i = 0;

		while (i < 13) {
			std::cout << "|" << AmatrixTrans[i] << " " << AmatrixTrans[i + 1] << " " << AmatrixTrans[i + 2] << " " << AmatrixTrans[i + 3]
				<< "||" << BmatrixTrans[i] << " " << BmatrixTrans[i + 1] << " " << BmatrixTrans[i + 2] << " " << BmatrixTrans[i + 3] << "|" << std::endl;
			i += 4;
		}

		int AmatrixDet =
			(AmatrixTrans[0] * ((AmatrixTrans[5] * (AmatrixTrans[10] * AmatrixTrans[15] - AmatrixTrans[11] * AmatrixTrans[14]))
				- (AmatrixTrans[6] * (AmatrixTrans[9] * AmatrixTrans[15] - AmatrixTrans[11] * AmatrixTrans[13]))
				+ (AmatrixTrans[7] * (AmatrixTrans[9] * AmatrixTrans[14] - AmatrixTrans[10] * AmatrixTrans[13]))))
			- (AmatrixTrans[4] * (AmatrixTrans[4] * (AmatrixTrans[10] * AmatrixTrans[15] - AmatrixTrans[11] * AmatrixTrans[13]))
				- (AmatrixTrans[5] * (AmatrixTrans[8] * AmatrixTrans[15] - AmatrixTrans[11] * AmatrixTrans[12]))
				+ (AmatrixTrans[7] * (AmatrixTrans[8] * AmatrixTrans[13] - AmatrixTrans[9] * AmatrixTrans[12])))
			+ (AmatrixTrans[2] * (AmatrixTrans[4] * (AmatrixTrans[9] * AmatrixTrans[15] - AmatrixTrans[11] * AmatrixTrans[13]))
				- (AmatrixTrans[5] * (AmatrixTrans[8] * AmatrixTrans[15] - AmatrixTrans[11] * AmatrixTrans[12]))
				+ (AmatrixTrans[7] * (AmatrixTrans[8] * AmatrixTrans[13] - AmatrixTrans[9] * AmatrixTrans[12])))
			- (AmatrixTrans[3] * (AmatrixTrans[4] * (AmatrixTrans[9] * AmatrixTrans[14] - AmatrixTrans[10] * AmatrixTrans[13]))
				- (AmatrixTrans[5] * (AmatrixTrans[8] * AmatrixTrans[14] - AmatrixTrans[10] * AmatrixTrans[12]))
				+ (AmatrixTrans[6] * (AmatrixTrans[8] * AmatrixTrans[13] - AmatrixTrans[9] * AmatrixTrans[12])));
		int BmatrixDet =
			(BmatrixTrans[0] * ((BmatrixTrans[5] * (BmatrixTrans[10] * BmatrixTrans[15] - BmatrixTrans[11] * BmatrixTrans[14]))
				- (BmatrixTrans[6] * (BmatrixTrans[9] * BmatrixTrans[15] - BmatrixTrans[11] * BmatrixTrans[13]))
				+ (BmatrixTrans[7] * (BmatrixTrans[9] * BmatrixTrans[14] - BmatrixTrans[10] * BmatrixTrans[13]))))
			- (BmatrixTrans[4] * (BmatrixTrans[4] * (BmatrixTrans[10] * BmatrixTrans[15] - BmatrixTrans[11] * BmatrixTrans[13]))
				- (BmatrixTrans[5] * (BmatrixTrans[8] * BmatrixTrans[15] - BmatrixTrans[11] * BmatrixTrans[12]))
				+ (BmatrixTrans[7] * (BmatrixTrans[8] * BmatrixTrans[13] - BmatrixTrans[9] * BmatrixTrans[12])))
			+ (BmatrixTrans[2] * (BmatrixTrans[4] * (BmatrixTrans[9] * BmatrixTrans[15] - BmatrixTrans[11] * BmatrixTrans[13]))
				- (BmatrixTrans[5] * (BmatrixTrans[8] * BmatrixTrans[15] - BmatrixTrans[11] * BmatrixTrans[12]))
				+ (BmatrixTrans[7] * (BmatrixTrans[8] * BmatrixTrans[13] - BmatrixTrans[9] * BmatrixTrans[12])))
			- (BmatrixTrans[3] * (BmatrixTrans[4] * (BmatrixTrans[9] * BmatrixTrans[14] - BmatrixTrans[10] * BmatrixTrans[13]))
				- (BmatrixTrans[5] * (BmatrixTrans[8] * BmatrixTrans[14] - BmatrixTrans[10] * BmatrixTrans[12]))
				+ (BmatrixTrans[6] * (BmatrixTrans[8] * BmatrixTrans[13] - BmatrixTrans[9] * BmatrixTrans[12])));

		std::cout << "행렬식의 값" << std::endl;
		std::cout << "첫 번째 행렬 : " << AmatrixDet << "\n" << "두 번째 행렬 : " << BmatrixDet << std::endl;

		return 0;
	}
	else if (CommandOpt == 101) { // 홀수, 짝수만 출력
		if (oen == 2) {
			std::cout << "전체 출력" << std::endl;
			ShowMatrixs();
			oen = 0;
		}
		else if (oen == 0) {
			char oenAmatrix[16], oenBmatrix[16];
			std::cout << "짝수만 출력" << std::endl;
			for (auto i = 0; i < 16; ++i) {
				if (Amatrix[i] == 2)
					oenAmatrix[i] = 50;
				else
					oenAmatrix[i] = ' ';
				if (Bmatrix[i] == 2)
					oenBmatrix[i] = 50;
				else
					oenBmatrix[i] = ' ';
			}

			int i = 0;

			while (i < 13) {
				std::cout << "|" << oenAmatrix[i] << " " << oenAmatrix[i + 1] << " " << oenAmatrix[i + 2] << " " << oenAmatrix[i + 3]
					<< "||" << oenBmatrix[i] << " " << oenBmatrix[i + 1] << " " << oenBmatrix[i + 2] << " " << oenBmatrix[i + 3] << "|" << std::endl;
				i += 4;
			}

			std::cout << std::endl;

			oen = 1;
		}
		else if (oen == 1) {
			char oenAmatrix[16], oenBmatrix[16];
			std::cout << " 홀수만 출력" << std::endl;
			for (auto i = 0; i < 16; ++i) {
				if (Amatrix[i] == 2)
					oenAmatrix[i] = ' ';
				else if (Amatrix[i] == 1)
					oenAmatrix[i] = 49;
				else
					oenAmatrix[i] = 48;
				if (Bmatrix[i] == 2)
					oenBmatrix[i] = ' ';
				else if (Bmatrix[i] == 1)
					oenBmatrix[i] = 49;
				else
					oenBmatrix[i] = 48;
			}

			int i = 0;

			while (i < 13) {
				std::cout << "|" << oenAmatrix[i] << " " << oenAmatrix[i + 1] << " " << oenAmatrix[i + 2] << " " << oenAmatrix[i + 3]
					<< "||" << oenBmatrix[i] << " " << oenBmatrix[i + 1] << " " << oenBmatrix[i + 2] << " " << oenBmatrix[i + 3] << "|" << std::endl;
				i += 4;
			}

			std::cout << std::endl;

			oen = 2;
		}
		return 0;
	}
	else if (CommandOpt == 115) {
		LoadingMakingMatrixs();
		ShowMatrixs();
		return 0;
	}
	else if (CommandOpt >= 49 && CommandOpt <= 57) { // 입력한 숫자를 행렬에 곱
		std::cout << "행렬에 " << CommandOpt << "을(를) 곱" << std::endl;

		int AmatrixNum[16], BmatrixNum[16];
		for (auto i = 0; i < 16; ++i) {
			AmatrixNum[i] = Amatrix[i] * (CommandOpt - 48);
			BmatrixNum[i] = Bmatrix[i] * (CommandOpt - 48);
		}
		int i = 0;

		while (i < 13) {
			std::cout << "|" << AmatrixNum[i] << " " << AmatrixNum[i + 1] << " " << AmatrixNum[i + 2] << " " << AmatrixNum[i + 3]
				<< "||" << BmatrixNum[i] << " " << BmatrixNum[i + 1] << " " << BmatrixNum[i + 2] << " " << BmatrixNum[i + 3] << "|" << std::endl;
			i += 4;
		}

		std::cout << std::endl;
		return 0;
	}
	else if (CommandOpt == 113) { // 프로그램 종료
		std::cout << "프로그램을 종료합니다." << std::endl;
		exit(0);
	}

	for (auto i = 0; i < 13; i += 4) {
		std::cout << "|" << NewMatrix[i] << " " << NewMatrix[i + 1] << " " << NewMatrix[i + 2] << " " << NewMatrix[i + 3]
			<< "|" << std::endl;
	}
}