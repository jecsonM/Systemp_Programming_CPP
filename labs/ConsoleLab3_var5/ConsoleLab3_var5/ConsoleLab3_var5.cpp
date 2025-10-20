#include <windows.h>
#include <iostream>

void PrintLastError(const std::string& context) {
    DWORD errorCode = GetLastError();
    if (errorCode == 0) {
        std::cout << context << ": No error code." << std::endl;
        return;
    }

    LPSTR errorMessage = nullptr;
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&errorMessage,
        0,
        NULL
    );

    if (size > 0) {
        std::cerr << context << " Error (" << errorCode << "): " << errorMessage << std::endl;
    }
    else {
        std::cerr << context << " Failed to get error message for code: " << errorCode << std::endl;
    }

    if (errorMessage != nullptr) {
        LocalFree(errorMessage);
    }
}



int main()
{

    size_t n = 2;
    HANDLE* hOriginalMutexes = new HANDLE[n];
    HANDLE* hOriginalEvents = new HANDLE[n];

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


         /*
         * HANDLE CreateEventA(
              [in, optional] LPSECURITY_ATTRIBUTES lpEventAttributes, - атрибуты безопасности
              [in]           BOOL                  bManualReset, если TRUE функция создает объект события ручного сброса
              если FALSE, то система автоматического сброса
              [in]           BOOL                  bInitialState, если TRUE то начальное состояние события, иначе не назначено
              [in, optional] LPCSTR                lpName - имя дескриптора
            );
         */
        hOriginalEvents[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
    }
    
    HANDLE* hDuplicatedMutexes = new HANDLE[n];
    HANDLE* hDuplicatedEvents = new HANDLE[n];

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
          [in]  BOOL     bInheritHandle, - указывает на наследуемость дескриптора - тру, если наследуем, фалс, если не наследуем
          [in]  DWORD    dwOptions необязательные действия
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

        dupSuccess = DuplicateHandle(
            currentProcess,
            hOriginalEvents[i],
            currentProcess,
            &hDuplicatedEvents[i],
            0, //0, потому что не нужны особые права
            FALSE,
            DUPLICATE_SAME_ACCESS
        );

        
        
    }
    for (size_t i = 0; i < n; i++)
    {
        BOOL result = SetEvent(hOriginalEvents[i]); //Вызываю события
    }

    /*
    * DWORD WaitForMultipleObjects(
          [in] DWORD        nCount,  - число дескрипторов
          [in] const HANDLE *lpHandles, - массив дескрипторов
          [in] BOOL         bWaitAll, - Если TRUE то всех, если FALSE, то хотя бы одного
          [in] DWORD        dwMilliseconds интервал ожидания в милисекундах
        );
    */



    DWORD res = WaitForMultipleObjects(n, hDuplicatedEvents, TRUE, 5000); //5сек

    if (res == WAIT_OBJECT_0)
    {
        std::wcout << L"Index of signaling obj" << res << std::endl;
    }

    if (res == WAIT_TIMEOUT)
    {
        std::wcout << L"Wait TimedOut" << std::endl;
    }
    if (res == WAIT_FAILED)
    {
        std::wcout << L"Wait Failed" << std::endl;
    }


    int successFullyClosedAmount = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (CloseHandle(hOriginalMutexes[i]))
            successFullyClosedAmount++;
        if (CloseHandle(hDuplicatedMutexes[i]))
            successFullyClosedAmount++;
        if (CloseHandle(hOriginalEvents[i]))
            successFullyClosedAmount++;
        if (CloseHandle(hDuplicatedEvents[i]))
            successFullyClosedAmount++;
    }

    if (successFullyClosedAmount == n * 4)
        std::wcout << L"SuccessfullyClosed" << std::endl;
    else
        std::wcout << L"UnsuccessfullyClosed" << std::endl;

    

}
