#include "MyString.h"
#include <iostream>
#include <vector>
#include <cwchar>
#include <fstream>
#define _CRT_SECURE_NO_WARNINGS


MyString::MyString()
{
    std::wcout << L"Used constructor without params\n";
    str = nullptr;
}

MyString::MyString(const wchar_t* s)
{
    std::wcout << L"Used constructor with parameter\n";
    str = new wchar_t[wcslen(s) + 1];
    wcscpy(str, s);
}

MyString::MyString(const MyString& other)
{
    std::wcout << L"Used constructor of copying\n";
    if (other.str) {
        str = new wchar_t[wcslen(other.str) + 1];
        wcscpy(str, other.str);
    }
    else {
        str = nullptr;
    }

}

MyString::~MyString()
{
    std::wcout << L"Used destructor\n";
    delete[] str;
}

void MyString::set()
{
    std::wcout << L"Input new string: ";
    wchar_t buffer[256];
    std::wcin.ignore(1000, '\n');
    std::wcin.getline(buffer, 256);
    delete[] str;
    str = new wchar_t[wcslen(buffer) + 1];
    wcscpy(str, buffer);
    std::wcout << L"New string set\n";

}

void MyString::update()
{
    if (!str) 
    {
        std::wcout << L"Empty string!\n";
        return;
    }

    int len = wcslen(str);
    std::wcout << L"Str length: " << len << std::endl;

    if (len <= 6)
    {
        std::wcout << L"Str length <= 6 no changes";
        return;
    }

    
    std::vector<wchar_t> newStrVec;

    wchar_t* ptr = str;
    bool insideBrackets = false;
    std::vector<wchar_t> tempBuffer;

    while (*ptr != L'\0') 
    {
        if (*ptr == L'{') 
        {
            insideBrackets = true;
            tempBuffer.clear();
        }
        else if (*ptr == L'}') 
        {
            if (insideBrackets) 
            {
                for (wchar_t c : tempBuffer)
                {
                    newStrVec.push_back(c);
                }
                insideBrackets = false;
            }
        }
        else
        {
            if (insideBrackets)
            {
                tempBuffer.push_back(*ptr);
            }
        }
        ptr++;
    }
    newStrVec.push_back(L'\0');

    
    wchar_t* new_str = new wchar_t[newStrVec.size()];
    size_t i = 0;
    for (size_t i = 0; i < newStrVec.size(); i++) 
    {
        new_str[i] = newStrVec[i];
    }
    

    std::wofstream file("strings.txt", std::ios::app);
    file << "Initial str: " << str << std::endl;
    file << "Changed str: " << new_str << std::endl << std::endl;
    file.close();

    delete[] str;
    str = new_str;

    std::wcout << L"Str has changed and saved to file\n";
};
    


void MyString::print() 
{
    std::wcout << L"Current str: " << (str ? str : L"(empty)") << std::endl;
};
    
