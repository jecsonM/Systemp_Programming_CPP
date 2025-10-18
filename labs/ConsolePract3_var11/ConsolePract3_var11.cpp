#include "MyString.h"
#include <iostream>

int main()
{
    MyString s1;
    s1.set();
    s1.print();

    s1.update();
    s1.print();

    std::wcout << L"\nCreating second MyStr with params:\n";
    MyString s2(L"SomeStringSample");
    s2.print();

    s2.update();
    s2.print();

    std::wcout << L"\nCopying MyStr:\n";
    MyString s3 = s2;
    s3.print();

    
    return 0;

}
