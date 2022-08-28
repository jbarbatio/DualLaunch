// DualLaunchLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <shellapi.h>
#include <cassert>
#include <string>
#include "SharedCommandLine.h"
#include <detours.h>
#include "DualLaunchLib.h"

BOOL __stdcall ShellExecuteExWDetour(_Inout_ SHELLEXECUTEINFOW* pExecInfo) {
    ::OutputDebugStringW(pExecInfo->lpParameters);

    std::wstring str = pExecInfo->lpFile;
    str += L" ";
    str += pExecInfo->lpParameters;
    const std::string s(str.begin(), str.end());
    
    duallaunch::cmdline::Set(s.c_str());
    ::MessageBoxW(NULL, pExecInfo->lpParameters, pExecInfo->lpVerb, MB_OK);
    ExitProcess(0);
    return NULL;
}

void duallaunch::PatchFirst() {
    HMODULE hMod = NULL;
    FARPROC proc = NULL;
    
    hMod = ::LoadLibraryA("SHELL32.DLL");
    assert(hMod != nullptr || "LoadLibraryA SHELL32");

    proc = ::GetProcAddress(hMod, "ShellExecuteExW");
    assert(proc != nullptr || "GetProcAddress ShellExecuteExW");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    LONG res = DetourAttach(&(PVOID&)proc, ShellExecuteExWDetour);
    assert(res == 0 || "DetourAttachEx");

    res = DetourTransactionCommit();
    assert(res == 0 || "DetourTransactionCommit");
}

void duallaunch::PatchSecond() {

}