export PREFIX="$PWD/Toolchain/Local/i686"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# TODO Download gcc and binutils if not exist

cd Toolchain/Build
mkdir build-binutils
cd build-binutils
../binutils-gdb/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd ..
mkdir build-gcc
cd build-gcc
../gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc