#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::map<std::string, int> load_map() {
    std::map<std::string, int> loaded_map;
    for (char c = 'a'; c <= 'z'; ++c) {
        loaded_map[std::string(1, c)] = 0;
    }
    return loaded_map;
}

int main() {
    std::ifstream file("data/words.txt");
    std::vector<std::string> words;
    std::string word;
    std::map<std::string, int> letter_data1 = load_map();
    std::map<std::string, int> letter_data2 = load_map();
    std::map<std::string, int> letter_data3 = load_map();
    std::map<std::string, int> letter_data4 = load_map();
    std::map<std::string, int> letter_data5 = load_map();

    if (file.is_open()) {
        while (std::getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    } else std::cerr << "Unable to open file" << std::endl;

    for (size_t i = 0; i < words.size(); i++) {
        std::string letter(1, words[i][0]);
        if (letter_data1.find(letter) != letter_data1.end()) {
            letter_data1[letter] += 1;
        }
        letter = words[i][1];
        if (letter_data2.find(letter) != letter_data2.end()) {
            letter_data2[letter] += 1;
        }
        letter = words[i][2];
        if (letter_data3.find(letter) != letter_data3.end()) {
            letter_data3[letter] += 1;
        }
        letter = words[i][3];
        if (letter_data4.find(letter) != letter_data4.end()) {
            letter_data4[letter] += 1;
        }
        letter = words[i][4];
        if (letter_data5.find(letter) != letter_data5.end()) {
            letter_data5[letter] += 1;
        }
    }

    std::cout << "column 1:\n";
    for (const auto& pair : letter_data1) std::cout << pair.first << ": " << pair.second << std::endl;
    std::cout << "column 2:\n";
    for (const auto& pair : letter_data2) std::cout << pair.first << ": " << pair.second << std::endl;
    std::cout << "column 3:\n";
    for (const auto& pair : letter_data3) std::cout << pair.first << ": " << pair.second << std::endl;
    std::cout << "column 4:\n";
    for (const auto& pair : letter_data4) std::cout << pair.first << ": " << pair.second << std::endl;
    std::cout << "column 5:\n";
    for (const auto& pair : letter_data5) std::cout << pair.first << ": " << pair.second << std::endl;

    return 0;
}
