#!/usr/bin/env pwsh
# packaging/nsis/omniedge_installer.nsi
!define SOURCE_DIR "$SOURCE_DIR"
!define OUTPUT_FILE "$OUTPUT_FILE"

Name "OmniEdge Windows"
OutFile "${OUTPUT_FILE}"
InstallDir "$PROGRAMFILES\\OmniEdge"
InstallDirRegKey HKLM "Software\\OmniEdge" "Install_Dir"
RequestExecutionLevel admin

Page Directory
Page Instfiles

Section "Install"
  SetOutPath "$INSTDIR"
  File /r "${SOURCE_DIR}\\*.*"
  CreateDirectory "$SMPROGRAMS\\OmniEdge"
  CreateShortCut "$DESKTOP\\OmniEdge.lnk" "$INSTDIR\\OmniEdge.exe"
  CreateShortCut "$SMPROGRAMS\\OmniEdge\\OmniEdge.lnk" "$INSTDIR\\OmniEdge.exe"
SectionEnd

Section "Uninstall"
  Delete "$DESKTOP\\OmniEdge.lnk"
  Delete "$SMPROGRAMS\\OmniEdge\\OmniEdge.lnk"
  RMDir "$INSTDIR"
SectionEnd
