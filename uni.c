#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include "getkey.h"
#include "tos.h"

static int COLS=80, ROWS=25;

static void draw_legend()
{
    int x, y;
    printf("\e[0m\e[2J");
    
    for (y=0; y<16; y++)
    {
        printf("\e[%d;1f\e[0;34%sm", y+5, (y&1)?";1":"");
        for (x=0; x<COLS; x++)
            printf("%C", 0x2588);
    }
}

static char hexdigits[]="0123456789ABCDEF";

static void draw_table(int c0)
{
    int x, y;
    wchar_t c;
    char label[12];
    
    printf("\e[0;30;47m");
    for (y=0; y<16; y++)
        printf("\e[%d;%df%c", y+5, 1, hexdigits[(y+c0)&0xf]);
    for (x=0; x<COLS/2-1; x++)
    {
        snprintf(label, sizeof(label), "%4X", ((c0>>4)+x)&0xFffff);
        if ((c0>>4)+x>=0x100000)
            label[0]='G';
        for (y=0; y<4; y++)
            if (label[y])
                printf("\e[%d;%df%c", y+1, 2*x+3, label[y]);
    }
    
    for (x=0; x<COLS/2-1; x++)
        for(y=0; y<16; y++)
        {
            c=c0+x*16+y;
            
            printf("\e[0m\e[%d;%df", y+5, 2*x+3);
            
            switch(wcwidth(c))
            {
            case 1:
                printf("%C", c);
                printf("\e[0;34%sm%C", (y&1)?";1":"", 0x2588);
                break;
            case 2:
                printf("%C", c);
                break;
            default:
                printf("\e[0;34%sm%C%C", (y&1)?";1":"", 0x2588, 0x2588);
            }
        }
    
    printf("\e[%d;1f", ROWS);
    fflush(stdout);
}

int main()
{
    int c0;
    
    setlocale(LC_CTYPE, "");
    kbd_raw();
    
    draw_legend();
    
    c0=32;
    while(1)
    {
        draw_table(c0);
        
        switch(getkey(0))
        {
        case KEY_HOME:
        case KEY_HOME_ALT:
        case KEY_F1:
        case KEY_F1_ALT:
        case KEY_KPAD_NUMLOCK:
            c0 = 32;
            break;
        case KEY_F2:
        case KEY_F2_ALT:
        case KEY_KPAD_DIV:
            c0 = 0x2000;
            break;
        case KEY_F3:
        case KEY_F3_ALT:
        case KEY_KPAD_MULT:
            c0 = 0x2600;
            break;
        case KEY_F4:
        case KEY_F4_ALT:
        case KEY_KPAD_MINUS:
            c0 = 0x1d400;
            break;
        case KEY_KPAD9:
        case KEY_PGUP:
        case KEY_PGUP_ALT:
            c0 -= (COLS/2-1)*16;
            if (c0<32)
                c0=32;
            break;
        case KEY_KPAD3:
        case KEY_PGDN:
        case KEY_PGDN_ALT:
        case ' ':
            c0 += (COLS/2-1)*16;
            break;
        case 'q':
        case 'Q':
        case KEY_ESC:
            kbd_restore();
            exit(0);
        }
    }
    
    return 0;
}
