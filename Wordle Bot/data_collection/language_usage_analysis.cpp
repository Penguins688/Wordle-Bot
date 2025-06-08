#include <iostream>
#include <cstdlib>
#include <string>
#include "../libraries/json.hpp"

using json = nlohmann::json;

std::string run_curl_command(const std::string& word) {
    std::string command = "curl -s \"https://api.datamuse.com/words?sp=" + word + "&md=f&max=1\"";
    std::string result;
    char buffer[128];
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "[]";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

float extract_frequency(const std::string& json_response) {
    try {
        auto data = json::parse(json_response);
        if (data.empty()) return 0.0f;

        auto tags = data[0]["tags"];
        for (const auto& tag : tags) {
            if (tag.is_string() && tag.get<std::string>().rfind("f:", 0) == 0) {
                std::string freq_str = tag.get<std::string>().substr(2);
                return std::stof(freq_str);
            }
        }
    } catch (...) {
        return 0.0f;
    }
    return 0.0f;
}

int main() {
    std::string word;
    std::cout << "Enter a 5-letter word: ";
    std::cin >> word;

    std::string response = run_curl_command(word);
    float frequency = extract_frequency(response);

    std::cout << "Estimated frequency of \"" << word << "\": " << frequency << " per million words" << std::endl;

    return 0;
}
