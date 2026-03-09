#include "Utils.hpp"

std::vector<std::string> splitString(const std::string &string, char delimeter)
{
    std::vector<std::string> result;

    std::string acc;

    for(char c : string) {
        if(c == delimeter) {
            result.emplace_back(acc);
            acc.clear();
            continue;
        }
        acc+=c;
    }

    result.emplace_back(acc);

    return result;
}

std::string joinVector(const std::vector<std::string> &source, char delimeter)
{
    std::string acc = "";
    for(const auto& el : source) {
        acc += el + delimeter;
    }
    return acc;
}
