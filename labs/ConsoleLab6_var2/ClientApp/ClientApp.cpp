#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>



int main() {
    setlocale(LC_ALL, "Russian");
    std::wcout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::wcout << L"Клиент запущен. PID: " << GetCurrentProcessId() << std::endl;
    std::wcout << L"Подключение к серверу..." << std::endl;



    while (true) {
        if (WaitNamedPipeW(L"\\\\.\\pipe\\BroadcastPipe", NMPWAIT_WAIT_FOREVER)) {
            HANDLE hPipe = CreateFileW(
                L"\\\\.\\pipe\\BroadcastPipe",
                GENERIC_READ,
                0, NULL, OPEN_EXISTING, 0, NULL);

            if (hPipe != INVALID_HANDLE_VALUE) {
                std::wcout << L"Успешное подключение к серверу" << std::endl;

                wchar_t buffer[256];
                DWORD bytesRead;

                while (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
                    if (bytesRead > 0) {
                        std::wcout << L"PID " << GetCurrentProcessId() << L": " << buffer << std::endl;
                    }
                }

                CloseHandle(hPipe);
                std::wcout << L"Соединение разорвано. Переподключение..." << std::endl;
            }
        }

        Sleep(1000);
    }

    return 0;
}