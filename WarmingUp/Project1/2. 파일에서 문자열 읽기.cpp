#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>

void ShowAll();
void CFindWord(const std::string& line, int& ChangeWords);
void DReverseLines();
void EInsertAlpha();
void FReverseWords();
void GChangeChar(const std::string& line, const char oldChar, const char newChar);
void HCountWords();
void RUp();
void RDown();
void SFindWord(const std::string& searchWord, int& wordCount);

std::ifstream file("data.txt");
std::string line;

int main() {
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string word;
	}

	bool Cbool = true;
	bool Dbool = true;
	bool Ebool = true;
	bool Fbool = true;
	bool Gbool = true;
	int Rbool = 0;
	bool Sbool = true;

	ShowAll();

	while (true) {
		char Option{};
		std::cout << "명령어를 입력하세요 : ";
		std::cin >> Option;

		switch (Option) {
		case 'c': {
			if (Cbool == true) {
				file.clear();
				file.seekg(0);

				int ChangeWords = 0;

				while (std::getline(file, line)) {
					CFindWord(line, ChangeWords);
				}

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				std::cout << "색이 변경된 단어의 개수 : " << ChangeWords << std::endl;
				Cbool = false;
			}
			else {
				ShowAll();
				Cbool = true;
			}
			break;
		}
		case 'd': {
			if (Dbool == true) {
				DReverseLines();
				Dbool = false;
			}
			else {
				ShowAll();
				Dbool = true;
			}
			break;
		}
		case 'e': {
			if (Ebool == true) {
				EInsertAlpha();
				Ebool = false;
			}
			else {
				ShowAll();
				Ebool = true;
			}
			break;
		}
		case 'f': {
			if (Fbool) {
				FReverseWords();
				Fbool = false;
			}
			else {
				ShowAll();
				Fbool = true;
			}
			break;
		}
		case 'g': {
			if (Gbool) {
				char oldChar, newChar;

				std::cout << "변경할 문자를 입력하세요 : ";
				std::cin >> oldChar;
				std::cout << "새로운 문자를 입력하세요 : ";
				std::cin >> newChar;

				file.clear();
				file.seekg(0);

				std::string line;
				while (std::getline(file, line)) {
					GChangeChar(line, oldChar, newChar);
				}

				Gbool = false;
			}
			else {
				ShowAll();
				Gbool = true;
			}
			break;
		}
		case 'h': {
			HCountWords();
			break;
		}
		case 'r': {
			if (Rbool == 0) {
				RUp();
				Rbool = 1;
			}
			else if (Rbool == 1) {
				RDown();
				Rbool = 2;
			}
			else if (Rbool == 2) {
				ShowAll();
				Rbool = 0;
			}
			break;
		}
		case 's': {
			int wordCount = 0;

			std::string searchWord;
			std::cout << "검색할 단어를 입력하세요 : ";
			std::cin >> searchWord;

			SFindWord(searchWord, wordCount);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			break;
		}
		case 'q' : {
			std::cout << "프로그램을 종료합니다." << std::endl;
			exit(0);
			break;
		}
		default: {
			std::cout << "잘못된 입력입니다." << std::endl;
			break;
		}
		}
	}

	file.close();
}

void ShowAll() {
	file.clear();
	file.seekg(0);

	std::string line;
	while (std::getline(file, line)) {
		std::cout << line << std::endl;
	}
}

void CFindWord(const std::string& line, int& ChangeWords) {
	std::istringstream iss(line);
	std::string word;

	while (iss >> word) {
		if (std::isalpha(word[0]) && std::isupper(word[0])) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			std::cout << word << " ";
			ChangeWords++;
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			std::cout << word << " ";
		}
	}
	std::cout << std::endl;
}

void DReverseLines() {
	file.clear();
	file.seekg(0);

	std::string line;
	while (std::getline(file, line)) {
		std::reverse(line.begin(), line.end());
		std::cout << line << std::endl;
	}
}

void EInsertAlpha() {
	file.clear();
	file.seekg(0);

	std::string line;
	while (std::getline(file, line)) {
		std::string ChangeLine;
		int count = 0;

		for (char c : line) {
			if (!std::isspace(c)) {
				count++;
			}
			ChangeLine += c;

			if (count % 3 == 0 && !std::isspace(c)) {
				ChangeLine += "@@";
			}
		}

		std::cout << ChangeLine << std::endl;
	}
}

void FReverseWords() {
	file.clear();
	file.seekg(0);

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string word;

		while (iss >> word) {
			std::reverse(word.begin(), word.end());
			std::cout << word << " ";
		}

		std::cout << std::endl;
	}
}

void GChangeChar(const std::string& line, const char oldChar, const char newChar) {
	std::string Changeline = line;
	std::replace(Changeline.begin(), Changeline.end(), oldChar, newChar);
	std::cout << Changeline << std::endl;
}

void HCountWords() {
	file.clear();
	file.seekg(0);

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int wordCount = 0;
		std::string word;

		while (iss >> word) {
			++wordCount;
		}

		std::cout << "이 줄의 단어 개수 : " << wordCount << std::endl;
	}
}

void RUp() {
	file.clear();
	file.seekg(0);

	std::vector<std::pair<int, std::string>> lines;

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int wordCount = 0;
		std::string word;

		while (iss >> word) {
			++wordCount;
		}

		lines.emplace_back(wordCount, line);
	}

	std::sort(lines.begin(), lines.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;
	});

	for (const auto& pair : lines) {
		std::cout << pair.second << " - " << "단어의 개수 : " << pair.first << std::endl;
	}
}

void RDown() {
	file.clear();
	file.seekg(0);

	std::vector<std::pair<int, std::string>> lines;

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int wordCount = 0;
		std::string word;

		while (iss >> word) {
			++wordCount;
		}

		lines.emplace_back(wordCount, line);
	}

	std::sort(lines.begin(), lines.end(), [](const auto& a, const auto& b) {
		return a.first > b.first;
		});

	for (const auto& pair : lines) {
		std::cout << pair.second << " - " << "단어의 개수 : " << pair.first << std::endl;
	}
}

void SFindWord(const std::string& searchWord, int& wordCount) {
	auto toLower = [](const std::string& str) {
		std::string lowerStr;
		lowerStr.reserve(str.size());
		std::transform(str.begin(), str.end(), std::back_inserter(lowerStr), [](unsigned char c) {
			return std::tolower(c);
		});
		return lowerStr;
		};
	file.clear();
	file.seekg(0);

	std::string line;
	std::string lowerSearchWord = toLower(searchWord);

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string word;
		std::string lowerSearchWord = toLower(searchWord);

		while (iss >> word) {
			std::string lowerWord = toLower(word);

			if (lowerWord == lowerSearchWord) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
				std::cout << word << " ";
				wordCount++;
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				std::cout << word << " ";
			}
		}

		std::cout << std::endl;
	}
}