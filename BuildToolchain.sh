export ARCH=i686
export PREFIX="$PWD/Toolchain/Local/$ARCH"
export TARGET=$ARCH-elf
export PATH="$PREFIX/bin:$PATH"

export GCC="gcc-12.2.0"
export BINUTILS="binutils-2.39"
export GCC_GZ="$GCC.tar.gz"
export BINUTILS_GZ="$BINUTILS.tar.gz"
export GCC_LINK="https://ftp.gwdg.de/pub/misc/gcc/releases/gcc-12.2.0/$GCC_GZ"
export BINUTILS_LINK="https://ftp.gnu.org/gnu/binutils/$BINUTILS_GZ"

export BUILD="Build"
export ARTIFACTS="Artifacts"
export BUILD_PATH="$BUILD/$ARCH"
export ARTIFACTS_PATH="$ARTIFACTS/$ARCH"

mkdir -p Toolchain
cd Toolchain
rm -f -d -r $ARTIFACTS
rm -f -d -r $BUILD
mkdir -p $ARTIFACTS_PATH
mkdir -p $BUILD_PATH

wget $GCC_LINK -P $ARTIFACTS_PATH
wget $BINUTILS_LINK -P $ARTIFACTS_PATH

tar -xf $ARTIFACTS_PATH/$GCC_GZ -C $ARTIFACTS_PATH
tar -xf $ARTIFACTS_PATH/$BINUTILS_GZ -C $ARTIFACTS_PATH

cd $BUILD_PATH
mkdir build-binutils
cd build-binutils
../../../$ARTIFACTS_PATH/$BINUTILS/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd ..
mkdir build-gcc
cd build-gcc
../../../$ARTIFACTS_PATH/$GCC/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

