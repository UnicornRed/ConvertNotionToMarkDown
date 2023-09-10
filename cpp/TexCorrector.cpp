#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

std::string& makeStrTex(std::string& str, const std::map<std::string, std::string>& command)
{
    size_t pos = 0;

    for (const auto& c : command)
    {
        pos = 0;

        while ((pos = str.find(c.first, pos)) != std::string::npos)
        {
            if (pos + c.first.length() == str.length() || !std::isalpha(str[pos + c.first.length()]))
                str.replace(pos, c.first.length(), c.second);

            pos += c.first.length();
        }
    }
    
    return str;
}

int main (int argn, char* argv[])
{
    std::string changeCommand = argv[1];
    std::string md = argv[2];
    std::string output = argv[3];

    std::string str, correctStr;

    std::ifstream changeCommandFile, mdFile;
    changeCommandFile.open(changeCommand, std::ios::in);
    mdFile.open(md, std::ios::in);
    std::ofstream outputFile;
    outputFile.open(output, std::ios::out);

    std::cout << "Start: " << output << std::endl;

    std::map<std::string, std::string> command;

    while (changeCommandFile >> str >> correctStr)
        command[str] = correctStr;

    str = "";

    while (getline(mdFile, str))
    {
        makeStrTex(str, command);

        outputFile << str << std::endl;
    }

    std::cout << "Finish: " << output << std::endl;

    changeCommandFile.close();
    mdFile.close();
    outputFile.close();

    return 0;
}