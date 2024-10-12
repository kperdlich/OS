//
// Created by n3dry on 04.10.24.
//

extern "C" const char* getHelloWorld()
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
    return 0;
}