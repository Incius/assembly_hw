

#include <iostream>
#include <thread>
#include<vector>
#include<sstream>
#include <fstream>
//читаем ключи
std::vector<char> ReadKeys(std::string path) {
	std::ifstream in(path);
	std::vector<char> keys;
	if (in.good()) {
			for (size_t i = 0; i < 26; i++)
			{
				std::string item;
				getline(in, item, ' ');

				keys.push_back(std::stoi(item));
			}
	}
	else {
		throw std::runtime_error("Cant read Keys");
	}
	return keys;
}
//потоки
void DecryptionStream(std::vector<char> keys,std::string &text,int start, int end) {
	for (int i = start; i < end; i++)
	{
		auto item = find(keys.begin(), keys.end(), text[i]);
		int index = item - keys.begin();
		if (std::find(keys.begin(), keys.end(), text[i]) != keys.end()) {
			text[i] = (char)('a' + index);
		}
	}
}
//метод дешифровки
void Decryption(std::string path, std::vector<char> keys) {
	//чтение
	std::ifstream in(path);
	std::string text;
	std::vector<std::string> lines;
	while (!in.eof()) {
		std::string line;
		std::getline(in, line, '\n');
		lines.push_back(line);
	}
	in.close();
	std::ofstream out("answer.txt");
	//вывод
	for (size_t i = 0; i < lines.size(); i++)
	{
		int size = lines[i].size() / 4;
		int add = lines[i].size() % 4;
		std::thread a(DecryptionStream, keys, std::ref(lines[i]), 0, size);
		std::thread b(DecryptionStream, keys, std::ref(lines[i]), size, size*2);
		std::thread c(DecryptionStream, keys, std::ref(lines[i]), size*2, size*3);
		std::thread d(DecryptionStream, keys, std::ref(lines[i]), size*3, size*4+add);
		a.join();
		b.join();
		c.join();
		d.join();
		out << lines[i] << std::endl;
		std::cout << lines[i] << std::endl;
	}
	out.close();
}
int main()
{
	bool flag = false;
	std::string path;
	std::vector<char> keys;
	//повтор ввода
	do{
		try {
		std::cout << "Input path to keys" << std::endl;
		std::cin >> path;
		keys= ReadKeys(path);
		flag = true;
		}
		catch (...) {
			std::cout << "incorrect input" << std::endl;
		}
	} while (!flag);
	std::cout <<"Input path to text:" <<std::endl;
	std::cin >> path;
	Decryption(path, keys);
}

