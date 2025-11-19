#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

// Задача 1
HANDLE m1, m2;

DWORD WINAPI T1(LPVOID) {
    cout << "T1: беру mutex1\n";
    WaitForSingleObject(m1, INFINITE);
    Sleep(1000);

    cout << "T1: жду mutex2\n";
    if (WaitForSingleObject(m2, 5000) == WAIT_TIMEOUT) {
        cout << "T1: deadlock! откат\n";
        ReleaseMutex(m1);
        return 0;
    }

    cout << "T1: работаю\n";
    Sleep(1000);
    ReleaseMutex(m2);
    ReleaseMutex(m1);
    cout << "T1: готово\n";
    return 0;
}

DWORD WINAPI T2(LPVOID) {
    cout << "T2: беру mutex2\n";
    WaitForSingleObject(m2, INFINITE);
    Sleep(1000);

    cout << "T2: жду mutex1\n";
    if (WaitForSingleObject(m1, 5000) == WAIT_TIMEOUT) {
        cout << "T2: deadlock! откат\n";
        ReleaseMutex(m2);
        return 0;
    }

    cout << "T2: работаю\n";
    Sleep(1000);
    ReleaseMutex(m1);
    ReleaseMutex(m2);
    cout << "T2: готово\n";
    return 0;
}

void Task1() {
    cout << "\nЗадача 1\n";
    m1 = CreateMutex(NULL, FALSE, NULL);
    m2 = CreateMutex(NULL, FALSE, NULL);

    CreateThread(NULL, 0, T1, NULL, 0, NULL);
    Sleep(200);
    CreateThread(NULL, 0, T2, NULL, 0, NULL);

    Sleep(8000);  
    CloseHandle(m1); CloseHandle(m2);
}

// Задача 2
HANDLE logMutex;
void WriteLog(string s) {
    WaitForSingleObject(logMutex, INFINITE);
    cout << s << endl;
    HANDLE f = CreateFile(L"log.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(f, 0, NULL, FILE_END);
    s += "\r\n";
    DWORD w;
    WriteFile(f, s.c_str(), s.size(), &w, NULL);
    CloseHandle(f);
    ReleaseMutex(logMutex);
}

DWORD WINAPI Writer(LPVOID n) {
    for (int i = 1; i <= 5; i++) {
        WriteLog("поток " + to_string((LONG)n) + " запись " + to_string(i));
        Sleep(100 + rand() % 200);
    }
    return 0;
}

void Task2() {
    cout << "\nЗадача 2\n";
    logMutex = CreateMutex(NULL, FALSE, NULL);
    for (int i = 1; i <= 5; i++)
        CreateThread(NULL, 0, Writer, (LPVOID)i, 0, NULL);
    Sleep(6000); 
    CloseHandle(logMutex);
}

// Задача 3
HANDLE dbMutex, wrtMutex;
LONG readers = 0;

void StartRead() {
    WaitForSingleObject(wrtMutex, INFINITE);
    WaitForSingleObject(dbMutex, INFINITE);
    readers++;
    ReleaseMutex(dbMutex);
    ReleaseMutex(wrtMutex);
}

void EndRead() {
    WaitForSingleObject(dbMutex, INFINITE);
    readers--;
    if (readers == 0) ReleaseMutex(wrtMutex);
    ReleaseMutex(dbMutex);
}

void StartWrite() {
    WaitForSingleObject(wrtMutex, INFINITE);
}

void EndWrite() {
    ReleaseMutex(wrtMutex);
}

DWORD WINAPI Reader(LPVOID param) {
    LONG id = (LONG)param;
    for (int i = 0; i < 4; i++) {
        Sleep(rand() % 800);
        StartRead();
        cout << "читатель " << id << " читает\n";
        EndRead();
    }
    return 0;
}

DWORD WINAPI Writer3(LPVOID param) {
    LONG id = (LONG)param;
    for (int i = 0; i < 3; i++) {
        Sleep(rand() % 1200 + 300);
        StartWrite();
        cout << "писатель " << id << " пишет\n";

        HANDLE f = CreateFile(L"db.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(f, 0, NULL, FILE_END);
        string s = "writer " + to_string(id) + " " + to_string(GetTickCount()) + "\r\n";
        DWORD wr;
        WriteFile(f, s.c_str(), s.size(), &wr, NULL);
        CloseHandle(f);

        EndWrite();
    }
    return 0;
}

void Task3() {
    cout << "\nЗадача 3\n";
    srand(GetTickCount());
    dbMutex = CreateMutex(NULL, FALSE, NULL);
    wrtMutex = CreateMutex(NULL, FALSE, NULL);
    CloseHandle(CreateFile(L"db.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

    for (int i = 1; i <= 6; i++) CreateThread(NULL, 0, Reader, (LPVOID)i, 0, NULL);
    for (int i = 1; i <= 3; i++) CreateThread(NULL, 0, Writer3, (LPVOID)i, 0, NULL);

    Sleep(15000);
    CloseHandle(dbMutex);
    CloseHandle(wrtMutex);
}


int main() {
    setlocale(0, "ru");

    Task1();
    Task2();
    Task3();

    return 0;
}