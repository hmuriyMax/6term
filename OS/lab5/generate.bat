@echo off 
if exist main.obj del main.obj 
if exist WordSearchAssembler.dll del WordSearchAssembler.dll 
C:\masm32\bin\ml /c /coff main.asm 
C:\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL /DEF:func.def main.obj 
dir func.* 
Pause