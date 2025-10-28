#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <iomanip>
#undef max(a, b)

struct ProcessHandleInfo {
    DWORD pid;
    HANDLE handle;
    std::wstring name;
};

std::vector<ProcessHandleInfo> getAllProcessHandles() {
    std::vector<ProcessHandleInfo> processes;

    // Создаем снимок всех процессов
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Error of creating snaphots" << std::endl;
        return processes;
    }

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    // Получаем первый процесс
    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            ProcessHandleInfo info;
            info.pid = processEntry.th32ProcessID;
            info.name = processEntry.szExeFile;

            // Открываем дескриптор процесса с минимальными правами
            info.handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ,
                FALSE, processEntry.th32ProcessID);

            if (info.handle != NULL) {
                processes.push_back(info);
            }

        } while (Process32NextW(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return processes;
}

// Функция для получения подробной информации о процессе
void showDetailedInfo(const ProcessHandleInfo& process) {
    std::wcout << L"\n=== Detailed Information ===" << std::endl;
    std::wcout << L"Process Name: " << process.name << std::endl;
    std::wcout << L"PID: " << process.pid << std::endl;
    std::wcout << L"Handle: " << process.handle << std::endl;

    // Получаем информацию о памяти
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(process.handle, &pmc, sizeof(pmc))) {
        std::wcout << L"Working Set Size: " << pmc.WorkingSetSize / 1024 << L" KB" << std::endl;
        std::wcout << L"Peak Working Set Size: " << pmc.PeakWorkingSetSize / 1024 << L" KB" << std::endl;
        std::wcout << L"Pagefile Usage: " << pmc.PagefileUsage / 1024 << L" KB" << std::endl;
    }
    else {
        std::wcout << L"Memory information: Access denied" << std::endl;
    }

    // Получаем время создания процесса
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(process.handle, &createTime, &exitTime, &kernelTime, &userTime)) {
        SYSTEMTIME sysTime;
        FileTimeToSystemTime(&createTime, &sysTime);
        std::wcout << L"Creation Time: "
            << std::setw(2) << std::setfill(L'0') << sysTime.wHour << L":"
            << std::setw(2) << std::setfill(L'0') << sysTime.wMinute << L":"
            << std::setw(2) << std::setfill(L'0') << sysTime.wSecond << std::endl;
    }

    // Получаем путь к исполняемому файлу
    WCHAR processPath[MAX_PATH];
    DWORD pathSize = MAX_PATH;
    if (QueryFullProcessImageNameW(process.handle, 0, processPath, &pathSize)) {
        std::wcout << L"Full Path: " << processPath << std::endl;
    }
    else {
        std::wcout << L"Full Path: Access denied" << std::endl;
    }

    // Получаем приоритет процесса
    DWORD priority = GetPriorityClass(process.handle);
    if (priority) {
        std::wcout << L"Priority Class: ";
        switch (priority) {
        case IDLE_PRIORITY_CLASS: std::wcout << L"Idle"; break;
        case BELOW_NORMAL_PRIORITY_CLASS: std::wcout << L"Below Normal"; break;
        case NORMAL_PRIORITY_CLASS: std::wcout << L"Normal"; break;
        case ABOVE_NORMAL_PRIORITY_CLASS: std::wcout << L"Above Normal"; break;
        case HIGH_PRIORITY_CLASS: std::wcout << L"High"; break;
        case REALTIME_PRIORITY_CLASS: std::wcout << L"Realtime"; break;
        default: std::wcout << L"Unknown"; break;
        }
        std::wcout << std::endl;
    }

    std::wcout << L"============================\n" << std::endl;
}

// Функция для завершения процесса
bool killProcess(const ProcessHandleInfo& process) {
    // Пытаемся закрыть процесс "красиво"
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, process.pid);
    if (hProcess == NULL) {
        std::wcout << L"Failed to open process for termination. Error: " << GetLastError() << std::endl;
        return false;
    }

    if (TerminateProcess(hProcess, 0)) {
        std::wcout << L"Process " << process.name << L" (PID: " << process.pid << L") successfully terminated!" << std::endl;
        CloseHandle(hProcess);
        return true;
    }
    else {
        DWORD error = GetLastError();
        std::wcout << L"Failed to terminate process. Error: " << error << std::endl;

        if (error == ERROR_ACCESS_DENIED) {
            std::wcout << L"Access denied. Try running as Administrator." << std::endl;
        }
        CloseHandle(hProcess);
        return false;
    }
}

int main()
{
    auto processes = getAllProcessHandles();
    DWORD ProcAmount = processes.size();

    for (size_t i = 0; i < ProcAmount; i++) {
        std::wcout << L"[" << i << L"] " << processes[i].name << std::endl;
    }

    std::wcout << std::endl
        << L"Actions: 1 - more info, 2 - kill process" << std::endl
        << L"Input id of process and action: id 1/2" << std::endl;

    int procID, action;

    std::wcout << L"Input process ID and action (space between): ";
    std::cin >> procID >> action;

    if (std::cin.fail() || (action != 1 && action != 2) || procID < 0 || procID >= ProcAmount) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::wcout << L"Error! Input valid process ID (0-" << ProcAmount - 1 << L") and action (1 or 2).\n";
    }
    else
    {
        switch (action)
        {
        case 1:
            showDetailedInfo(processes[procID]);
            break;
        case 2:
            std::wcout << L"Are you sure you want to kill process '"
                << processes[procID].name << L"' (PID: " << processes[procID].pid << L")? (y/n): ";

            wchar_t confirmation;
            std::wcin >> confirmation;

            if (confirmation == L'y' || confirmation == L'Y') {
                killProcess(processes[procID]);
            }
            else {
                std::wcout << L"Process termination cancelled." << std::endl;
            }
            break;
        }
    }

    
    for (size_t i = 0; i < ProcAmount; i++) {
        if (processes[i].handle != NULL) {
            CloseHandle(processes[i].handle);
        }
    }

    

    return 0;
}