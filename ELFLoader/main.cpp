//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"

typedef int(*TestFunc)(int);
typedef const char*(*GetHelloWorldFunc)();


int main()
{
    ELF::ELFLoader loader("test.o");
    const char* const func = "TestFunc";
    if (loader.load()) {
        loader.dump();
        TestFunc testFunc = reinterpret_cast<TestFunc>(loader.findFunc(func));
        static constexpr const int arg = 20;
        printf("Calling '%s' at location '%p' with value '%d' returns: '%d'\n", func, testFunc, arg, testFunc(arg));

        GetHelloWorldFunc getHelloWorld = reinterpret_cast<GetHelloWorldFunc>(loader.findFunc("getHelloWorld"));
        printf("Calling '%s' at location '%p' returns: '%s'\n", "getHelloWorld", getHelloWorld, getHelloWorld());
    }
}