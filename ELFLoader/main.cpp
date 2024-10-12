//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"

typedef int(*TestFunc)(int);
typedef int(*MainFunc)();


int main()
{
    ELF::ELFLoader loader("test.o");
    if (loader.load()) {
        //loader.dump();
        TestFunc testFunc = reinterpret_cast<TestFunc>(loader.findFunc("TestFunc"));
        static constexpr const int arg = 20;
        printf("Calling '%s' at location '%p' with value '%d' returns: '%d'\n", "TestFunc", testFunc, arg, testFunc(arg));

        MainFunc mainFunc = reinterpret_cast<MainFunc>(loader.findFunc("main"));
        printf("Calling 'main' at location '%p':\n", mainFunc);
        printf("'main' returns: '%d'\n", mainFunc());
    }
}