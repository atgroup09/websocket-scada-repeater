@echo off

rem # *.log     - actual log file (don't remove)
rem # *.log.arh - archived log file (free to remove)

rem # Variables

set ProDir=E:\pentaho-server\repeater

set LogName=repeater
set LogFile=%LogName%.log
set LogFileArh=%LogName%.log.arh

set ServiceName=WsRepeater


rem # Execution

sc stop "%ServiceName%"

rem # timeot 5 sec 
ping -n 6 127.0.0.1>nul

if exist "%LogFile%" (

    if exist "%LogFileArh%" (

        del %LogFileArh%
    )

    ren %LogFile% %LogFileArh%
)

rem echo. 2> %LogFile%

rem # timeot 1 sec 
ping -n 2 127.0.0.1>nul

sc start "%ServiceName%"

