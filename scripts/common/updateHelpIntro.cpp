#include "ofxOfeliaMaps.h"
#include <iostream>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <unordered_map>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    const std::string &ofeliaDirectoryPath = "../../ofelia/";
    const std::string &ofeliaHelpDirectoryPath = ofeliaDirectoryPath + "help/";
    const std::string &classesAndGlobalFunctionsPath = ofeliaDirectoryPath + "classesAndGlobalFunctions.txt";
    const std::string &helpIntroPath = ofeliaDirectoryPath + "help-intro.pd";
    ofxOfeliaMaps maps;
    maps.init();
    ofxOfeliaMaps::FunctionData functionData;
    std::vector<std::string> abstractionNames;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(ofeliaHelpDirectoryPath.c_str())))
    {
        while ((ent = readdir(dir)))
        {
            std::string helpFileName = ent->d_name;
            if (!std::isalpha(helpFileName[0])) continue;
            const size_t lastDotPos = helpFileName.rfind('.');
            if (lastDotPos == std::string::npos) continue;
            const std::string &ext = helpFileName.substr(lastDotPos + 1);
            if (ext != "pd") continue;
            const size_t helpPos = helpFileName.rfind("-help");
            if (helpPos == std::string::npos) continue;
            const std::string &helpFilePath = ofeliaHelpDirectoryPath + helpFileName;
            struct stat info;
            if (!stat(helpFilePath.c_str(), &info) && info.st_mode & S_IFREG)
            {
                helpFileName = helpFileName.substr(0, helpPos);
                abstractionNames.push_back(helpFileName);
            }
        }
        closedir(dir);
    }
    for (size_t i = 0; i < abstractionNames.size(); ++i)
        maps.functionMap[abstractionNames[i]] = functionData;
    std::unordered_map<std::string, std::string> objectMap;
    std::map<std::string, std::vector<std::string>> categoryMap;
    std::ifstream input(classesAndGlobalFunctionsPath);
    std::string line;
    for (std::string line; std::getline(input, line);)
    {
        const size_t firstSpacePos = line.find(' ');
        if (firstSpacePos != std::string::npos)
        {
            std::string objectName = line.substr(0, firstSpacePos);
            if (objectName.back() == '_')
                objectName.pop_back();
            std::string categoryName = line.substr(firstSpacePos + 1);
            const size_t secondSpacePos = categoryName.find(' ');
            if (secondSpacePos != std::string::npos)
                categoryName = categoryName.substr(0, secondSpacePos);
            const size_t subClassIndicatorPos = categoryName.find(':');
            if (subClassIndicatorPos != std::string::npos)
                categoryName = categoryName.substr(0, subClassIndicatorPos);
            objectMap.insert(std::make_pair(objectName, categoryName));
        }
    }
    input.close();
    auto functionMapIterator = maps.functionMap.begin();
    while (functionMapIterator != maps.functionMap.end())
    {
        const std::string &objectName = functionMapIterator->first;
        if (!objectName.empty())
        {
            if (objectMap.find(objectName) == objectMap.end())
            {
                const auto it = std::find_if(objectName.begin(), objectName.end(),
                                             [](char c) {return std::isupper(c);});
                const std::string &moduleName = objectName.substr(0, std::distance(objectName.begin(), it));
                objectMap.insert(std::make_pair(objectName, "misc/" + moduleName));
            }
            const std::string &categoryName = objectMap[objectName];
            categoryMap[categoryName].push_back(objectName);
        }
        functionMapIterator++;
    }
    auto classMapIterator = maps.classMap.begin();
    while (classMapIterator != maps.classMap.end())
    {
        std::string objectName = classMapIterator->first;
        if (objectMap.find(objectName) == objectMap.end())
            objectMap.insert(std::make_pair(objectName, "misc/" + objectName));
        const std::string &categoryName = objectMap[objectName];
        const auto it = std::find_if(objectName.begin(), objectName.end(),
                                     [](char c) {return std::isupper(c);});
        std::string varName = objectName.substr(std::distance(objectName.begin(), it));
        varName[0] = std::tolower(varName[0]);
        varName = " \\$0-" + varName;
        objectName = objectName + varName;
        categoryMap[categoryName].push_back(objectName);
        auto classData = classMapIterator->second;
        for (size_t i = 0; i < classData.size() - 1; ++i)
        {
            const std::string &methodName = classData[i].first;
            categoryMap[categoryName].push_back(objectName + ' ' + methodName);
        }
        classMapIterator++;
    }
    std::ostringstream ss;
    ss <<
    "#N canvas 520 30 660 700 10;\n"
    "#X declare -lib ofelia;\n"
    "#X obj 520 20 declare -lib ofelia;\n"
    "#X text 20 20 The following is a categorized list of built-in objects in ofelia. Double click on any object to open the help file or the documentation page in your browser.;\n";
    int categorySubPatchPosY = 80;
    int subCategorySubPatchPosY = 20;
    std::string categoryName = "";
    std::string categoryNameAlias = "";
    auto categoryMapIterator = categoryMap.begin();
    while (categoryMapIterator != categoryMap.end())
    {
        categoryName = categoryMapIterator->first;
        std::string subCategoryName = categoryName;
        const size_t slashPos = categoryName.find('/');
        if (slashPos != std::string::npos)
        {
            categoryName = categoryName.substr(0, slashPos);
            subCategoryName = subCategoryName.substr(slashPos + 1);
        }
        if (categoryName != categoryNameAlias)
        {
            if (categoryNameAlias != "")
            {
                ss << "#X restore 20 " << categorySubPatchPosY << " pd " << categoryNameAlias << ";\n";
                categorySubPatchPosY += 20;
            }
            ss << "#N canvas 540 120 620 590 " << categoryName << " 0;\n";
            categoryNameAlias = categoryName;
            subCategorySubPatchPosY = 20;
        }
        ss << "#N canvas 560 140 580 550 " << subCategoryName << " 0;\n";
        int objectPosY = 20;
        auto objectNames = categoryMapIterator->second;
        for (size_t i = 0; i < objectNames.size(); ++i)
        {
            ss << "#X obj 20 " << objectPosY << ' ' << objectNames[i] << ";\n";
            objectPosY += 20;
        }
        ss << "#X restore 20 " << subCategorySubPatchPosY << " pd " << subCategoryName << ";\n";
        subCategorySubPatchPosY += 20;
        categoryMapIterator++;
    }
    ss << "#X restore 20 " << categorySubPatchPosY << " pd " << categoryName << ";\n";
    std::ofstream output;
    output.open(helpIntroPath.c_str());
    output << ss.str();
    output.close();
}
