/**
 * terminal.c
 * Functions related to Console IO operation
 */

// include
#include <chalk.h>

// global variable
extern struct editorConfi E;

int getWindowSize(int *row,int *col)
{
        struct winsize ws;

        if( ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 && ws.ws_col == 0) {
                return -1;
        } else {
                *row = ws.ws_row;
                *col = ws.ws_col;
                return 0;
       }
}

void die(const char *S )
{
        perror(S);
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO,"\x1b[H", 3);
        exit(1);
}

void disableRawMode()
{
        if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
                die("tcsetattr");
        }
}

void enableRawMode()
{
        if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) {
                die("tcgetattr");
        }
        atexit(disableRawMode);

        struct termios raw = E.orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
        raw.c_oflag &= ~(OPOST);
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_cflag |= (CS8);
        raw.c_cc[VMIN] = 0; /* Return each byte, or 0 on timeout */
        raw.c_cc[VTIME] = 1; /* 100 ms timeout for read() */

        if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
                die("tcsetattr");
        }
}

int editorReadKey()
{
        int nread;
        char c;
        while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
                if (nread == -1 && errno != EAGAIN)
                        die("read");
        }
        if(c == '\x1b') {
                char buf [4];
                if(read(STDIN_FILENO, &buf[0], 1) == -1) {
                        return c;
                }
                if(read(STDIN_FILENO, &buf[1], 1) == -1) {
                        return c;
                }
                if( buf[0] == '[') {
                        if( buf[1] <= '9' && buf[1] >= '0') {
                                if(read(STDIN_FILENO, &buf[2], 1) == -1) {
                                        return c;
                                }
                                if( buf[2] == '~') {
                                        switch(buf[1]) {
                                                case '1' :
                                                        return Home_Key;
                                                case '3' :
                                                        return Delete_Key;
                                                case '4' :
                                                        return End_Key;
                                                case '5' :
                                                        return Page_Up;
                                                case '6' :
                                                        return Page_Down;
                                                case '7' :
                                                        return Home_Key;
                                                case '8' :
                                                        return End_Key;
                                        }
                                }
                        } else {
			        switch (buf[1]) {
                                case 'A' :
                                        return Up_Arrow;
                                case 'B' :
                                        return Down_Arrow;
                                case 'C' :
                                        return Right_Arrow;
                                case 'D' :
                                        return Left_Arrow;
                                case 'H' :
                                        return Home_Key;
                                case 'F' :
                                        return End_Key;
                                }
                        }
                } else if ( buf[0] == 'O') {
                        switch(buf[1]) {
                                case 'H' :
                                        return Home_Key;
                                case 'F' :
                                        return End_Key;
                        }
                }
                return c;
        }
        return c;
}