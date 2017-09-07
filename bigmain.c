#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include "BigChars.h"
#include "myGUI.h"
#include "myReadKey.h"

#define DUMP_FILENAME "memory.dump"
#define SETTINGS_FILENAME "myterm.save"

int ic = 0, ac = 0, cursor = 0;
keys key;

void ic_set ()
{
    if (FLG_TICK_IGNORE == 0)
    {
        ++ic;
        cursor = ic;
    }
    refresh (ac, ic, cursor);
}

void ic_reset ()
{
    sc_regSet(FLG_TICK_IGNORE, 1);
    signal(SIGINT, SIG_DFL);
    alarm(0);
    ic = 0;
    cursor = ic;
    refresh (ac, ic, cursor);
}

int main ()
{
    struct itimerval nval, oval;


    sc_memoryInit();
    sc_regInit();

    //for (int i = 0; i < 100; i++)
    //{
    //    sc_memorySet(i, i);
    //}

    sc_memorySet(0, 2581);
    sc_memorySet(1, 2582);
    sc_memorySet(2, 5141);
    sc_memorySet(3, 8214);
    sc_memorySet(4, 5399);
    sc_memorySet(5, 5142);
    sc_memorySet(6, 8471);
    sc_memorySet(7, 5400);
    sc_memorySet(8, 5141);
    sc_memorySet(9, 7960);
    sc_memorySet(10, 5401);
    sc_memorySet(11, 5142);
    sc_memorySet(12, 7706);
    sc_memorySet(13, 5397);
    sc_memorySet(14, 5145);
    sc_memorySet(15, 7706);
    sc_memorySet(16, 5398);
    sc_memorySet(17, 5142);
    sc_memorySet(18, 14082);
    sc_memorySet(19, 2837);
    sc_memorySet(20, 11008);

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
    int flag = 0;

    do
    {
        key = other;
        refresh(ac, ic, cursor);
        rk_mytermregime(0, 0, 1, 0, 0);
        rk_readkey(&key);

        if (key == r)
        {
            int Registr;
            sc_regGet(FLG_TICK_IGNORE, &Registr);

            if (Registr==0)
                sc_regSet(FLG_TICK_IGNORE, 1);
            else
                sc_regSet(FLG_TICK_IGNORE, 0);

            rk_mytermregime(0, 0, 1, 0, 1);
            nval.it_interval.tv_sec = 1;
            nval.it_interval.tv_usec = 0;
            nval.it_value.tv_sec = 1;
            nval.it_value.tv_usec = 0;

            signal(SIGALRM, ic_set);



            setitimer(ITIMER_REAL, &nval, &oval);
            //key = other;
            flag = 0;
            //Обработка прерываний
        }

        else if (key == i)
        {
            nval.it_interval.tv_sec = 0;
            nval.it_value.tv_sec = 0;
            setitimer(ITIMER_REAL, &nval, &oval);

            sc_memoryInit();
            sc_regInit();
            ac = 0;
            ic = 0;
            cursor = 0;
            flag = 1;
        }



        if (flag == 1)
        {

            if (key == F5)
            {

                //printf("\nInput ac: ");
                ++ac;

            }

            else if (key == F6)
            {
                ++ic;
                cursor = ic;

            }

            if (key == s)
            {
                char fname_mem[10], fname_term[10];
                printf("Input filename to save memory: ");
                scanf("%s", fname_mem);
                sc_memorySave(fname_mem);

                printf("\nInput filename to save terminal settings: ");
                scanf("%s", fname_term);
                rk_mytermsave(fname_term);
            }

            else if (key == l)
            {
                char fname_mem[10], fname_term[10];
                printf("\nInput filename to load memory: ");
                scanf("%s", fname_mem);
                sc_memoryLoad(fname_mem);

                printf("\nInput filename to load terminal settings: ");
                scanf("%s", fname_term);
                rk_mytermrestore(fname_term);
            }

            else if (key == enter)
            {
                int cell;
                printf("Insert cell value: ");
                scanf("%d", &cell);
                mem[cursor] = cell;
            }


            else if (key == left && cursor > 0)
                cursor -= 1;

            else if (key == right && cursor < 99)
                cursor += 1;

            else if (key == up && cursor > 9)
                cursor -= 10;

            else if (key == down && cursor < 90)
                cursor += 10;

            else if (cursor == 100)
            {
                ic_reset();
                rk_mytermregime(0, 0, 1, 0, 1);
                cursor = 0;
            }
        }
    }
    while (key != q);

    mt_clrscr();

    return 0;
}
