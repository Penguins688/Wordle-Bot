#include <vector>
#include <fstream>
#include <string>
#include <iostream>

int main() {
    std::ifstream file("data/words.txt");
    std::vector<std::string> words;
    std::string word;

    if (file.is_open()) {
        while (std::getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    } else std::cerr << "Unable to open file" << std::endl;

    words.erase(std::remove_if(words.begin(), words.end(),[](const std::string& w) {
        return w.length() >= 5 && w[4] == 's';
    }),
    words.end());

    std::ofstream outfile("word_scores.txt");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < words.size(); i++) outfile << words[i] << std::endl;
    outfile.close();
    
    return 0;
}