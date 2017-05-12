#ifndef MYTERM_H
#define MYTERM_H

enum Colors
{
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Purple = 5,
    LightBlue = 6,
    White = 7
};

int mt_clrscr(void);
int mt_gotoXY(int X, int Y);
int mt_getscreensize(int *rows, int *cols);
int mt_setfgColor(enum Colors color);
int mt_setbgColor(enum Colors color);

#endif
