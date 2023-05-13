@echo off
setlocal enabledelayedexpansion

for %%a in (*.*) do (
    set "filename=%%~na"
    set "extension=%%~xa"
    if "!filename:~1,1!"=="" (
        ren "%%a" "0!filename!!extension!"
        echo rename file %%a to 0!filename!!extension!
    )
)

pause