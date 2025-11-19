#include <Windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcerr << L"Не передан дескриптор канала\n";
        return 1;
    }

    // Получаем дескриптор канала из аргументов
    HANDLE hReadPipe = (HANDLE)(INT_PTR)_wtoi(argv[1]);

    if (hReadPipe == NULL || hReadPipe == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Неверный дескриптор канала\n";
        return 1;
    }

    // Чтение размера массива из канала
    int arraySize;
    DWORD bytesRead;
    if (!ReadFile(hReadPipe, &arraySize, sizeof(arraySize), &bytesRead, NULL) || bytesRead != sizeof(arraySize)) {
        std::wcerr << L"Ошибка чтения размера массива из канала\n";
        CloseHandle(hReadPipe);
        return 1;
    }

    // Чтение данных из канала
    std::vector<int> numbers(arraySize);
    if (!ReadFile(hReadPipe, numbers.data(), arraySize * sizeof(int), &bytesRead, NULL) ||
        bytesRead != arraySize * sizeof(int)) {
        std::wcerr << L"Ошибка чтения данных из канала\n";
        CloseHandle(hReadPipe);
        return 1;
    }

    std::wcout << L"Полученные числа: ";
    for (int num : numbers) {
        std::wcout << num << L" ";
    }
    std::wcout << std::endl;

    // Сортировка массива
    std::sort(numbers.begin(), numbers.end());

    // Запись в файл
    std::wofstream file(L"sorted_numbers.txt");
    if (file.is_open()) {
        for (int num : numbers) {
            file << num << L"\n";
        }
        file.close();
        std::wcout << L"Отсортированные числа записаны в файл sorted_numbers.txt\n";
    }
    else {
        std::wcerr << L"Ошибка создания файла sorted_numbers.txt\n";
        CloseHandle(hReadPipe);
        return 1;
    }

    CloseHandle(hReadPipe);
    return 0;
}