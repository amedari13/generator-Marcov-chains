// ConsoleApplication3.cpp: определяет точку входа для консольного приложения.
//

//#include "stdafx.h"

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <locale>
#include <codecvt>
#include <cwctype>
#include <iostream>

class Generator
{
	std::map < std::wstring, std::vector < std::wstring>> map_;

public:
	Generator()
	{
		open("text.txt");
	}

	std::wstring take_word(std::wstring& line)
	{
		for (int start = 0; start < (int)line.size(); ++start) {
			if (!iswspace(line[start])) {
				int end = start;
				for (; end < (int)line.size() && !iswspace(line[end]); ++end)
				{
				}
				std::wstring result = line.substr(start, end - start);
				line = line.substr(end);
				return result;
			}
		}
		line = std::wstring();
		return std::wstring();
	}

	void open(std::string filename)
	{
		setlocale(LC_ALL, "Russian");

		std::locale::global(std::locale(""));
		std::locale l_utf8(std::locale(), new std::codecvt_utf8<wchar_t, 0x10ffffUL, std::codecvt_mode::consume_header>);

		std::wifstream in(filename);
		in.imbue(l_utf8);

		std::wstring prev_word;
		if (!in) return;
		std::wstring line;
		while (std::getline(in, line)) {
			while (true) {
				std::wstring word = take_word(line);
				if (word.empty()) break;

				// магия
				if (!prev_word.empty()) {
					map_[prev_word].push_back(word);
				}

				prev_word = word;
			}
		}

		int x = map_.size();
	}

	std::wstring make_sentense()
	{
		std::vector<std::wstring> keys;
		// std::map<std::wstring, std::vector<std::wstring>>::iterator

		// for (auto pair; map_) {
		// keys.push_back(pair.first);
		// }

		for (auto iter = map_.begin();
			iter != map_.end();
			++iter)
		{
			keys.push_back(iter->first);
		}

		std::wstring word = keys[rand() % keys.size()];
		std::wstring result = word;
		for (;;) {
			auto& vec = map_[word];
			std::wstring next = vec[rand() % vec.size()];
			result += L" " + next;

			if (next.back() == L'.'
				|| next.back() == L'?'
				|| next.back() == L'!')
				break;

			word = next;
		}

		return result;
	}
};


int main()
{
	Generator gen;

	srand((unsigned)time(0));

	for (int index = 0; index < 10; ++index) {
		std::wstring text = gen.make_sentense();
		std::wcout << text << std::endl;
	}

	return 0;
}