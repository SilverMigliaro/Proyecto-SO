#!/bin/bash
gcc -o shell shell.c admin_comandos.c comandos.c comandos/cat.c comandos/cd.c comandos/clear.c comandos/exit.c comandos/ls.c comandos/mkdir.c comandos/rm.c comandos/touch.c comandos/chmod.c comandos/help.c
./shell
