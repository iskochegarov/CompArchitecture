#include <fcntl.h>
#include "BigChars.h"
#include "myGUI.h"

#define DUMP_FILENAME "memory.dump"


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
