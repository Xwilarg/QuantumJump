$ErrorActionPreference = "Stop"

if (-not (Test-Path -Path "build")) {
    New-Item "build" -ItemType "directory"
}
Set-Location -Path build

cmake ..
cmake --build . --config Release

Copy-Item "Release/*.exe" -Destination ".."
Set-Location -Path ..