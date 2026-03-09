#ifndef _WINAPI_H
#define _WINAPI_H

#include <string>

struct VsInfo {
	std::string installationPath;
	std::string vsdevcmdPath;
	std::string auxiliaryPath;
};

void WinCreateProc(std::string cmdLine);
VsInfo WinVsWhere(std::string path);
VsInfo ParseVsWhereData(std::string output);

#endif