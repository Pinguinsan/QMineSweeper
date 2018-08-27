; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "QMineSweeper (x86)"
#define MyAppVersion GetFileVersion("QMineSweeper.exe")
#define MyAppPublisher "Tyler Lewis"
#define MyAppURL "https://github.com/tlewiscpp/QMineSweeper"
#define MyAppExeName "QMineSweeper.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{71E22FB3-192C-4604-AD9F-7D4DF1F97F8B}
AppName={#MyAppName}
AppVerName={#MyAppName} {#MyAppVersion}
VersionInfoVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile=C:\Users\tlewis\GitHub\QMineSweeper\resources\licenses\LICENSE
OutputDir=C:\Users\tlewis\Downloads\QMineSweeper\
OutputBaseFilename=QMineSweeper_WinXP_x86
SetupIconFile=C:\Users\tlewis\GitHub\QMineSweeper\resources\QMineSweeper.ico
UninstallDisplayIcon=C:\Users\tlewis\GitHub\QMineSweeper\resources\QMineSweeper.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: "{app}\audio"
Name: "{app}\bearer"
Name: "{app}\iconengines"
Name: "{app}\mediaservice"
Name: "{app}\platforms"
Name: "{app}\playlistformats"
Name: "{app}\resources"
Name: "{app}\translations"

[Files]
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\QMineSweeper.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\audio\qtaudio_windows.dll"; DestDir: "{app}\audio"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\bearer\qgenericbearer.dll"; DestDir: "{app}\bearer"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\bearer\qnativewifibearer.dll"; DestDir: "{app}\bearer"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\iconengines\qsvgicon.dll"; DestDir: "{app}\iconengines"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qgif.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qicns.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qico.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qjp2.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qjpeg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qmng.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qsvg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qtga.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qtiff.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qwbmp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\imageformats\qwebp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\mediaservice\dsengine.dll"; DestDir: "{app}\mediaservice"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\mediaservice\qtmedia_audioengine.dll"; DestDir: "{app}\mediaservice"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\playlistformats\qtmultimedia_m3u.dll"; DestDir: "{app}\playlistformats"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_ca.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_cs.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_de.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_en.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_fi.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_fr.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_he.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_hu.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_it.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_ja.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_ko.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_lv.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_ru.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_sk.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "C:\Users\tlewis\GitHub\QMineSweeper\cmake-build-windowsxp-release\translations\qt_uk.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Registry]
Root: HKCR; SubKey: ".qms"; ValueType: string; ValueData: "QMineSweeper game"; Flags: uninsdeletekey
Root: HKCR; SubKey: "QMineSweeper game"; ValueType: string; ValueData: "QMineSweeper game"; Flags: uninsdeletekey
Root: HKCR; SubKey: "QMineSweeper game\Shell\Open\Command"; ValueType: string; ValueData: """{app}\QMineSweeper.exe"" ""%1"""; Flags: uninsdeletekey