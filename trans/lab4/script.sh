#!/bin/bash
rm lex.yy.c
rm y.tab.c
rm y.tab.h
rm a.out
yacc -d yacc
lex program
cc y.tab.c lex.yy.c
cat test | ./a.out
