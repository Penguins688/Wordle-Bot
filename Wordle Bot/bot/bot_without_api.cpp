#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

enum class Feedback { Gray, Yellow, Green };

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
        while (std::getline(file, word)) {
            if (word.length() == 5) {
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                words.push_back(word);
            }
        }
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

int score_word(const std::string& word, const std::map<char, int>& overall_frequencies, const std::vector<std::map<char, int>>& column_frequency, int attempt) {
    int score = 0;

    if (attempt == 1) score += count_repeating_letters(word) * -10000; 

    for (size_t i = 0; i < word.length(); i++) {
        if (overall_frequencies.count(word[i])) score += overall_frequencies.at(word[i]);
        if (column_frequency[i].count(word[i])) score += column_frequency[i].at(word[i]);
    }
    return score;
}

std::vector<Feedback> parse_feedback(const std::string& input) {
    std::vector<Feedback> feedback;
    for (char c : input) {
        if (c == 'g') feedback.push_back(Feedback::Green);
        else if (c == 'y') feedback.push_back(Feedback::Yellow);
        else feedback.push_back(Feedback::Gray);
    }
    return feedback;
}

bool is_valid(const std::string& word, const std::string& guess, const std::vector<Feedback>& feedback) {
    std::map<char, int> guess_letter_counts;
    for (char c : guess) guess_letter_counts[c]++;
    std::map<char, int> word_letter_counts;

    for (char c : word) word_letter_counts[c]++;

    for (int i = 0; i < 5; ++i) {
        char g = guess[i];
        char w = word[i];

        switch (feedback[i]) {
            case Feedback::Green:
                if (w != g) return false;
                break;

            case Feedback::Yellow:
                if (w == g) return false;
                if (word.find(g) == std::string::npos) return false;
                break;

            case Feedback::Gray:
                {
                    int required_count = 0;
                    for (int j = 0; j < 5; ++j) {
                        if ((feedback[j] == Feedback::Green || feedback[j] == Feedback::Yellow) && guess[j] == g) {
                            required_count++;
                        }
                    }
                    if (word_letter_counts[g] > required_count) return false;
                }
                break;
        }
    }
    return true;
}

int main() {
    std::vector<std::string> possible_words = load_words("data/words.txt");
    std::map<char, int> overall = get_overall_frequencies();
    std::vector<std::map<char, int>> column = get_column_frequencies();

    for (int attempt = 1; attempt <= 6; ++attempt) {
        if (possible_words.empty()) {
            std::cout << "No possible words left. Something went wrong." << std::endl;
            break;
        }

        std::vector<std::pair<std::string, int>> scored_words;
        for (const auto& w : possible_words) {
            scored_words.emplace_back(w, score_word(w, overall, column, attempt));
        }
        std::sort(scored_words.begin(), scored_words.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        std::string guess;
        bool accepted = false;
        size_t guess_index = 0;

        while (!accepted && guess_index < scored_words.size()) {
            guess = scored_words[guess_index].first;
            std::cout << "Guess #" << attempt << ": " << guess << std::endl;

            std::cout << "Do you accept this guess? (y = yes, n = no, q = quit): ";
            std::string user_choice;
            std::cin >> user_choice;

            if (user_choice == "y") {
                accepted = true;
            } else if (user_choice == "q") {
                std::cout << "Quitting..." << std::endl;
                return 0;
            } else {
                guess_index++;
                if (guess_index >= scored_words.size()) {
                    std::cout << "No more guesses available." << std::endl;
                    return 0;
                }
            }
        }

        std::string feedback_str;
        std::cout << "Enter feedback (g = green, y = yellow, b = gray): ";
        std::cin >> feedback_str;

        if (feedback_str == "ggggg") {
            std::cout << "Solved in " << attempt << " attempts!" << std::endl;
            break;
        }

        std::vector<Feedback> feedback = parse_feedback(feedback_str);

        std::vector<std::string> filtered_words;
        for (const auto& word : possible_words) {
            if (is_valid(word, guess, feedback)) {
                filtered_words.push_back(word);
            }
        }

        possible_words = filtered_words;
    }

    return 0;
}
