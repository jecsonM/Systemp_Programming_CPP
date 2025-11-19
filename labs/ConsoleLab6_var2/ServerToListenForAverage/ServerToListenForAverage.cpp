#include <Windows.h>
#include <iostream>
#include <random>

int main() {
    setlocale(LC_ALL, "Russian");
    std::wcout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::wcout << L"Запущен первый процесс генерации случайных чисел" << std::endl;

    // Создаем именованный канал с двунаправленным доступом
    HANDLE hPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\AveragePipe",
        PIPE_ACCESS_DUPLEX,  // Двунаправленный доступ
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        1024,
        1024,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::wcout << L"Ошибка создания канала: " << GetLastError() << std::endl;
        return 1;
    }

    std::wcout << L"Ожидание подключения клиента..." << std::endl;

    // Ждем подключения клиента
    if (!ConnectNamedPipe(hPipe, NULL)) {
        std::wcout << L"Ошибка подключения: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    std::wcout << L"Клиент подключен" << std::endl;

    // Генерируем 3 случайных числа
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    int numbers[3];
    for (int i = 0; i < 3; ++i) {
        numbers[i] = dist(gen);
    }

    std::wcout << L"Сгенерированные числа: "
        << numbers[0] << L", " << numbers[1] << L", " << numbers[2] << std::endl;

    // Отправляем числа клиенту
    DWORD bytesWritten;
    if (!WriteFile(hPipe, numbers, sizeof(numbers), &bytesWritten, NULL)) {
        std::wcout << L"Ошибка отправки данных: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    std::wcout << L"Числа отправлены клиенту" << std::endl;

    // Ждем ответ от клиента
    double average;
    DWORD bytesRead;
    if (ReadFile(hPipe, &average, sizeof(average), &bytesRead, NULL)) {
        std::wcout << L"Получено среднее арифметическое: " << average << std::endl;
    }
    else {
        std::wcout << L"Ошибка получения ответа: " << GetLastError() << std::endl;
    }

    // Закрываем канал
    CloseHandle(hPipe);
    std::wcout << L"Сервер завершает работу" << std::endl;

    return 0;
}