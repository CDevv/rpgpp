#include "winapi.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdio>

#ifdef _WIN32

char *WinReadFromHandle(HANDLE handle) {
	DWORD dwRead;
	CHAR charBuf[4096];
	BOOL bSuccess = FALSE;

	for (;;) {
		bSuccess = ReadFile(handle, charBuf, 4096, &dwRead, NULL);
		if (!bSuccess || dwRead == 0)
			break;

		if (!bSuccess)
			break;
	}

	return charBuf;
}

void WinWriteToHandle(HANDLE handle, std::string str) {
	DWORD dwWritten;
	CHAR charBuf[4096];
	BOOL bSuccess = FALSE;

	bSuccess = WriteFile(handle, static_cast<char *>(str.data()), str.size(),
						 &dwWritten, NULL);
	if (!bSuccess) {
		printf("WriteFile Error: %d", GetLastError());
		return;
	}

	if (!CloseHandle(handle)) {
		printf("WinWriteToHandle: CloseHandle error");
	}
}

void WinCreateProcEx(std::string cmdLine, HANDLE outHandle, HANDLE inHandle,
					 DWORD dwFlags, bool wait) {
#ifdef _WIN32
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (outHandle != NULL) {
		si.hStdError = outHandle;
		si.hStdOutput = outHandle;
	}
	if (inHandle != NULL) {
		si.hStdInput = inHandle;
	}
	si.dwFlags |= dwFlags;

	BOOL handleInheritance = FALSE;
	if (dwFlags == STARTF_USESTDHANDLES) {
		handleInheritance = TRUE;
	}

	if (CreateProcess(NULL, const_cast<char *>(cmdLine.data()), NULL, NULL,
					  handleInheritance, 0, NULL, NULL, &si, &pi)) {
		// Wait until child process exits.
		if (wait) {
			WaitForSingleObject(pi.hProcess, INFINITE);
		}

		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (outHandle != NULL)
			CloseHandle(outHandle);
		if (inHandle != NULL)
			CloseHandle(inHandle);
	} else {
		printf("WinCreateProc: CreateProcess failed (%d).\n", GetLastError());
	}
#else
	printf("This is for Windows API only");
#endif
}

void WinCreateProc(std::string cmdLine) {
	WinCreateProcEx(cmdLine, NULL, NULL, STARTF_FORCEONFEEDBACK, true);
}

VsInfo WinVsWhere(std::string path) {
	VsInfo result;

	printf("%s : \n", path.c_str());

	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;

	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
		printf("StdoutRd CreatePipe");
		return result;
	}

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		printf("Stdout SetHandleInformation");
		return result;
	}

	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
		printf("Stdin CreatePipe");
		return result;
	}

	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
		printf("Stdin SetHandleInformation");
		return result;
	}

	WinCreateProcEx(path, g_hChildStd_OUT_Wr, NULL, STARTF_USESTDHANDLES,
					false);

	CHAR *charBuf = WinReadFromHandle(g_hChildStd_OUT_Rd);
	std::string bufferString = charBuf;
	VsInfo info = ParseVsWhereData(bufferString);
	result = info;

	return result;
}

VsInfo ParseVsWhereData(std::string output) {
	VsInfo struc;
	std::size_t found = output.find("installationPath:");
	std::string installationPath;

	int charIndex = found + 18;
	for (char c = output[charIndex]; c != '\n'; c = output[charIndex]) {
		if (c != '\r') {
			installationPath.push_back(c);
		}
		charIndex++;
	}
	std::string vsdevcmdPath = installationPath;
	vsdevcmdPath = vsdevcmdPath.append("\\Common7\\Tools\\vsdevcmd.bat");
	struc.installationPath = std::string(installationPath.c_str());
	struc.vsdevcmdPath = std::string(vsdevcmdPath);
	struc.auxiliaryPath = installationPath.append("\\VC\\Auxiliary\\Build");

	return struc;
}

#else

void WinCreateProc(std::string cmdLine) {}
VsInfo WinVsWhere(std::string path) {
	VsInfo info;
	return info;
}
VsInfo ParseVsWhereData(std::string output) {
	VsInfo info;
	return info;
}

#endif