#include <Windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <string>

const int ARRAY_SIZE = 10;
const int MIN_VALUE = 1;
const int MAX_VALUE = 100;

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcerr << L"Не передан дескриптор канала\n";
        return 1;
    }

    // Получаем дескриптор канала из аргументов
    HANDLE hWritePipe = (HANDLE)(INT_PTR)_wtoi(argv[1]);

    if (hWritePipe == NULL || hWritePipe == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Неверный дескриптор канала\n";
        return 1;
    }

    // Генерация случайных чисел
    std::vector<int> numbers(ARRAY_SIZE);
    
    std::wcout << L"Сгенерированные числа: ";
    
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        
        numbers[i] = rand() % 100;
        
        std::wcout << numbers[i] << L" ";
    }
    std::wcout << std::endl;

    // Запись размера массива и данных в канал
    DWORD bytesWritten;
    if (!WriteFile(hWritePipe, &ARRAY_SIZE, sizeof(ARRAY_SIZE), &bytesWritten, NULL)) {
        std::wcerr << L"Ошибка записи размера массива в канал\n";
        CloseHandle(hWritePipe);
        return 1;
    }

    if (!WriteFile(hWritePipe, numbers.data(), ARRAY_SIZE * sizeof(int), &bytesWritten, NULL)) {
        std::wcerr << L"Ошибка записи данных в канал\n";
        CloseHandle(hWritePipe);
        return 1;
    }

    std::wcout << L"Данные успешно записаны в канал\n";
    CloseHandle(hWritePipe);
    return 0;
}