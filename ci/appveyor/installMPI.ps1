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
    Write-Host "Installing CMake 3.4.0 ..." -ForegroundColor Cyan
    $exePath = "$($env:USERPROFILE)/cmake-3.15.0-rc4-win64-x64.msi"
    Write-Host "Downloading..."
    #(New-Object Net.WebClient).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.15.0-rc4/cmake-3.15.0-rc4-win64-x64.msi', $exePath)
    (New-Object Net.WebClient).DownloadFile('https://cmake.org/files/v3.15/cmake-3.15.0-rc4-win64-x64.msi', $exePath)
    Write-Host "Installing..."
    cd $($env:USERPROFILE)
    dir
    msiexec.exe /i cmake-3.15.0-rc4-win64-x64.msi /quiet /l* myjjjjjjlog.log
    Write-Host "execute.msiexec.exe /i $exePath /quiet /l* myjjjjjjlog.log.... "
    dir
    cmake --version
    Write-Host "CMake 3.4.0 installed" -ForegroundColor Green
}

function main() {
    InstallMPI
    InstallCmake
}

main