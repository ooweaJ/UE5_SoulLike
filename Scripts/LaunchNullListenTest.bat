@echo off
setlocal

set "UE_EDITOR=C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe"
set "PROJECT=%~dp0..\SoulLike.uproject"
set "MAP=/Game/_dev/Level/MainWorld"
set "SERVER_URL=%MAP%?listen"
set "CLIENT_URL=127.0.0.1"

if not exist "%UE_EDITOR%" (
    echo UnrealEditor.exe was not found:
    echo %UE_EDITOR%
    pause
    exit /b 1
)

if not exist "%PROJECT%" (
    echo SoulLike.uproject was not found:
    echo %PROJECT%
    pause
    exit /b 1
)

echo Starting SoulLike listen server...
start "SoulLike Listen Server" "%UE_EDITOR%" "%PROJECT%" "%SERVER_URL%" -game -log -windowed -NoVSync -ExecCmds="t.IdleWhenNotForeground 0,t.MaxFPS 60,Slate.bAllowThrottling 0,r.VSync 0" -ResX=1280 -ResY=720 -WinX=40 -WinY=40 -nosteam

echo Waiting for the listen server to initialize...
timeout /t 8 /nobreak >nul

echo Starting SoulLike client...
start "SoulLike Client" "%UE_EDITOR%" "%PROJECT%" "%CLIENT_URL%" -game -log -windowed -NoVSync -ExecCmds="t.IdleWhenNotForeground 0,t.MaxFPS 60,Slate.bAllowThrottling 0,r.VSync 0" -ResX=1280 -ResY=720 -WinX=1360 -WinY=40 -nosteam

echo Launch commands were sent.
endlocal
