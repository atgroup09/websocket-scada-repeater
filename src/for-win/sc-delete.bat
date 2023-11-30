@echo off

set ServiceName=WsRepeater


sc stop "%ServiceName%"

rem # Timeot 5 sec 
ping -n 6 127.0.0.1>nul

sc delete "%ServiceName%" 
