#include "utils.hpp"
#include <Windows.h>

namespace Util
{
    std::vector<std::string> getFileNames(const std::string& dirName, const GetFileNamesMethod method, const std::string& extention)
    {
        HANDLE handle;
        WIN32_FIND_DATA win32fd;
        std::vector<std::string> fileNames;

        std::string searchName = dirName + "/*" + extention;

        handle = FindFirstFile(searchName.c_str(), &win32fd);

        do
        {
            if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && method != FILES_ONLY)
            {
                if (win32fd.cFileName[0] != '.')
                {
                    fileNames.push_back(win32fd.cFileName);
                }
            }
            else if (method != FOLDERS_ONLY)
            {
                fileNames.push_back(win32fd.cFileName);
            }
        } while (FindNextFile(handle, &win32fd));

        FindClose(handle);

        return fileNames;
    }
}