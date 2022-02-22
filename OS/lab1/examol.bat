@echo off

set /p directory="Directory for search: "
set /p username="Input username: "
set /p minSize="Minimal size of files: "
set /p todo="Path to dir for moving: "
Net User %username% 1,2>nul
If %ErrorLevel% Neq 0 (
    echo This user does not exist
) else (
    if %minSize% lss 0 (
        echo Incorrect size
    ) else (
        mkdir %todo%
        set todo=%todo:\=/%
        set res=0
        for /f %%A in ('dir %directory% /b /s /a-d ') do (
            echo weee
            for /f "tokens=1,2,3,4* delims= " %%b in ('dir /q /a-d /-c /o:s %%A ^|findstr /c:"%username%"') do (
                if %%d geq %minSize% (
                    set /a res+=1
                    move %%A %todo% >nul
                )
            )
        )
        if %res% == 0 (
            echo No files for this params
        ) else (
            echo Was moved %res% files
        )
    )
)