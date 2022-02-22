@echo %~1

setlocal enabledelayedexpansion
SETLOCAL
if "%~2"=="" (
    set /p num="Enter the number: "
) else (
    set num=%~2
)
echo 4uclo B 7-ou c/c: %num%

if "%num:~0,1%"=="-" (
    set sgn=-
    set num= %num:~1%
)
set isZ=0
set rpart=
set fpart=

set copynum=n%num%
set /a dotfound=0
:checker
    set copynum=%copynum:~1%
    if "%copynum:~0,1%"=="." (
        if "%dotfound%"=="0" (
            set /a dotfound=1
            goto:checker
        ) else (
            echo Dot is doubled in the number!
            goto:end
        )
    ) else if "%copynum:~0,1%"=="1" (
        goto:checker
    ) else if "%copynum:~0,1%"=="2" (
        goto:checker
    ) else if "%copynum:~0,1%"=="3" (
        goto:checker
    ) else if "%copynum:~0,1%"=="4" (
        goto:checker
    ) else if "%copynum:~0,1%"=="5" (
        goto:checker
    ) else if "%copynum:~0,1%"=="6" (
        goto:checker
    ) else if "%copynum:~0,1%"=="0" (
        goto:checker
    ) else if "%copynum:~0,1%"==" " (
        goto:checker
    ) else if "%copynum:~0,1%"=="~0,1" (
        goto:while_getter_real_part
    ) else (
        echo Found strange symbol: "%copynum:~0,1%"
        goto:end
    )

:while_getter_real_part
    if not "%num:~0,1%"=="~0,1" (
        if not "%num:~0,1%"=="." (
            set rpart=!rpart!%num:~0,1%
            set num=!num:~1!
            goto:while_getter_real_part
        ) else (
            goto:while_getter_float_part
        )
    ) else (
        set isZ=1
        goto :rpart_parser
    )

:while_getter_float_part
    if not "%num:~0,1%"=="~0,1" (
        set fpart=%fpart%%num:~0,1%
        set num=%num:~1%
        goto :while_getter_float_part
    ) else (
        goto :rpart_parser
    )

:rpart_parser
@echo off
    call:len %rpart%
    set /a rlen=%len%
    set rres=0
    : echo rpart: %rpart% len: %rlen%
    for /l %%i in (1,1,%rlen%) do (
        call:get_symb %rpart% %%i
        : echo get_symb[%%i]: !get_symb!
        call:conv_lit_75 !get_symb!
        set /a pw= %%i - 1 
        call:pow10 7 !pw!
        : echo !conv_lit_75! * !pow10!
        call:mltn10 !conv_lit_75! !pow10! 5
        : echo !rres! + !mltn10!
        call:sum !rres! !mltn10! 5
        set /a rres=!sum!
    )
    : echo %rres%
    if "%isZ%"=="0" (
        goto :fpart_parser
    ) else (
        if "%sgn%"=="-" (
        echo 4uclo B 5-ou c/c: -%rres%
    ) else (
        echo 4uclo B 5-ou c/c: %rres%
    )
    goto:end
    )

:fpart_parser 
    set fpart=%fpart:~1%
    call:len %fpart%
    set /a flen=%len%
    set fres=
    for /l %%i in (1, 1, %flen%) do (
        call:mltn10 !fpart! 5 7
        set fpart=!mltn10!
        call:len !mltn10!
        set /a tmplen=!len!
        set let=
        if !tmplen! gtr %flen% (
             set let=!fpart:~1,1!
             set fpart=!fpart:~2!
        ) else (
            set let=0
        ) 
        set fres=!fres!!let!
    ) 
    if "%sgn%"=="-" (
        echo 4uclo B 5-ou c/c: -%rres%.%fres%
    ) else (
        echo 4uclo B 5-ou c/c: %rres%.%fres%
    )
    goto:end

:len []
    set $out=%~0
    set /a len=0
    set "a=%~1"
    :t
    if not "%a%" == "" (
        set /a len+=1
        set a=%a:~1%
        goto:t
    )
    set %$out:~1%=%len%
    goto :EOF

:pow10 []
    SETLOCAL
    set $out=%~0
    set /a "num=%~1"
    set /a "pow=%~2"
    set /a powres=1
    :t2
    if not %pow%==0 (
        set /a powres= !powres! * %num%
        set /a pow-=1
        goto:t2
    )
    ENDLOCAL & set %$out:~1%=%powres%
    goto :EOF

:mltn10 []
    SETLOCAL
    set $out=%~0
    set "num5=%~1"
    set "num10=%~2"
    set "base=%~3"
    set /a mltres=0
    for /l %%i in (1,1,%num10%) do (
        call:sum !mltres! %num5% %base%
        set mltres= !sum!
    )
    ENDLOCAL & set %$out:~1%=%mltres%
    goto :EOF

:sum []
    SETLOCAL
    set $out=%~0
    set "num1=%~1"
    set "num2=%~2"
    set "base= %~3"
    call:len %num1%
    set /a len1= %len%
    call:len %num2%
    set /a len2= %len%
    set /a mxlen=0
    if %len1% gtr %len2% (
        set /a mxlen= %len1%
        set /a diff= %len1%-%len2% 
        for /l %%i in (1,1,!diff!) do ( 
            set num2=0!num2!
        )
    ) else (
        set /a mxlen= %len2%
        set /a diff=%len2%-%len1% 
        for /l %%i in (1,1,!diff!) do ( 
            set num1=0!num1!
        )
    )
    set /a promise=0
    set ressum=
     for /l %%i in (1,1,%mxlen%) do (
        call:get_symb !num1! %%i
        set /a lhs= !get_symb!
        call:get_symb !num2! %%i
        set /a rhs= !get_symb!
       : echo !lhs! + !rhs!
        set /a sum= !lhs! + !rhs!
        set /a sum+= !promise!
       : echo = !sum!
        set /a promise=0
        if !sum! geq %base% (
            set /a promise=1
            set /a sum-=%base% 
        )
        set ressum=!sum!!ressum!
    )
    if %promise% gtr 0 (
        set ressum=1!ressum!
    )
    ENDLOCAL & set sum=%ressum%
    goto:EOF

:get_symb []
    SETLOCAL
    set $out=%~0
    set "tstr=%~1"
    set /a tind=%~2
    if %tind%==1 (
        set sres=%tstr:~-1,1%
    ) else if %tind%==2 (
        set sres=%tstr:~-2,1%
    ) else if %tind%==3 (
        set sres=%tstr:~-3,1%
    ) else if %tind%==4 (
        set sres=%tstr:~-4,1%
    ) else if %tind%==5 (
        set sres=%tstr:~-5,1%
    )
    ENDLOCAL & set %$out:~1%=%sres%
    goto:EOF

:conv_lit_75 []
    SETLOCAL
    set $out=%~0
    set "symb=%~1"
    if (%symb% gtr 5) (
        set /a symb = %symb% - 5
        set symb = 1!symb!
    )
    ENDLOCAL & set %$out:~1%=%symb%
    goto:EOF

:end 
    ENDLOCAL
    goto:EOF