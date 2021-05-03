############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "Mazend"
!define COMP_NAME "Group O"
!define VERSION "00.00.00.01"
!define COPYRIGHT "Group O © 2021"
!define DESCRIPTION "Game"
!define LICENSE_TXT "license.txt"
!define INSTALLER_NAME "mazend_installer.exe"
!define MAIN_APP_EXE "MAZEND_GAME.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor /SOLID Lzma
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\Mazend"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Mazend"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "mazend\MAZEND_GAME.exe"
File "mazend\sfml-audio-2.dll"
File "mazend\sfml-graphics-2.dll"
File "mazend\sfml-system-2.dll"
File "mazend\sfml-window-2.dll"
File "mazend\openal32.dll"
SetOutPath "$INSTDIR\res\sprites"
File "mazend\res\sprites\botSpriteSheet.png"
File "mazend\res\sprites\midspritesheet.png"
File "mazend\res\sprites\spriteSheet.png"
SetOutPath "$INSTDIR\res\maps\lvl1"
File "mazend\res\maps\lvl1\botMap.txt"
File "mazend\res\maps\lvl1\midMap.txt"
File "mazend\res\maps\lvl1\topMap.txt"
SetOutPath "$INSTDIR\res\fonts"
File "mazend\res\fonts\ZenDots-Regular.ttf"
SetOutPath "$INSTDIR\res\audio"
File "mazend\res\audio\movement.wav"
File "mazend\res\audio\music.wav"
File "mazend\res\audio\select.ogg"
File "mazend\res\audio\select.wav"
File "mazend\res\audio\collection.wav"
File "mazend\res\audio\defeat.wav"
File "mazend\res\audio\movement.wav"
File "mazend\res\audio\victory.wav"
SetOutPath "$APPDATA\Mazend"
File "mazend\database\scores.txt"
File "mazend\database\isFullScreen.txt"
File "mazend\database\controls.txt"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\Mazend"
CreateShortCut "$SMPROGRAMS\Mazend\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\Mazend\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\Mazend\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\MAZEND_GAME.exe"
Delete "$INSTDIR\sfml-audio-2.dll"
Delete "$INSTDIR\sfml-graphics-2.dll"
Delete "$INSTDIR\sfml-system-2.dll"
Delete "$INSTDIR\sfml-window-2.dll"
Delete "$INSTDIR\openal32.dll"
Delete "$INSTDIR\res\sprites\botSpriteSheet.png"
Delete "$INSTDIR\res\sprites\midspritesheet.png"
Delete "$INSTDIR\res\sprites\spriteSheet.png"
Delete "$INSTDIR\res\maps\lvl1\botMap.txt"
Delete "$INSTDIR\res\maps\lvl1\midMap.txt"
Delete "$INSTDIR\res\maps\lvl1\topMap.txt"
Delete "$INSTDIR\res\fonts\ZenDots-Regular.ttf"
Delete "$INSTDIR\res\audio\movement.wav"
Delete "$INSTDIR\res\audio\music.wav"
Delete "$INSTDIR\res\audio\select.ogg"
Delete "$INSTDIR\res\audio\select.wav"
Delete "$INSTDIR\res\audio\collection.wav"
Delete "$INSTDIR\res\audio\defeat.wav"
Delete "$INSTDIR\res\audio\movement.wav"
Delete "$INSTDIR\res\audio\victory.wav"

RmDir "$INSTDIR\res\fonts"
RmDir "$INSTDIR\res\audio"
RmDir "$INSTDIR\res\maps\lvl1"
RmDir "$INSTDIR\res\sprites"
RmDir "$INSTDIR\res\maps"
RmDir "$INSTDIR\res"

Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\Mazend\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\Mazend\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\Mazend\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\Mazend"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

