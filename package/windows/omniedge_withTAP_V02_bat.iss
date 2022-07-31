; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "OmniEdge"
#define MyAppVersion "0.2.4"
#define MyAppPublisher "OmniEdge, Inc."
#define MyAppURL "https://www.omniedge.io/"
#define MyAppExeName "OmniEdge.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E883B91B-23B8-4C8B-99A0-72DF7499DD88}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=C:\10_Github\OmniEdge\Release
OutputBaseFilename=omniedge-setup-0.2.4
SetupIconFile=C:\10_Github\OmniEdge\Release\logo.ico
UninstallDisplayIcon =  {app}\{#MyAppExeName}
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\10_Github\OmniEdge\Release\0.2.4\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\10_Github\OmniEdge\Release\0.2.4\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "D:\01_N2N\Release\tap-windows-9.21.2.exe"; DestDir: "{app}"; Flags: deleteafterinstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Registry]
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "OmniEdge"; ValueData: "{app}\{#MyAppExeName}"
[Run]
Filename: {app}\tap-windows\add_tap_device.bat; \
    Parameters: "/q /passive /Q:a /c:""msiexec /q /i vcredist.msi"""; \
    StatusMsg: "Installing tap driver ..."
