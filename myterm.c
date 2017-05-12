#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "myterm.h"

int mt_clrscr(void)
{
    char *clear;
    clear = "\E[H\E[2J";
    ssize_t len = strlen(clear) * sizeof(char);
    if (write(STDOUT_FILENO, clear, strlen(clear)) < len)
    {
        return -1;
    }
    return 0;
}

int mt_gotoXY (int x, int y)
{
    int rows, cols;

    mt_getscreensize(&rows, &cols);
    if ((y < rows) && (x < cols) && (x >= 0) && (y >= 0))
    {
        printf("\E[%d;%dH", y, x);
        return 0;
    }
    else
        return -1;
}

int mt_getscreensize(int *rows, int *cols)
{
    struct winsize w;
    if(!ioctl(1, TIOCGWINSZ, &w))
    {
        *rows = w.ws_row;
        *cols = w.ws_col;
//        printf("\n [OK] Screeen size was received");
    }
    else
    {
        printf(" [Error] Screeen size wasn't received\n");
        return -1;
    }
    return 0;
}

int mt_setfgColor(enum Colors color)
{
    char fgc[10];
    memset(fgc, '\0', 10);
    sprintf(fgc, "\E[3%dm", color);

    if((write(1, fgc, sizeof(fgc))) == -1)
    {
        return -1;
    }
    return 0;
}

int mt_setbgColor(enum Colors color)
{
    char bgc[10];
    memset(bgc, '\0', 10);
    sprintf(bgc, "\E[4%dm", color);
    if((write(1, bgc, sizeof(bgc))) == -1)
    {
        return -1;
    }
    return 0;
}

