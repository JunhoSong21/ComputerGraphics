#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

int TextFileReading();
void CfindWord();
void GchangeLetter(bool &Gbool);

std::ifstream file("data.txt");
std::vector<std::string> lines;
std::string line;

int main() {
	TextFileReading();

	bool Gbool = true;

	while (true) {
		char Option{};
		std::cout << "명령어를 입력하세요 : ";
		std::cin >> Option;

		switch (Option) {
			case 'c':
			{

			}
			case 'g':
			{
				GchangeLetter(Gbool);
			}
			case 'q' :
			{
				std::cout << "프로그램을 종료합니다." << std::endl;
				exit(0);
			}
		}
	}
}

int TextFileReading() {
	std::cout << "파일을 읽어오는중..." << std::endl;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	}
	else {
		std::cerr << "파일을 열 수 없습니다." << std::endl;
		return 1;
	}

	std::cout << "파일 읽어오기 완료" << std::endl;
	for (const std::string& line : lines) {
		std::cout << line << std::endl;
	}
}

void GchangeLetter(bool &Gbool) {
	if (Gbool == true) {
		Gbool = false;
		char NewLetter;
		char OldLetter;
		std::cout << "바꿀 문자 입력 : ";
		std::cin >> OldLetter;
		std::cout << "새롭게 넣을 문자 입력 : ";
		std::cin >> NewLetter;

		size_t pos = line.find(OldLetter);
		while (pos != std::string::npos) {
			line[pos] = NewLetter;
			pos = line.find(OldLetter, pos + 1);
		}
	}
	else if (Gbool == false) {
		Gbool = true;
		std::cout << "원래대로 출력" << std::endl;
	}
}