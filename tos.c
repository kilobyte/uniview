#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "tos.h"

int COLS=80, ROWS=25;

static struct termios old_tattr;

void kbd_raw()
{
    struct termios tattr;

    if (!isatty(0))
        return;

    tcgetattr(0,&old_tattr);
    tattr=old_tattr;
    /* cfmakeraw(&tattr); */
    tattr.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP
                       |INLCR|IGNCR|ICRNL|IXON);
    tattr.c_oflag &= ~OPOST;
    tattr.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    tattr.c_cflag &= ~(CSIZE|PARENB);
    tattr.c_cflag |= CS8;

#ifndef IGNORE_INT
    tattr.c_lflag|=ISIG;        /* allow C-c, C-\ and C-z */
#endif
    tattr.c_cc[VMIN]=1;
    tattr.c_cc[VTIME]=0;
    tcsetattr(0,TCSANOW,&tattr);
}

void kbd_restore()
{
    tcdrain(0);
    tcsetattr(0,TCSADRAIN,&old_tattr);
}

void tty_get_size()
{
    struct winsize ts;

    if (ioctl(1,TIOCGWINSZ,&ts) || ts.ws_row<=0 || ts.ws_col<=0)
    {   /* not a terminal or a broken one*/
        COLS=80;
        ROWS=25;
        return;
    }
    COLS=ts.ws_col;
    ROWS=ts.ws_row;
}

void tty_setup_winch()
{
    struct sigaction sa;
    sa.sa_handler = tty_get_size;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags   = SA_RESTART;
    sigaction(SIGWINCH, &sa, 0);
}
