#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <wchar.h>
#include <string.h>
#include "getkey.h"

#define GETKEY				\
        if (read(in, &ch, 1)!=1)	\
            return EOF;			\
        switch(ch)
int getkey(int in)
{
    struct pollfd pfd;
    unsigned char ch, k;
    mbstate_t ps;
    wchar_t wc;
    
    memset(&ps, 0, sizeof(ps));    
more:
    GETKEY
    {
    case 27:
        pfd.fd=in;
        pfd.events=POLLIN;
        if (!poll(&pfd, 1, 500))
            return 27; /* lone ESC */
        /* errors on poll are ok */
        GETKEY
        {
        case 'O':
            GETKEY{}
            /* ESC O A */
            return -0x200+ch;
        case '[':
            GETKEY
            {
            case 'O':
                GETKEY{}
                /* ESC [ O A */
                return -0x300+ch;
            case '[':
                GETKEY{}
                /* ESC [ [ A */
                return -0x400+ch;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                k=ch-'0';
                while(ch>='0' && ch<='9')
                {
                    k=k*10+ch-'0';
                    GETKEY{}
                }
                if (ch=='~')
                    return -0x600+k;	/* ESC [ 11 ~ */
                else
                    return -0x700+k;	/* ESC [ 11 invalid */
            /* ESC [ A */
            }
            return -0x500+ch;
        }
        return -0x100+ch;	/* ESC A -- usually alt+A */
    }
    if ((int)mbrtowc(&wc, (char*)&ch, 1, &ps)<0)
        goto more;
    return wc;
}
