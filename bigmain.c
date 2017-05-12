#include <fcntl.h>
#include "memory.h"
#include "BigChars.h"

#define DUMP_FILENAME "memory.dump"

int ic = 0, ac = 0, cursor = 0, bigchars[256];

int flg_state (int8_t regist)
{
    int8_t value;
    sc_regGet (regist, &value);
    return value;
}

void color_reset ()
{
    mt_setbgColor(Black);
    mt_setfgColor(LightBlue);
}

void update_accumulator ()
{
    char str[4];
    sprintf(str, "%04X", ac);

    color_reset();
    bc_box(1, 62, 3, 22);
    mt_gotoXY(67, 0);
    printf(" Accumulator \n");
    mt_gotoXY(70, 2);
    printf("%s\n",str);
}

void update_ic ()
{
    char str[4];
    sprintf(str, "%04X", ic);

    color_reset();
    bc_box(4, 62, 3, 22);
    mt_gotoXY(63, 4);
    printf(" InstructionCounter \n");
    mt_gotoXY(70, 5);
    printf("%s\n", str);
}

void update_operation ()
{
    char str[8];
    sprintf(str, "+%04X", mem[cursor]);

    color_reset();
    bc_box(7, 62, 3, 22);
    mt_gotoXY(68, 7);
    printf(" Operation \n");
    mt_gotoXY(69, 8);
    printf("%s\n", str);
}

void update_flags()
{
    color_reset();

    bc_box(10, 62, 3, 22);
    mt_gotoXY(70, 10);
    printf(" Flags \n");

    if (flg_state(FLG_OVERFLOW) == 0)
        mt_setfgColor(Green);
    else
        mt_setfgColor(Red);

    mt_gotoXY(69, 11);
    printf ("O\n");

    if (flg_state(FLG_ZERO_DIVISION) == 0)
        mt_setfgColor(Green);
    else
        mt_setfgColor(Red);

    mt_gotoXY(71, 11);
    printf("Z\n");

    if (flg_state(FLG_MEMORY_FAULT) == 0)
        mt_setfgColor(Green);
    else
        mt_setfgColor(Red);

    mt_gotoXY(73, 11);
    printf("M\n");

    if (flg_state(FLG_TICK_IGNORE) == 0)
        mt_setfgColor(Green);
    else
        mt_setfgColor(Red);

    mt_gotoXY(75, 11);
    printf("I\n");

    if (flg_state(FLG_WRONG_COMMAND) == 0)
        mt_setfgColor(Green);
    else
        mt_setfgColor(Red);

    mt_gotoXY(77, 11);
    printf("C\n");

    color_reset();
}

void update_memory()
{
    color_reset();
    bc_box(1, 1, 12, 61);
    mt_gotoXY(27, 0);
    printf(" Memory \n");
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j<10; j++)
        {
            mt_gotoXY(j*5+2+j, i+2);
            if (i + j == cursor)
            {
                mt_setbgColor(LightBlue);
                mt_setfgColor(Black);
            }

            printf("+%04X\n", mem[i*10+j]);
            color_reset();
        }

    }
}

void update_bigchars()
{
    char str[16];
    sprintf(str, "+%04X", mem[cursor]);

    color_reset();
    bc_box(13, 1, 10, 46);
    for (int i = 0; str[i] != '\0'; ++i)
        bc_printbigchar(bigchars + (str[i] * 2), i*8+2, 14, LightBlue, Black);
    printf("\n");
}

void keys_box ()
{
    color_reset();

    bc_box(13, 47, 10, 37);
    mt_gotoXY(61, 13);
    printf(" Keys: \n");
    mt_gotoXY(48, 14);
    printf(" l  - load\n");

    mt_gotoXY(48, 15);
    printf(" s  - save\n");

    mt_gotoXY(48, 16);
    printf(" r  - run\n");

    mt_gotoXY(48, 17);
    printf(" t  - step\n");

    mt_gotoXY(48, 18);
    printf(" i  - reset\n");

    mt_gotoXY(48, 19);
    printf(" F5 - accumulator\n");

    mt_gotoXY(48, 20);
    printf(" F6 - instructionCounter\n");

    printf("\n\n\n");

}

void refresh ()
{
    mt_clrscr();
    update_memory();
    update_bigchars();
    update_accumulator();
    update_ic();
    update_operation();
    update_flags();

    keys_box();
}

int main ()
{
    sc_memoryInit();
    sc_regInit();

    int16_t val = 42;
    sc_memorySet(0, val);
    sc_memorySave(DUMP_FILENAME);
    sc_memoryLoad(DUMP_FILENAME);
    sc_regSet (FLG_MEMORY_FAULT, 1);

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

    refresh();
    return 0;
}
