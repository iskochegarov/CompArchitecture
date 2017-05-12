#include <fcntl.h>
#include "BigChars.h"
#include "myGUI.h"
#include "myReadKey.h"

#define DUMP_FILENAME "memory.dump"
#define SETTINGS_FILENAME "myterm.save"

int main ()
{
    int ic = 0, ac = 0, cursor = 0;
    keys key;
    sc_memoryInit();
    sc_regInit();

    for (int i = 0; i < 100; i++)
    {
        sc_memorySet(i, i);
    }

    sc_memorySave(DUMP_FILENAME);
    sc_memoryLoad(DUMP_FILENAME);

    sc_regSet (FLG_MEMORY_FAULT, 1);
    sc_regSet (FLG_OVERFLOW, 1);
    sc_regSet (FLG_TICK_IGNORE, 1);
    sc_regSet (FLG_WRONG_COMMAND, 1);
    sc_regSet (FLG_ZERO_DIVISION, 1);

    mt_clrscr();

    int fd;
    if ((fd = open("bigchars", O_CREAT | O_RDWR )) == -1)
    {
        perror("Open error\n");
        return -1;
    }

    int cnt;
    bc_bigcharread(fd, bigchars, 128, &cnt);
    close(fd);

    do
    {
        refresh(ac, ic, cursor);
        rk_readkey(&key);

        if (key == s)
        {
            sc_memorySave(DUMP_FILENAME);
            rk_mytermsave(SETTINGS_FILENAME);
        }

        else if (key == l)
        {
            sc_memoryLoad(DUMP_FILENAME);
            rk_mytermrestore(SETTINGS_FILENAME);
        }

        else if (key == r)
        {
            sc_regSet(FLG_TICK_IGNORE, 0);
            //Обработка прерываний
        }

        else if (key == i)
        {
            sc_memoryInit();
            sc_regInit();
            ac = 0;
            ic = 0;
            cursor = 0;
        }

        else if (key == left && cursor > 0)
            cursor -= 1;

        else if (key == right && cursor < 99)
            cursor += 1;

        else if (key == up && cursor > 9)
            cursor -= 10;

        else if (key == down && cursor < 90)
            cursor += 10;
    }
    while (key != q);

    mt_clrscr();

    return 0;
}
