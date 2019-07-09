function BuildTestCI() {
    # Go to build directory and build
    # Build TestCI with cmake
    md build
    cd build
    cmake -G "Visual Studio 14 2015 Win64" ../
    cmake --build . --target INSTALL --config release
#    dir
#    msbuild Test.sln
#    cd TestCI/Release
#    dir
#    ./Test.exe
#   cd ../../../
#   dir
}

function main() {
    BuildTestCI
}

main