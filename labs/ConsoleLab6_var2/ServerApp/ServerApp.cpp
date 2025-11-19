#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

int sendTimeToAll(std::vector<HANDLE> &clientPipes)
{
    while (true) {
        DWORD currentTime = GetTickCount();



        SYSTEMTIME st;
        GetLocalTime(&st);
        wchar_t message[256];
        swprintf_s(message, L"Время: %02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

        std::wcout << L"Отправка сообщения: " << message << L" клиентам: " << clientPipes.size() << std::endl;

        // Отправляем сообщение всем подключенным клиентам
        for (auto it = clientPipes.begin(); it != clientPipes.end(); ) {
            DWORD bytesWritten;
            BOOL success = WriteFile(*it, message, (wcslen(message) + 1) * sizeof(wchar_t),
                &bytesWritten, NULL);

            if (!success) {
                std::wcout << L"Клиент отключен" << std::endl;
                CloseHandle(*it);
                it = clientPipes.erase(it);
            }
            else {
                ++it;
            }
        }


        Sleep(3000);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::wcout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::wcout << L"Сервер запущен. Ожидание клиентов..." << std::endl;

    std::vector<HANDLE> clientPipes;
    std::vector<std::thread> threads;

    // Создаем канал для подключения клиентов
    HANDLE hListenPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\BroadcastPipe",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        512, 512, 0, NULL);

    if (hListenPipe == INVALID_HANDLE_VALUE) {
        std::wcout << L"Ошибка создания канала: " << GetLastError() << std::endl;
        return 1;
    }

    
    threads.emplace_back(sendTimeToAll, ref(clientPipes));

    while (true) {
        // Проверяем новые подключения без блокировки
        BOOL connected = ConnectNamedPipe(hListenPipe, NULL);
        if (connected || GetLastError() == ERROR_PIPE_CONNECTED) {
            std::wcout << L"Новый клиент подключен. Всего клиентов: " << clientPipes.size() + 1 << std::endl;
            clientPipes.push_back(hListenPipe);

            // Создаем новый канал для следующих подключений
            hListenPipe = CreateNamedPipeW(
                L"\\\\.\\pipe\\BroadcastPipe",
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                512, 512, 0, NULL);

            if (hListenPipe == INVALID_HANDLE_VALUE) {
                std::wcout << L"Ошибка создания канала: " << GetLastError() << std::endl;
                break;
            }
        }

        

        Sleep(100); // Короткая пауза чтобы не грузить процессор
    }

    threads[0].join();
    // Закрываем все каналы
    for (HANDLE hPipe : clientPipes) {
        CloseHandle(hPipe);
    }
    CloseHandle(hListenPipe);

    return 0;
}