#include "winapi.hpp"
#include <windows.h>
#include <cstdio>

void WinCreateProc(std::string cmdLine) 
{
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
}