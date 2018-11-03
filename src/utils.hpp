#pragma once
#include <vector>
#include <string>

namespace Util
{
    enum GetFileNamesMethod
    {
        FILES_AND_FOLDERS,
        FILES_ONLY,
        FOLDERS_ONLY
    };

    std::vector<std::string> getFileNames(const std::string&, const GetFileNamesMethod, const std::string& extention = "");
}