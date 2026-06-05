@echo off
setlocal

set "UE_EDITOR=C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe"
set "PROJECT=%~dp0..\SoulLike.uproject"
set "MAP=/Game/_dev/Level/Menu"

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

echo Steam OSS test requires Steam to be running.
echo Running two Steam clients on one PC with one Steam account can fail.
echo For a real Steam session test, use two PCs or two Steam accounts.
echo.

echo Starting SoulLike Steam host candidate...
start "SoulLike Steam Host" "%UE_EDITOR%" "%PROJECT%" "%MAP%" -game -log -windowed -NoVSync -ExecCmds="t.IdleWhenNotForeground 0,t.MaxFPS 60,Slate.bAllowThrottling 0,r.VSync 0" -ResX=1280 -ResY=720 -WinX=40 -WinY=40

echo Waiting before starting the second instance...
timeout /t 8 /nobreak >nul

echo Starting SoulLike Steam client candidate...
start "SoulLike Steam Client" "%UE_EDITOR%" "%PROJECT%" "%MAP%" -game -log -windowed -NoVSync -ExecCmds="t.IdleWhenNotForeground 0,t.MaxFPS 60,Slate.bAllowThrottling 0,r.VSync 0" -ResX=1280 -ResY=720 -WinX=1360 -WinY=40

echo Launch commands were sent. Host from the first window, then refresh/join from the second.
endlocal
