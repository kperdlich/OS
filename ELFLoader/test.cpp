//
// Created by n3dry on 04.10.24.
//

int add(int a, int b)
{
    return a + b;
}

extern "C" int TestFunc(int a)
{
    return add(a, 5);
}

int main()
{
    return 0;
}