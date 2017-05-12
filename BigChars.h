#ifndef BIGCHARS_H
#define BIGCHARS_H

#include "myterm.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int bigchars[256];
int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(int *big, int x, int y, enum Colors fgcolor, enum Colors bgcolor);
int bc_setbigcharpos(int *big, int x, int y, short int value);
int bc_getbigcharpos(int *big, int x, int y, short int *value);
int bc_bigcharwrite(int fd, int *big, int count);
int bc_bigcharread(int fd, int *big, int need_count, int *count);

#endif //BIGCHARS_H
