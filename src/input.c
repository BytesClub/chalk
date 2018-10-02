/**
 * input.c
 * Functions related to processing Keyboard input
 */

// include
#include <chalk.h>

// global variable
extern struct editorConfi E;

void moveCursor(int key)
{
        switch(key) {
                case Left_Arrow :
                        if(E.cx != 0) {
                                E.cx--;
                        }
                        break;
                case Right_Arrow :
                        if(E.cx != (E.screencols - 1)) {
                                E.cx++;
                        }
                        break;
                case Up_Arrow :
                        if(E.cy != 0) {
                                E.cy--;
                        }
                        break;
                case Down_Arrow :
                        if(E.cy != (E.screenrows - 1)) {
                                E.cy++;
                        }
                        break;
        }
}

void editorProcessKeypress()
{
        int c = editorReadKey();

        switch(c) {
                case CTRL_KEY('q'):
                        write(STDOUT_FILENO, "\x1b[2J", 4);
                        write(STDOUT_FILENO, "\x1b[H", 3);
                        exit(0);
                        break;
                case Page_Up :
                case Page_Down :
                {
                        int pwc = E.screenrows;
                        while(pwc--) {
                                moveCursor(c == Page_Up ? Up_Arrow : Down_Arrow);
                        }
                }
                case Home_Key :
                        E.cx = 0;
                        break;
                case End_Key :
                        E.cx = (E.screencols - 1);
                        break;
                case Left_Arrow :
                case Up_Arrow :
                case Down_Arrow :
                case Right_Arrow :
                         moveCursor(c);
                         break;
        }
}