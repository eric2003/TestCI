function InstallMPI() {
    md mpi
    cd mpi
    # install MPI SDK and Runtime
    Write-Host "Installing Microsoft MPI SDK..."
    appveyor DownloadFile https://download.microsoft.com/download/A/E/0/AE002626-9D9D-448D-8197-1EA510E297CE/msmpisdk.msi
    Start-Process -FilePath msiexec.exe -ArgumentList "/quiet /qn /i msmpisdk.msi" -Wait
    Write-Host "Microsoft MPI SDK installation complete"
    Write-Host "Installing Microsoft MPI Runtime..."
    appveyor DownloadFile https://download.microsoft.com/download/A/E/0/AE002626-9D9D-448D-8197-1EA510E297CE/msmpisetup.exe
    Start-Process -FilePath MSMpiSetup.exe -ArgumentList -unattend -Wait
    Write-Host "Microsoft MPI Runtime installation complete..."
    cd 
    dir
    c:
    cd "C:/Program Files (x86)/Microsoft SDKs/MPI"
    dir
}

function InstallCmake() {
    md cmake_tmp
    cd cmake_tmp
    dir
    Write-Host "Installing CMake 3.14.5 ..." -ForegroundColor Cyan
    $exePath = "$($env:USERPROFILE)/cmake-3.14.5-win64-x64.msi"
    Write-Host "Downloading..."
    Start-FileDownload 'https://github.com/Kitware/CMake/releases/download/v3.14.5/cmake-3.14.5-win64-x64.msi'
    dir
    Write-Host "Installing..."
    cmake-3.14.5-win64-x64.msi
    Write-Host "CMake 3.14.5 installed installation complete"
    dir
    dir
    cmake --version
    Write-Host "CMake 3.14.5 installed" -ForegroundColor Green
}

function main() {
    InstallMPI
    InstallCmake
}

main