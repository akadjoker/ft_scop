#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> Split(const std::string &s, char delimiter) 
{
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;

    while (std::getline(tokenStream, token, delimiter))
    {
        if (token == "")
            continue;
        tokens.push_back(token);
    }

    return tokens;
}

bool hasNumberBeforeFirstSlash(const std::string &input)
{
    size_t firstSlash = input.find('/');
    return (firstSlash != std::string::npos && firstSlash > 0 && std::isdigit(input[firstSlash - 1]));
}

bool hasNumberBetweenSlashes(const std::string &input)
{
    size_t firstSlash = input.find("//");
    return (firstSlash == std::string::npos);
}

bool hasNumberAfterLastSlash(const std::string &input)
{
    size_t lastSlash = input.find_last_of('/');
    return (lastSlash != std::string::npos && lastSlash < input.size() - 1 && std::isdigit(input[lastSlash + 1]));
}

void analyzeFormat(const std::string &components, bool &hasVertex, bool &hasUV, bool &hasNormal)
{
    hasVertex = hasNumberBeforeFirstSlash(components);
    hasUV = hasNumberBetweenSlashes(components);
    hasNormal = hasNumberAfterLastSlash(components);
}

int main() 
{
    {
        std::string string ="1/1/1";
        bool hasVertex, hasUV, hasNormal=false;

        analyzeFormat(string, hasVertex, hasUV, hasNormal);

        std::cout << "hasVertex: " << hasVertex << "\n";
        std::cout << "hasUV: " << hasUV << "\n";
        std::cout << "hasNormal: " << hasNormal << "\n";
        std::cout << "-----------------------------------------------" << "\n";
    }

    {
        std::string string ="1//1";
        bool hasVertex, hasUV, hasNormal;
        analyzeFormat(string, hasVertex, hasUV, hasNormal);

        std::cout << "hasVertex: " << hasVertex << "\n";
        std::cout << "hasUV: " << hasUV << "\n";
        std::cout << "hasNormal: " << hasNormal << "\n";
        std::cout << "-----------------------------------------------" << "\n";
    }

    {
        std::string string ="1/1/";
        bool hasVertex, hasUV, hasNormal;
        analyzeFormat(string, hasVertex, hasUV, hasNormal);

        std::cout << "hasVertex: " << hasVertex << "\n";
        std::cout << "hasUV: " << hasUV << "\n";
        std::cout << "hasNormal: " << hasNormal << "\n";
        std::cout << "-----------------------------------------------" << "\n";
    }

    {
        std::string string ="1 1 1";
        bool hasVertex, hasUV, hasNormal;
        analyzeFormat(string, hasVertex, hasUV, hasNormal);

        std::cout << "hasVertex: " << hasVertex << "\n";
        std::cout << "hasUV: " << hasUV << "\n";
        std::cout << "hasNormal: " << hasNormal << "\n";
        std::cout << "-----------------------------------------------" << "\n";
    }

    return 0;
}
