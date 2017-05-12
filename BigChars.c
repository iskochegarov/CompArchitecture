#include "BigChars.h"

int bc_printA(char* str)
{
    printf("\E(0%s\E(B", str);

    return 0;
}

int bc_box (int x1, int y1, int x2, int y2)
{
    int i;

    printf("\E[%d;%dH", x1, y1);
    bc_printA("l"); //левый верхний угол
    for (i = 1; i <= y2 - 2; i++)
    {
        bc_printA("q"); //горизонтальная
    }
    bc_printA("k"); //правый верхний угол

    for (i = 1; i <= x2 - 2; i++)
    {
        printf("\E[%d;%dH", x1+i, y1);
        bc_printA("x"); //вертикальная
    }
    for (i = 1; i <= x2 - 2; i++)
    {
        printf("\E[%d;%dH", x1+i, y1+y2-1);
        bc_printA("x"); //вертикальная
    }
    printf("\E[%d;%dH", x1+x2-1, y1);
    bc_printA("m"); //левый нижний угол

    for (i = 1; i <= y2 - 2; i++)
    {
        bc_printA("q"); //горизонтальная
    }

    bc_printA("j"); //правый нижний угол
    return 0;
}

int bc_printbigchar(int *big, int x, int y, enum Colors fgcolor, enum Colors bgcolor)
{
    int maxx, maxy;
    int pos, bit;
    int i, j;
    char row[9];

    mt_getscreensize(&maxy, &maxx);
    if ((x < 0) || (y < 0) || (x + 8 > maxx) || (y + 8 > maxy))
        return -1;
    row[8] = '\0';
    mt_setfgColor(fgcolor);
    mt_setbgColor(bgcolor);
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            pos = i >> 2;
            bit = (big[pos] >> ((i % 4) * 8 + j)) & 1;
            if (bit == 0)
                row[j] = ' ';
            else
                row[j] = 'a';
        }
        mt_gotoXY(x, y + i);
        bc_printA(row);
        printf("\n");
    }
    mt_setfgColor(LightBlue);
    mt_setbgColor(Black);
    return 0;
}

int bc_setbigcharpos(int *big, int x, int y, short int value)
{
    if (x < 0 || x > 7 || y < 0 || y > 7 || value > 1 || value < 0)
        return -1;

    int part = x / 4;
    x %= 4;

    if (value)
        big[part] |= (1 << (8 * x + y));
    else
        big[part] &= ~(1 << (8 * x + y));
    return 0;
}

int bc_getbigcharpos(int *big, int x, int y, short int *value)
{
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return -1;
    short int part = x / 4;
    x %= 4;
    if (big[part] & (1 << (8 * x + y)))
    {
        *value = 1;
    }
    else
        *value = 0;
    return 0;
}

int bc_bigcharwrite(int fd, int *big, int count)
{
    for (int i = 0; i < count * 2; i++)
        if (write(fd, &big[i], sizeof(int)) == -1)
            return 1;
    return 0;
}

int bc_bigcharread(int fd, int *big, int need_count, int *count)
{
    for (*count = 0; (*count < need_count * 2); *count += 1)
        if (read(fd, &big[*count], sizeof(int)) == -1)
            return 1;
    return 0;
}
