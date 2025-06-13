#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

std::map<char, int> get_overall_frequencies() {
    return {{'e', 7455}, {'s', 7319}, {'a', 7128}, {'o', 5212}, {'r', 4714}, {'i', 4381}, {'l', 3780}, {'t', 3707}, {'n', 3478}, {'d', 2735}, {'u', 2927}, {'p', 2436}, {'m', 2414}, {'y', 2400}, {'c', 2246}, {'h', 1993}, {'g', 1864}, {'b', 1849}, {'k', 1753}, {'f', 1240}, {'w', 1127}, {'v', 801},  {'z', 503},  {'j', 342},  {'x', 326}, {'q', 145}};
}

std::vector<std::map<char, int>> get_column_frequencies() {
    std::vector<std::map<char, int>> col_freqs(5);
    col_freqs[0] = {{'s',1666},{'p',1130},{'b',1003},{'m',951},{'t',882},{'a',868},{'r',795},{'c',970},{'d',735},{'g',685},{'f',646},{'l',625},{'h',532},{'w',434}, {'k',429},{'n',468},{'o',352},{'e',330},{'v',284},{'y',205},{'u',217}, {'j',225},{'i',180},{'q',103},{'z',122},{'x',18}};
    col_freqs[1] = {{'a',2682},{'o',2414},{'e',1857},{'i',1598},{'u',1368},{'r',1019},{'l',774}, {'h',604},{'n',388},{'m',211},{'c',207},{'w',177},{'y',298},{'t',256}, {'p',249},{'b',108},{'d',108},{'k',114},{'s',113},{'g',94},{'v',57}, {'x',63},{'f',37},{'z',30},{'q',16},{'j',13}};
    col_freqs[2] = {{'a',1374},{'r',1354},{'i',1173},{'n',1119},{'o',1113},{'e',998},{'l',973}, {'s',613},{'m',580},{'u',754},{'t',735},{'d',467},{'c',467},{'k',329}, {'w',305},{'y',246},{'v',278},{'g',427},{'p',426},{'b',386},{'h',146}, {'x',146},{'z',165},{'f',204},{'j',59},{'q',18}};
    col_freqs[3] = {{'e',2531},{'a',1319},{'t',1039},{'i',1048},{'r',809},{'n',885},{'l',869}, {'o',827},{'s',588},{'d',551},{'k',571},{'u',492},{'g',487},{'p',466}, {'m',445},{'c',456},{'h',283},{'b',282},{'v',177},{'w',143},{'z',145}, {'y',124},{'f',258},{'j',42},{'x',15},{'q',3}};
    col_freqs[4] = {{'s',4339},{'e',1739},{'y',1527},{'a',885},{'d',874},{'r',737},{'n',618}, {'l',539},{'h',428},{'o',506},{'i',382},{'k',310},{'m',227},{'g',171}, {'p',165},{'c',146},{'f',95},{'u',96},{'x',84},{'b',70},{'w',68}, {'z',41},{'q',5},{'v',5},{'j',3}};
    return col_freqs;
}

std::vector<std::string> load_words(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string word;

    if (file.is_open()) {
        while (std::getline(file, word)) if (word.length() == 5) words.push_back(word);
        file.close();
    } else std::cerr << "Unable to open file: " << filename << std::endl;

    return words;
}

int count_repeating_letters(const std::string& word) {
    std::map<char, int> letter_counts;
    for (char c : word) letter_counts[c]++;
    int repeating = 0;
    for (const auto& pair : letter_counts) if (pair.second > 1) repeating++;
    return repeating;
}

int score_word(std::string word, std::map<char, int> overall_frequencies, std::vector<std::map<char, int>> column_frequency) {
    int score = 0;
    score+=count_repeating_letters(word)*-10000;
    for (size_t i = 0; i < word.length(); i++) {
        score+=overall_frequencies[word[i]];
        score+=column_frequency[i][word[i]];
    }
    
    return score;
}

int main() {
    std::vector<std::string> words = load_words("words.txt");
    std::map<char, int> overall_frequencies = get_overall_frequencies();
    std::vector<std::map<char, int>> column_frequencies = get_column_frequencies();
    std::map<std::string, int> word_scores;
    
    std::ofstream outfile("word_scores.txt");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    int score;
    int highest_score = 0;
    std::pair<std::string, int> best_word;
    for (size_t i = 0; i < words.size(); i++) {
        score = score_word(words[i], overall_frequencies, column_frequencies);
        word_scores.insert({words[i], score});

        std::cout << words[i] << ": " << score << std::endl;
        outfile << words[i] << ": " << score << std::endl;

        if (score > highest_score) { 
            highest_score = score; 
            best_word = {words[i], score};
        }
    }

    std::cout << "\nBest word: " << best_word.first << " (Score: " << best_word.second << ")" << std::endl;
    outfile << "\nBest word: " << best_word.first << " (Score: " << best_word.second << ")" << std::endl;

    outfile.close();

    return 0;
}
