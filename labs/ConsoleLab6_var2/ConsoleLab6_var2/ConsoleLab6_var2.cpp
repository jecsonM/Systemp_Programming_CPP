#include <windows.h>
#include <iostream>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Инициализация структур нулевыми значениями
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Командная строка для запуска (в данном случае, "notepad.exe")
    // Если путь содержит пробелы, его следует заключить в кавычки.
    // Для универсальности можно использовать полное имя пути, например L"C:\\Windows\\System32\\notepad.exe"
    LPWSTR szCmdline = (LPWSTR)L"cmd.exe";

    // Запуск дочернего процесса
    if (!CreateProcess(
        NULL,           // Имя исполняемого модуля (NULL, если указано в командной строке)
        szCmdline,      // Командная строка
        NULL,           // Атрибуты безопасности процесса по умолчанию
        NULL,           // Атрибуты безопасности потока по умолчанию
        FALSE,          // Дескрипторы не наследуются
        0,              // Флаги создания (например, CREATE_NEW_CONSOLE для нового окна консоли)
        NULL,           // Использование существующего блока переменных среды
        NULL,           // Использование существующего рабочего каталога
        &si,            // Указатель на структуру STARTUPINFO
        &pi)            // Указатель на структуру PROCESS_INFORMATION
        )
    {
        std::wcout << stderr << L"CreateProcess failed (" << GetLastError() << ").\n";
    }
    

}