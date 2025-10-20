#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string.h>
#include <windows.h>
#include <wchar.h>

// Замена для обычных строк (char*)
bool strreplace(char* str, const char* oldSubstr, const char* newSubstr, DWORD bufferSize)
{
    if (!str || !oldSubstr || !newSubstr || bufferSize == 0) 
    {
        return false;
    }

    size_t oldLen = strlen(oldSubstr);
    size_t newLen = strlen(newSubstr);
    if (oldLen == 0) {
        return false;
    }

    std::vector<char> result;
    char* currentPos = str;
    char* foundPos;

    while ((foundPos = strstr(currentPos, oldSubstr)) != nullptr) 
    {
        result.insert(result.end(), currentPos, foundPos);
        result.insert(result.end(), newSubstr, newSubstr + newLen);
        currentPos = foundPos + oldLen;
    }
    result.insert(result.end(), currentPos, currentPos + strlen(currentPos) + 1);

    if (result.size() > bufferSize) 
    {
        return false;
    }

    strcpy(str, result.data());
    return true;
}

// Замена для Unicode-строк (wchar_t*)
bool strreplace(wchar_t* str, const wchar_t* oldSubstr, const wchar_t* newSubstr, DWORD bufferSize)
{
    if (!str || !oldSubstr || !newSubstr || bufferSize == 0) 
    {
        return false;
    }

    size_t oldLen = wcslen(oldSubstr);
    size_t newLen = wcslen(newSubstr);
    if (oldLen == 0) {
        return false;
    }

    std::vector<wchar_t> result;
    wchar_t* currentPos = str;
    wchar_t* foundPos;

    while ((foundPos = wcsstr(currentPos, oldSubstr)) != nullptr) 
    {
        result.insert(result.end(), currentPos, foundPos);
        result.insert(result.end(), newSubstr, newSubstr + newLen);
        currentPos = foundPos + oldLen;
    }
    
    result.insert(result.end(), currentPos, currentPos + wcslen(currentPos) + 1);

    if (result.size() > bufferSize / sizeof(wchar_t)) {
        return false;
    }

    wcscpy(str, result.data());
    return true;
}





//parse csv для обычных строк (*char)
bool parsecsventry(const char* csvLine, int fieldIndex, char* outputBuffer, DWORD bufferSize) {
    if (!csvLine || fieldIndex < 0 || !outputBuffer || bufferSize == 0) {
        return false;
    }

    size_t len = strlen(csvLine);
    size_t currentPos = 0;
    int currentField = 0;

    while (currentPos < len && currentField <= fieldIndex) {
        size_t startPos = currentPos;
        bool inQuote = false;

        if (csvLine[currentPos] == '"') {
            inQuote = true;
            startPos++;
            currentPos++;
        }

        while (currentPos < len) {
            if (inQuote) {
                if (csvLine[currentPos] == '"') {
                    if (currentPos + 1 < len && csvLine[currentPos + 1] == '"') {
                        currentPos++;
                    }
                    else {
                        inQuote = false;
                    }
                }
            }
            else if (csvLine[currentPos] == ',') {
                break;
            }
            currentPos++;
        }

        if (currentField == fieldIndex) {
            size_t copyLen = currentPos - startPos;
            if (inQuote && csvLine[currentPos - 1] == '"') {
                copyLen--;
            }

            if (copyLen + 1 > bufferSize) {
                return FALSE;
            }
            
            strncpy(outputBuffer, csvLine + startPos, copyLen);
            outputBuffer[copyLen] = '\0';
            return true;
        }

        currentField++;
        if (currentPos < len && csvLine[currentPos] == ',') {
            currentPos++;
        }
    }
    return false;
}

// parse csv для Unicode-строк (wchar_t*)
bool parsecsventry(const wchar_t* csvLine, int fieldIndex, wchar_t* outputBuffer, DWORD bufferSize) {
    if (!csvLine || fieldIndex < 0 || !outputBuffer || bufferSize == 0) {
        return false;
    }

    size_t len = wcslen(csvLine);
    size_t currentPos = 0;
    int currentField = 0;

    while (currentPos < len && currentField <= fieldIndex) {
        size_t startPos = currentPos;
        bool inQuote = false;

        if (csvLine[currentPos] == L'"') {
            inQuote = true;
            startPos++;
            currentPos++;
        }

        while (currentPos < len) {
            if (inQuote) {
                if (csvLine[currentPos] == L'"') {
                    if (currentPos + 1 < len && csvLine[currentPos + 1] == L'"') {
                        currentPos++;
                    }
                    else {
                        inQuote = false;
                    }
                }
            }
            else if (csvLine[currentPos] == L',') {
                break;
            }
            currentPos++;
        }

        if (currentField == fieldIndex) {
            size_t copyLen = currentPos - startPos;
            if (inQuote && csvLine[currentPos - 1] == L'"') {
                copyLen--;
            }

            if ((copyLen + 1) * sizeof(wchar_t) > bufferSize) {
                return false;
            }

            wcsncpy(outputBuffer, csvLine + startPos, copyLen);
            outputBuffer[copyLen] = L'\0';
            return true;
        }

        currentField++;
        if (currentPos < len && csvLine[currentPos] == L',') {
            currentPos++;
        }
    }
    return false;
}









// Получить кодировку для обычных строк (char*)
int getstringencoding(const char* str, int length) {
    if (!str || length <= 0) {
        return 0;
    }

    if (
        length >= 3 && (unsigned char)str[0] == 0xEF && 
        (unsigned char)str[1] == 0xBB && 
        (unsigned char)str[2] == 0xBF
        ) 
    {
        return 3; // UTF-8 BOM
    }

    bool is_utf8 = true;
    if (is_utf8) 
    {
        return 3;
    }

    int i;
    bool has_high_bytes = false;
    for (i = 0; i < length; ++i) {
        if ((unsigned char)str[i] > 127) {
            has_high_bytes = true;
        }
    }

    if (!has_high_bytes) 
    {
        return 1; // ANSI (ASCII)
    }

    return 0; // не определено
}

// Получить кодировку для Unicode-строк (wchar_t*)
int getstringencoding(const wchar_t* str, int length) {
    if (!str || length <= 0) {
        return 0;
    }

    if (length >= 1 && str[0] == 0xFEFF) {
        return 2; // UTF-16LE BOM
    }

    
    bool is_utf16 = false;
    for (int i = 0; i < length; ++i) {
        if (str[i] > 0xFF) // если содержит 2-байтные символы
        {
            is_utf16 = true;
            break;
        }
    }
    if (is_utf16) {
        return 2; // UTF-16LE
    }

    return 0; // не определено
}







int main()
{
    
    /*Перевод из ANSII в Wide*/
    const std::wstring wstr = L"Hello";
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1 /*null-terminated*/, nullptr, 0 /*размер буфера, если 0, то расчитать буфер*/, nullptr, nullptr);
    std::vector<char> buffer(bufferSize);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);
    std::string convertedStr = std::string(buffer.data());
    std::cout << convertedStr << std::endl;
    
    /*Сообщение об ошибке*/
    /*SetLastError(ERROR_ABIOS_ERROR);
    DWORD errorCode = GetLastError();
    if (errorCode == 0) {
        std::wcout << L"Ошибок нет.\n";
    }

    LPWSTR lpMsgBuf = nullptr;
    DWORD dwChars = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&lpMsgBuf),
        0,
        NULL);

    std::wstring errorMessage;
    errorMessage = std::wstring(lpMsgBuf);
    std::wcout << errorMessage;*/
    
    

    std::string initialStrANSII = "My string super strength ANSII";
    std::string oldDubStrANSII = "str";
    std::string newSubStrANSII = "Power";
    int bufferSizeForReplaceANSII = 256;

    char* finalStrANSII = new char[initialStrANSII.length() + 1];
    strcpy(finalStrANSII, initialStrANSII.c_str());

    strreplace(finalStrANSII, oldDubStrANSII.c_str(), newSubStrANSII.c_str(), bufferSizeForReplaceANSII);




    std::wstring initialStrWIDE = L"My string super strength WIDE";
    std::wstring oldDubStrWIDE = L"str";
    std::wstring newSubStrWIDE = L"Power";
    int bufferSizeForReplaceWIDE = 256;

    wchar_t* finalStrWIDE = new wchar_t[initialStrWIDE.length() + 1];
    wcscpy(finalStrWIDE, initialStrWIDE.c_str());

    strreplace(finalStrWIDE, oldDubStrWIDE.c_str(), newSubStrWIDE.c_str(), bufferSizeForReplaceWIDE);

    std::cout << finalStrANSII << "\n";
    std::wcout << finalStrWIDE << L"\n";
    




    std::string StrToParseANSII = "zerouth,first,second,third,fourth";
    int bufferSizeForParseANSII = 64;
    char *parsedStrANSII = new char[bufferSizeForParseANSII];
    parsecsventry(StrToParseANSII.c_str(), 3, parsedStrANSII, bufferSizeForParseANSII);
    std::cout << parsedStrANSII << "\n";
    

    std::wstring StrToParseWIDE = L"zerouth,first,second,third,fourth";
    int bufferSizeForParseWIDE = 64;
    wchar_t* parsedStrWIDE = new wchar_t[bufferSizeForParseWIDE];
    parsecsventry(StrToParseWIDE.c_str(), 3, parsedStrWIDE, bufferSizeForParseWIDE);
    std::wcout << parsedStrWIDE << L"\n";

    std::string strToGetEncodingFrom = u8"Here is the string";

    

    int encoding = getstringencoding(strToGetEncodingFrom.c_str(), strToGetEncodingFrom.length());
    std::cout << strToGetEncodingFrom << " in encoding " << encoding << " what means utf8\n";

}
