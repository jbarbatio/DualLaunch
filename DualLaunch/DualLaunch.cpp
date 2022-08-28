// CockMonger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <string>
#include "SharedCommandLine.h"
namespace fs = std::filesystem;


std::string firstDll = "First.dll";
std::string firstExe = R"(C:\Program Files\game\exe1)";

std::string secondDll = "";
std::string secondExe = R"(C:\Program Files\game\exe2)";

void die(const char* msg) {
	int result = MessageBoxA(NULL, "Death", msg, MB_ABORTRETRYIGNORE);
	if (result == IDABORT) {
		DebugBreak();
	}
	else {
		exit(1);
	}
}
void LaunchInjectWait(const std::string& exename, const std::string& dllname, bool wait) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);


	LPVOID stringAddr = NULL;
	LPVOID LoadLib = NULL;

	LoadLib = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if (LoadLib == NULL) {
		die("LoadLibraryA couldn't be found");
	}
	char buf[256] = { 0 };


	if (duallaunch::cmdline::Exists()) {
		std::string cmdLine = duallaunch::cmdline::Get();

		strncpy_s(buf, cmdLine.c_str(), cmdLine.length());

		if (!CreateProcess(NULL, buf, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
			die("Create process failed");
		}
	}
	else {
		if (!CreateProcess(exename.c_str(), NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
			die("Create process failed");
		}
	}
	stringAddr = VirtualAllocEx(pi.hProcess, NULL, dllname.length(), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (stringAddr == NULL) {
		die("VirtualAlloc process failed");
	}

	WriteProcessMemory(pi.hProcess, stringAddr, dllname.c_str(), dllname.length(), NULL);
	CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, stringAddr, NULL, NULL);


	ResumeThread(pi.hThread);
	if (wait) {
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
int main()
{
	LaunchInjectWait(firstExe, firstDll, true);

	LaunchInjectWait(secondExe, secondDll,false);
	return 0;
}