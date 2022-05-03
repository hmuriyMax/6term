#!/bin/bash
rm a.out
rm lex.yy.c
lex program
cc lex.yy.c
cat test.txt | ./a.out
