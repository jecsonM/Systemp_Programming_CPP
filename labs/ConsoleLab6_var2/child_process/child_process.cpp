#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

// Функция для конвертации времени в wide string
std::wstring GetCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    wchar_t timeStr[100];
    _wctime_s(timeStr, sizeof(timeStr) / sizeof(wchar_t), &time);

    return std::wstring(timeStr);
}

// Ваша функция main2 - основная логика дочернего процесса
int main2() {
    int counter = 0;
    std::wstring input;

    // Приветственное сообщение
    std::wcout << L"=== Child Process Started ===" << std::endl;
    std::wcout << L"Available commands: count, time, message <text>, exit, help" << std::endl;
    std::wcout << L"Ready for commands..." << std::endl;
    std::wcout.flush();

    while (true) {
        // Читаем команду из stdin (из pipe)
        if (!std::getline(std::wcin, input)) {
            // Если чтение не удалось, возможно родительский процесс закрыл pipe
            break;
        }

        if (input == L"exit") {
            std::wcout << L"Child process shutting down..." << std::endl;
            std::wcout.flush();
            break;
        }
        else if (input == L"count") {
            counter++;
            std::wcout << L"Counter incremented to: " << counter << std::endl;
        }
        else if (input == L"time") {
            std::wcout << L"Current time: " << GetCurrentTimeString();
        }
        else if (input.find(L"message") == 0) {
            if (input.length() > 8) {
                std::wstring message = input.substr(8);
                std::wcout << L"Message received: '" << message << L"'" << std::endl;
            }
            else {
                std::wcout << L"Please provide a message after 'message' command" << std::endl;
            }
        }
        else if (input == L"help") {
            std::wcout << L"Available commands:" << std::endl;
            std::wcout << L"  count    - Increment counter" << std::endl;
            std::wcout << L"  time     - Show current time" << std::endl;
            std::wcout << L"  message <text> - Echo message" << std::endl;
            std::wcout << L"  exit     - Exit process" << std::endl;
            std::wcout << L"  help     - Show this help" << std::endl;
        }
        else if (!input.empty()) {
            std::wcout << L"Unknown command: '" << input << L"'" << std::endl;
            std::wcout << L"Type 'help' for available commands" << std::endl;
        }

        // Флушим вывод чтобы он сразу отправлялся в pipe
        std::wcout.flush();
    }

    return 0;
}

int main() {
    // Дочерний процесс использует стандартные handles для общения с родителем
    return main2();
}