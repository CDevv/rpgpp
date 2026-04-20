#ifndef _WINAPI_H
#define _WINAPI_H

#include <string>

struct VsInfo {
	std::string installationPath;
	std::string vsdevcmdPath;
	std::string auxiliaryPath;
};

bool WinOpenFileAssociate(std::string operation, std::string file);
void WinCreateProc(std::string cmdLine);
VsInfo WinVsWhere(std::string path);
VsInfo ParseVsWhereData(std::string output);
bool WinCreateDetachedExecutable(std::string path);
void WinRunWithLog(std::string logName, std::string cmdLine);
#endif