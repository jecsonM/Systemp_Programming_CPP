#include <Windows.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "Russian");
    std::wcout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::wcout << L"Запущен второй процесс вычисления среднего арифметического" << std::endl;

    // Пытаемся подключиться несколько раз с задержкой
    HANDLE hPipe;
    int attempts = 0;

    while (attempts < 10) {
        hPipe = CreateFile(
            L"\\\\.\\pipe\\AveragePipe",
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE) {
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            std::wcout << L"Ошибка подключения к каналу: " << GetLastError() << std::endl;
            return 1;
        }

        // Ждем перед следующей попыткой
        Sleep(1000);
        attempts++;
    }

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::wcout << L"Не удалось подключиться к серверу после " << attempts << L" попыток" << std::endl;
        return 1;
    }

    // Устанавливаем режим чтения сообщений
    DWORD mode = PIPE_READMODE_MESSAGE;
    if (!SetNamedPipeHandleState(hPipe, &mode, NULL, NULL)) {
        std::wcout << L"Ошибка установки режима канала: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    std::wcout << L"Подключено к серверу" << std::endl;

    // Получаем числа от сервера
    int numbers[3];
    DWORD bytesRead;
    if (!ReadFile(hPipe, numbers, sizeof(numbers), &bytesRead, NULL)) {
        std::wcout << L"Ошибка чтения данных: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    std::wcout << L"Полученные числа: "
        << numbers[0] << L", " << numbers[1] << L", " << numbers[2] << std::endl;

    // Вычисляем среднее арифметическое
    double average = (numbers[0] + numbers[1] + numbers[2]) / 3.0;
    std::wcout << L"Вычисленное среднее: " << average << std::endl;

    // Отправляем результат обратно серверу
    DWORD bytesWritten;
    if (!WriteFile(hPipe, &average, sizeof(average), &bytesWritten, NULL)) {
        std::wcout << L"Ошибка отправки результата: " << GetLastError() << std::endl;
    }
    else {
        std::wcout << L"Результат отправлен серверу" << std::endl;
    }

    CloseHandle(hPipe);
    std::wcout << L"Клиент завершает работу" << std::endl;
    return 0;
}