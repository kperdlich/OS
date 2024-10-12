//
// Created by n3dry on 04.10.24.
//

#include <stdio.h>

const char* getHelloWorld()
{
    return "Hello World";
}

int add(int a, int b)
{
    return a + b;
}

extern "C" int TestFunc(int a)
{
    return add(a, 10);
}

int main()
{
    puts("main called!");
    puts(getHelloWorld());
    const int a = add(10, -10);
    return a;
}