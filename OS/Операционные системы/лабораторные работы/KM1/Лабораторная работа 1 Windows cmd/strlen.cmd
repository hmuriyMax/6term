@echo off
 call :len var "������"                                 &:: "var" - ���������� ���� ������� ��������
 echo ����� ������: %var%                      
goto :eof

:len
 set len=%~2
 if not "%len%"=="" set /a %1+=1 & call :len %1 "%len:~1%"
goto :eof