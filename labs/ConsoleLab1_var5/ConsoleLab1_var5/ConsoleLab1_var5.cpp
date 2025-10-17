#ifdef _WIN32
#include <windows.h>
#include <cwchar> 
#else
#define _LINUX
#include <unistd.h> 
#include <cstdlib>  
#include <fstream>
#endif

#include <chrono>
#include <iostream>

int main()
{
#ifdef _WIN32
    wchar_t computerNameBuffer[256];
    LPWSTR computerName = computerNameBuffer;
    DWORD computerNameLenDword = 256;

    wchar_t userNameBuffer[256];
    LPWSTR userName = userNameBuffer;
    DWORD userNameLenDword = 256;

    GetUserName(userName, &userNameLenDword);
    GetComputerName(computerName, &computerNameLenDword);

    ULONGLONG ticks = GetTickCount64();

    ULONGLONG totalSeconds = ticks / 1000;

    ULONGLONG hours = totalSeconds / 3600;
    ULONGLONG minutes = (totalSeconds % 3600) / 60;
    ULONGLONG seconds = totalSeconds % 60;
    
    //MessageBox(NULL, L"Vnutri", L"Zaholovok", MB_ICONWARNING | MB_OK);

    std::wcout << L"its Windows " << computerName << L" user: " << userName << L" timeOfSystemWork " << 
        hours << L"h " << minutes << L"m " << seconds << L"s" << std::endl;
#endif


#ifdef _LINUX
    char computerNameBuffer[256];
    gethostname(computerNameBuffer, 256);

    const char* userName = getenv("USER");

    long totalSeconds = 0;
    std::ifstream uptimeFile("/proc/uptime");
    if (uptimeFile.is_open()) {
        uptimeFile >> totalSeconds;
        uptimeFile.close();
    }

    long hours = totalSeconds / 3600;
    long minutes = (totalSeconds % 3600) / 60;
    long seconds = totalSeconds % 60;

    std::cout << "its Linux " << computerNameBuffer << " user: " << (userName ? userName : "unknown") << " timeOfSystemWork: " 
        << hours << "h " << minutes << "m " << seconds << "s" << std::endl;
#endif
}
