#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

std::string& makeStrTex(std::string& str, const std::map<std::string, std::string>& command, std::map<std::string, std::string> pathToPath)
{
    size_t pos = 0;

    while ((pos = str.find("%", pos)) != std::string::npos)
        str.replace(pos, 3, "");
//std::cout << str << std::endl;
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

    pos = 0;
//std::cout << str << std::endl;
    for (const auto& c : pathToPath)
    {
        pos = 0;
        while ((pos = str.find(c.first + ")", pos)) != std::string::npos)
        {
            str.replace(pos, c.first.length() + 1, c.second + ")");
            pos += c.first.length();
        }
    }
    
    return str;
}

int main (int argn, char* argv[])
{
    std::string changeCommand = argv[1];
    std::string output = argv[2];
    std::string md = argv[2];
    std::string namePNG = argv[2];
    std::string CorrectFilesPath = argv[3];
    namePNG = namePNG.replace(namePNG.length() - 3, namePNG.length() - 1, "");

    output = output.substr(0, output.length() - 2);

    std::string str, correctStr;

    std::ifstream changeCommandFile, mdFile, CorrectFilesPathFile;
    changeCommandFile.open(changeCommand, std::ios::in);
    mdFile.open(md, std::ios::in);
    CorrectFilesPathFile.open(CorrectFilesPath, std::ios::in);
    std::ofstream outputFile;
    outputFile.open(output, std::ios::out);

    std::map<std::string, std::string> command;
    std::map<std::string, std::string> pathToPath;

    while (changeCommandFile >> str >> correctStr)
        command[str] = correctStr;

    while (std::getline(CorrectFilesPathFile, str))
    {
        std::getline(CorrectFilesPathFile, correctStr);

        size_t pos = 0;

        if ((pos = correctStr.find(".md.e", pos)) != std::string::npos)
            correctStr.replace(pos, 5, ".md");

        for (size_t i{}; i < str.length(); ++i)
        {
            if (str[i] < 0 || str[i] == ' ')
            {
                str.replace(i, 1, "");
                --i;
            }
        }

        pathToPath[str] = correctStr;

        pos = 0;

        while ((pos = str.find('/', 0)) != std::string::npos)
        {
            str = str.substr(str.find('/', 0) + 1);
            correctStr = correctStr.substr(correctStr.find('/', 0) + 1);

            pathToPath[str] = correctStr;
        }

        //std::cout << str << " " << correctStr << "\n";
    }

    str = "";

    while (getline(mdFile, str))
    {
        makeStrTex(str, command, pathToPath);

        outputFile << str << std::endl;
    }

    changeCommandFile.close();
    mdFile.close();
    CorrectFilesPathFile.close();
    outputFile.close();

    return 0;
}