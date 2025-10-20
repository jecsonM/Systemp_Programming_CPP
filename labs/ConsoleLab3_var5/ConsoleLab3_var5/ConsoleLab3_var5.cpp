#include <windows.h>
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";

    size_t n = 10;
    HANDLE* hOriginalMutexes = new HANDLE[n];

    for (size_t i = 0; i < n; i++)
    {
        /*
        HANDLE CreateMutexA(
          [in, optional] LPSECURITY_ATTRIBUTES lpMutexAttributes, - атрибуты безопасности
          [in]           BOOL                  bInitialOwner, - для наследования
          [in, optional] LPCSTR                lpName - имя дескриптора
        );
        */
        hOriginalMutexes[i] = CreateMutex(NULL, FALSE, NULL);
    }
    
    HANDLE* hDuplicatedMutexes = new HANDLE[n];
    HANDLE currentProcess = GetCurrentProcess();

    for (size_t i = 0; i < n; i++)
    {
        /*
        BOOL DuplicateHandle(
          [in]  HANDLE   hSourceProcessHandle, - Дескриптор процесса с дескриптором, который будет дублироваться
          [in]  HANDLE   hSourceHandle, - Дескриптор, который необходимо дублировать
          [in]  HANDLE   hTargetProcessHandle,- Дескриптор процесса куда будет сдублирован копируемый дескриптор
          [out] LPHANDLE lpTargetHandle, - Дублированный дескриптор
          [in]  DWORD    dwDesiredAccess, - Доступ запрашивается для нового дескриптора
          [in]  BOOL     bInheritHandle,
          [in]  DWORD    dwOptions
        );
        */
        BOOL dupSuccess = DuplicateHandle(
            currentProcess,
            hOriginalMutexes[i], 
            currentProcess,
            &hDuplicatedMutexes[i],
            0, //0, потому что не нужны особые права
            FALSE,               
            DUPLICATE_SAME_ACCESS
        );

    }
    
}
