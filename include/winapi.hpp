#ifndef _WINAPI_H
#define _WINAPI_H

#include <string>

struct VsInfo {
    std::string installationPath;
};

void WinCreateProc(std::string cmdLine);
void WinVsWhere(std::string path);
VsInfo ParseVsWhereData(std::string output);

#endif