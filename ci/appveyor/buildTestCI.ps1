function BuildTestCI() {
    # Go to build directory and build
    # Build TestCI with cmake
    md build
    cd build
    cmake -G "Visual Studio 14 2015 Win64" ../
    dir
    msbuild Test.sln
    cd TestCI/Release
    dir
    ./Test.exe
    cmake --build . --target INSTALL --config release
    cd ..
}

function main() {
    BuildTestCI
}

main