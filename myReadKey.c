#include "myReadKey.h"


int rk_readkey(keys* key)
{
    struct termios orig_options;
    char buf[16];
    int num_read;

    if (tcgetattr(STDIN_FILENO, &orig_options) != 0)
        return -1;

    if (rk_mytermregime(0, 0, 1, 0, 1) != 0)
        return -1;

    num_read = read(STDIN_FILENO, buf, 15);
    if (num_read < 0)
        return -1;

    buf[num_read] = '\0';

    if (strcmp(buf, "l") == 0)
        *key = l;
    else
    if (strcmp(buf, "s") == 0)
        *key = s;
    else
    if (strcmp(buf, "r") == 0)
        *key = r;
    else
    if (strcmp(buf, "t") == 0)
        *key = t;
    else
    if (strcmp(buf, "i") == 0)
        *key = i;
    else
    if (strcmp(buf, "q") == 0)
        *key = q;
    else
    if (strcmp(buf, "\n") == 0)
        *key = enter;
    else
    if (strcmp(buf, "\E[15~") == 0)
        *key = F5;
    else
    if (strcmp(buf, "\E[17~") == 0)
        *key = F6;
    else
    if (strcmp(buf, "\E[A") == 0)
        *key = up;
    else
    if (strcmp(buf, "\E[B") == 0)
        *key = down;
    else
    if (strcmp(buf, "\E[C") == 0)
        *key = right;
    else
    if (strcmp(buf, "\E[D") == 0)
        *key = left;
    else
        *key = other;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_options) != 0)
        return -1;

    return 0;
}

int rk_mytermsave(char* filename)
{
    struct termios options;
    FILE *save;

    if (tcgetattr(STDIN_FILENO, &options) != 0)
        return -1;

    if ((save = fopen(filename, "wb")) == NULL)
        return -1;

    fwrite(&options, sizeof(options), 1, save);
    fclose(save);

    return 0;
}

int rk_mytermrestore(char* filename)
{
    struct termios options;
    FILE *save;

    if ((save = fopen(filename, "rb")) == NULL)
        return -1;

    if (fread(&options, sizeof(options), 1, save) > 0)
    {
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &options) != 0)
            return -1;
    }
    else
        return -1;

    return 0;
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios options;

    if (tcgetattr(STDIN_FILENO, &options) != 0)
        return -1;

    if (regime == 1)
        options.c_lflag |= ICANON;
    else
    if (regime == 0)
        options.c_lflag &= ~ICANON;
    else
        return -1;
    if (regime == 0)
    {
        options.c_cc[VTIME] = vtime;
        options.c_cc[VMIN] = vmin;
        if (echo == 1)
            options.c_lflag |= ECHO;
        else
        if (echo == 0)
            options.c_lflag &= ~ECHO;
        else
            return -1;
        if (sigint == 1)
            options.c_lflag |= ISIG;
        else
        if (sigint == 0)
            options.c_lflag &= ~ISIG;
        else
            return -1;
    }
    if (tcsetattr(STDIN_FILENO, TCSANOW, &options) != 0)
        return -1;

    return 0;
}
