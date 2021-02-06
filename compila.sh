#!/bin/bash
#Utilizzo:
# ./compila.sh -o ESEGUIBILE FILE1.c FILE2.c FILE3.c ....

gcc -g3 -fsanitize=address -fsanitize=undefined -std=gnu89 -pedantic-errors -Wall -Wextra -o VM fx.c vm.c main_vm.c
