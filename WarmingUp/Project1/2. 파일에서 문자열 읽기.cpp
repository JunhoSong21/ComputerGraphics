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
		std::cout << "��ɾ �Է��ϼ��� : ";
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
				std::cout << "���α׷��� �����մϴ�." << std::endl;
				exit(0);
			}
		}
	}
}

int TextFileReading() {
	std::cout << "������ �о������..." << std::endl;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	}
	else {
		std::cerr << "������ �� �� �����ϴ�." << std::endl;
		return 1;
	}

	std::cout << "���� �о���� �Ϸ�" << std::endl;
	for (const std::string& line : lines) {
		std::cout << line << std::endl;
	}
}

void GchangeLetter(bool &Gbool) {
	if (Gbool == true) {
		Gbool = false;
		char NewLetter;
		char OldLetter;
		std::cout << "�ٲ� ���� �Է� : ";
		std::cin >> OldLetter;
		std::cout << "���Ӱ� ���� ���� �Է� : ";
		std::cin >> NewLetter;

		size_t pos = line.find(OldLetter);
		while (pos != std::string::npos) {
			line[pos] = NewLetter;
			pos = line.find(OldLetter, pos + 1);
		}
	}
	else if (Gbool == false) {
		Gbool = true;
		std::cout << "������� ���" << std::endl;
	}
}