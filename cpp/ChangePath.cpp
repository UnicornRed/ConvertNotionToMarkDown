#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

int main(int argn, char* argv[])
{
    size_t level = 0, pos, prev_pos;
    std::string str;
    std::ifstream filesPath, hierarchyFile;
    filesPath.open(argv[1], std::ios::in);
    hierarchyFile.open(argv[2], std::ios::in);
    std::ofstream correctFilesPath;
    correctFilesPath.open(std::string("Correct") + argv[1], std::ios::out);
    std::map<std::string, std::string> pathToPath;
    std::vector<std::string> correctHierarchy;

    while (std::getline(hierarchyFile, str))
        correctHierarchy.push_back(str);

    while (std::getline(filesPath, str))
    {
        size_t lenStr = str.length();

        prev_pos = 0;
        pos = 0;

        while ((pos = str.find('/', pos)) != std::string::npos)
        {
            prev_pos = pos;
            ++pos;
        }

        size_t posMd = str.find(".md", 0);

        if (posMd == std::string::npos)
            posMd = 0;

        pos = std::max(posMd, prev_pos); 
        
        if (pathToPath.count(str.substr(0, pos - 1)) == 0)
        {
            pathToPath[str.substr(0, pos - 1)] = correctHierarchy[level];
            ++level;
        }

        correctFilesPath << str << std::endl;

        std::string noSpaces = str.substr(pos, lenStr - pos);
        size_t posSpaces = 0;

        while((posSpaces = noSpaces.find(' ', 0)) != std::string::npos)
            noSpaces.replace(posSpaces, 1, "");
        
        correctFilesPath << pathToPath[str.substr(0, pos - 1)] << noSpaces << (posMd != 0 ? ".e" : "") << std::endl;
    }
    

    filesPath.close();
    hierarchyFile.close();
    correctFilesPath.close();

    return 0;
}