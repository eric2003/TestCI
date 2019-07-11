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
    Write-Host "Installing CMake 3.14.5 ..." -ForegroundColor Cyan
    $exePath = "$($env:USERPROFILE)/cmake-3.14.5-win64-x64.msi"
    Write-Host "Downloading..."
    #(New-Object Net.WebClient).DownloadFile('https://cmake.org/files/v3.4/cmake-3.4.0-rc2-win32-x86.exe', $exePath)
    (New-Object Net.WebClient).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.14.5/cmake-3.14.5-win64-x64.msi', $exePath)
    Write-Host "Installing..."
    cmd /c start /wait $exePath /S
    cmake --version
    Write-Host "CMake 3.14.5 installed" -ForegroundColor Green
}

function main() {
    InstallMPI
    InstallCmake
}

main