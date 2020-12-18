// DLLInjector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
using namespace std;

int main()
{
    LPSTARTUPINFOA startupInfo = new STARTUPINFOA();
    LPPROCESS_INFORMATION processInfo = new PROCESS_INFORMATION();
    bool successful = CreateProcessA(
        NULL,
        (LPSTR)"HelloWorld.exe",
        NULL,
        NULL,
        true,
        CREATE_SUSPENDED,
        NULL,
        NULL,
        startupInfo,
        processInfo
    );
    if (successful) {
        cout << "Process created." << endl;
        HANDLE hThread = processInfo->hThread;
        wchar_t dllPath[] = TEXT("DLL.dll");
        PVOID pDllPath = VirtualAllocEx(processInfo->hProcess, NULL, sizeof dllPath, MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(processInfo->hProcess, pDllPath, (LPVOID)dllPath, sizeof dllPath, NULL);
        PTHREAD_START_ROUTINE pLoadLibrary = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
        HANDLE hInjectedThread = CreateRemoteThread(processInfo->hProcess, NULL, 0, pLoadLibrary, pDllPath, 0, NULL);
        if (hInjectedThread) {
            cout << "Injection succedded." << endl;
        }
        else {
            cout << "Injection failed." << endl;
            return 1;
        }
        HANDLE lpHandles[] = { hInjectedThread };
        WaitForMultipleObjects(1, lpHandles, true, INFINITE);
        ResumeThread(hThread);
    }
    else {
        cout << "Process creation failed." << endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
