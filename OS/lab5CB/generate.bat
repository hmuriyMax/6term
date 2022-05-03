@echo off
if exist func.obj del func.obj 
if exist CutterLib.dll del CutterLib.dll 
C:\masm32\bin\ml /c /coff main.asm 
C:\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL /DEF:func.def main.obj 
Pause