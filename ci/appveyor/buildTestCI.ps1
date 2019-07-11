function BuildTestCI() {
    # Go to build directory and build
    # Build TestCI with cmake
    md build
    cd build
    cmake -G "Visual Studio 14 2015 Win64" -DMPI_DIR="C:/Program Files (x86)/Microsoft SDKs/MPI" ../
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