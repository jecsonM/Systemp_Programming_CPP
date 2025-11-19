#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

const int ARRAY_SIZE = 10;


int wmain() {
    
    std::wcout << L"Главный процесс запущен\n";

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES saAttr;

    // Настройка безопасности для наследуемого канала
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Создание анонимного канала
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        std::wcerr << L"Ошибка создания канала\n";
        return 1;
    }

    // Первый дочерний процесс - генерация случайных чисел
    STARTUPINFOW si1 = { sizeof(si1) };
    PROCESS_INFORMATION pi1;

    // Подготовка командной строки с передачей дескриптора
    wchar_t cmdLine1[256];
    swprintf_s(cmdLine1, L"child1.exe %d", (int)(INT_PTR)hWritePipe);

    if (CreateProcessW(NULL, cmdLine1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        std::wcout << L"Запущен первый процесс генерации случайных чисел\n";
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
    }
    else {
        std::wcerr << L"Ошибка создания первого процесса\n";
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return 1;
    }

    // Закрываем дескриптор записи в главном процессе
    CloseHandle(hWritePipe);

    // Второй дочерний процесс - сортировка и запись в файл
    STARTUPINFOW si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2;

    // Подготовка командной строки с передачей дескриптора
    wchar_t cmdLine2[256];
    swprintf_s(cmdLine2, L"child2.exe %d", (int)(INT_PTR)hReadPipe);

    if (CreateProcessW(NULL, cmdLine2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        std::wcout << L"Запущен второй процесс сортировки и записи в файл\n";
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
    }
    else {
        std::wcerr << L"Ошибка создания второго процесса\n";
        CloseHandle(hReadPipe);
        return 1;
    }

    // Закрываем дескриптор чтения в главном процессе
    CloseHandle(hReadPipe);

    // Ждем завершения дочерних процессов
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    std::wcout << L"Главный процесс завершен. Файл sorted_numbers.txt создан.\n";
    return 0;
}