//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"

int main(int argc, char* argv[])
{
    ELF::ELFLoader loader("ELFLoaderTest");
    if (loader.load()) {
        loader.dump();
    }
}