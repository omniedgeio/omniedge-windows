#!/usr/bin/env pwsh
<#! Windows packaging script for OmniEdge #>
$ErrorActionPreference = 'Stop'
$repoRoot = (Get-Location).Path
$exe = Get-ChildItem -Path $repoRoot -Filter *.exe -Recurse | Where-Object { $_.Name -match 'OmniEdge|OmniEdge|Omniedge|omniedge' } | Select-Object -First 1
if (-not $exe) { Write-Error 'Executable not found'; exit 1 }
$stage = Join-Path $repoRoot 'packaging/stage'
if (Test-Path $stage) { Remove-Item -Recurse -Force $stage }
New-Item -ItemType Directory -Path $stage | Out-Null
$binDir = Join-Path $stage 'bin'; New-Item -ItemType Directory -Path $binDir | Out-Null
$langDir = Join-Path $stage 'languages'; New-Item -ItemType Directory -Path $langDir | Out-Null
$platDir = Join-Path $stage 'platforms'; New-Item -ItemType Directory -Path $platDir | Out-Null
# Copy executable
Copy-Item $exe.FullName -Destination $stage -Force
# Copy translations
$qmFiles = Get-ChildItem -Path (Join-Path $repoRoot 'languages') -Filter '*.qm' -ErrorAction SilentlyContinue
if ($qmFiles) { foreach ($f in $qmFiles) { Copy-Item $f.FullName -Destination $langDir -Force } }
# Copy Qt DLLs
if ($env:QTDIR -and (Test-Path "$env:QTDIR\bin")) { Copy-Item (Join-Path $env:QTDIR 'bin\*.dll') -Destination $binDir -Force -Recurse; if (Test-Path (Join-Path $env:QTDIR 'plugins\platforms\*')) { Copy-Item (Join-Path $env:QTDIR 'plugins\platforms\*') -Destination $platDir -Force -Recurse } } else { Write-Warning 'QTDIR not set or not found; Qt DLLs may be missing.' }
# Output and zip
$outDir = Join-Path $repoRoot 'packaging/output'; if (-Not (Test-Path $outDir)) { New-Item -ItemType Directory -Path $outDir | Out-Null }
$zipPath = Join-Path $outDir 'omniedge-windows-setup.zip'; if (Test-Path $zipPath) { Remove-Item $zipPath -Force } Compress-Archive -Path (Join-Path $stage '*') -DestinationPath $zipPath -Force
Write-Output 'Packaged to ' + $zipPath
