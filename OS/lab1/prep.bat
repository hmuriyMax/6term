@echo off
setlocal enabledelayedexpansion

set user=%computername%\%username%
set now_date=%date%
set now_day=!now_date:~0,2!
set now_month=!now_date:~3,2!
set "path=C:\prog\5term\ABC\lab7"

set /a count=0

dir %path% /t:c /-c /a:-d /o:-n /q > file.txt

for /f "skip=5 TOKENS=1,3,4,5* DELIMS= " %%a in (file.txt) do (
    echo %%a %%b %%c %%d
    set file_date=%%a
    set file_day=!file_date:~0,2!
    set file_month=!file_date:~3,2!!

    if %%c==!user! (
        if !file_month!==!now_month!-1 (
            set /a dif=!now_day!+30-!file_day!
            if !dif! gtr 14 (
                if %%b geq 10 (
                    echo %%d > result.txt
                )
            ) 
        ) else (
            if "!file_month!"=="!now_month!" (
                set /a dif=!now_day!-!file_day!
                if !dif! gtr 14 (
                    if %%b geq 10 (
                        echo %%d > result.txt
                    )
                )
            ) else (
                if !file_month! gtr !now_month! (
                    if %%b geq 10 (
                        echo %%d > result.txt
                    )
                )
            )
        )
    )
)