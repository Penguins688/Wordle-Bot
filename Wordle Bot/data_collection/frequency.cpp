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
    std::map<std::string, int> letter_data = load_map();

    if (file.is_open()) {
        while (std::getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    } else std::cerr << "Unable to open file" << std::endl;

    for (size_t i = 0; i < words.size(); i++) {
        for (size_t j = 0; j < words[i].length(); j++) {
            std::string letter(1, words[i][j]);
            if (letter_data.find(letter) != letter_data.end()) {
                letter_data[letter] += 1;
            }
        }
    }

    for (const auto& pair : letter_data) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
