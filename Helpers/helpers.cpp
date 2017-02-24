#include "StdAfx.h"
#include "helpers.h"

namespace OHelpers
{

    bool DirectoryExists(wchar_t* path)
    {
        DWORD attribs = ::GetFileAttributes(path);
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            return false;
        }
        return (attribs & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    void Clamp(int& val, int minVal, int maxVal)
    {
        if (val < minVal) {
            val = minVal;
        }
        if (val > maxVal) {
            val = maxVal;
        }
    }

}
