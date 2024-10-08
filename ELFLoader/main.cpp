//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"

typedef int(*TestFunc)(int);

int main(int argc, char* argv[])
{
    ELF::ELFLoader loader("ELFTestProgram");
    const char* const func = "TestFunc";
    if (loader.load()) {
        loader.dump();
        TestFunc testFunc = reinterpret_cast<TestFunc>(loader.findFunc(func));
        printf("Found function '%s' at location '%p'", func, testFunc);
    }
}