#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#ifndef MYREADKEY_H
#define MYREADKEY_H

typedef enum {q, s, l, r, t, i, F5, F6, up, down, left, right, enter, other} keys;



int rk_readkey(keys*);
int rk_mytermsave(char*);
int rk_mytermrestore(char*);
int rk_mytermregime(int, int, int, int, int);

#endif
