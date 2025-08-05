#include "winapi.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdio>

void WinCreateProc(std::string cmdLine) 
{
	#ifdef _WIN32
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
	ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
	
	if (CreateProcess(NULL, const_cast<char*>(cmdLine.data()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		printf("CreateProcess failed (%d).\n", GetLastError());
	}
	#else
	printf("This is for Windows API only");
	#endif
}

void WinVsWhere(std::string path)
{
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;

	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
   	saAttr.bInheritHandle = TRUE; 
   	saAttr.lpSecurityDescriptor = NULL; 

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) 
    {
		printf("StdoutRd CreatePipe");
	  	return;
	}

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		printf("Stdout SetHandleInformation");
		return;
	}
	
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
	ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
	si.hStdError = g_hChildStd_OUT_Wr;
   	si.hStdOutput = g_hChildStd_OUT_Wr;
   	si.dwFlags |= STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL, const_cast<char*>(path.data()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		printf("WinVsWhere: CreateProcess error: %d", GetLastError());
		return;
	} else {
		CloseHandle(pi.hProcess);
      	CloseHandle(pi.hThread);

		CloseHandle(g_hChildStd_OUT_Wr);
      	CloseHandle(g_hChildStd_IN_Rd);
	}

	DWORD dwRead;
	CHAR charBuf[4096];
	BOOL bSuccess = FALSE;

	for (;;) {
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, charBuf, 4096, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		if (!bSuccess) break;
	}

	std::string bufferString = charBuf;
	VsInfo info = ParseVsWhereData(bufferString);
	
	printf("%s \n", info.installationPath.c_str());
}

VsInfo ParseVsWhereData(std::string output)
{
	VsInfo struc;
	std::size_t found = output.find("installationPath:");
	std::string installationPath;

	int charIndex = found + 17;
	for (char c = output[charIndex]; c != '\n'; c = output[charIndex]) {
		installationPath.push_back(c);
		++charIndex;
	}
	struc.installationPath = installationPath;

	return struc;
}