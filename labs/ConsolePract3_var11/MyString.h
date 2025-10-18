#pragma once
#define _CRT_SECURE_NO_WARNINGS
class MyString {
protected:
    wchar_t* str;
public:
    MyString();

    MyString(const wchar_t* s);

    MyString(const MyString& other);

    ~MyString();

    void set();

    void update();

    void print();
};

