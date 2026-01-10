#!/usr/bin/env pwsh
param(
  [string]$SourceDir,
  [string]$OutputFile
)
$ErrorActionPreference = 'Stop'
$repoRoot = (Get-Location).Path
$nsisScript = Join-Path $repoRoot 'packaging/nsis/omniedge_installer.nsi'
if (-not (Test-Path $nsisScript)) {
  Write-Host 'NSIS script not found, skipping installer build.'
  exit 0
}
$makensis = $null
$paths = @(
  "$env:ProgramFiles(x86)\\NSIS\\makensis.exe",
  "$env:ProgramFiles\\NSIS\\makensis.exe",
  'C:\\Program Files\\NSIS\\makensis.exe'
)
foreach ($p in $paths) {
  if (Test-Path $p) { $makensis = $p; break }
}
if (-not $OutputFile) {
  $OutputFile = Join-Path (Join-Path $repoRoot 'packaging/output') 'omniedge-windows-installer.exe'
}
if (-not $makensis) {
  Write-Host 'NSIS not found on this runner; skipping installer packaging.'
  exit 0
}
$SourceDirParam = (Convert-Path $SourceDir)
$OutputParam = (Convert-Path $OutputFile)
& $makensis -D SOURCE_DIR="$SourceDirParam" -D OUTPUT_FILE="$OutputParam" "$nsisScript"
Write-Host "NSIS installer created at: $OutputParam"