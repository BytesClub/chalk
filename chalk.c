/* includes */

#include <unistd.h>
#include <stdlib.h>  
#include <termios.h> 
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/* defines */

#define CTRL_KEY(k) ((k) & 0x1f)

/* data */

struct termios orig_termios;

/* terminal */

void die(const char *S ) 
{
        perror(S);
        exit(1);
}

void disableRawMode()
{
        if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios) == -1) {
                die("tcsetattr");
        }
}

void enableRawMode()
{
        if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
                die("tcgetattr");
        }
        atexit(disableRawMode);

        struct termios raw=orig_termios; 
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

char editorReadKey()
{
        int nread;
        char c;
        while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
                if (nread == -1 && errno != EAGAIN)
                        die("read");
        }
        return c;
}

/* output */

void editorRefreshScreen()
{
        write(STDOUT_FILENO, "\x1b[2J", 4);
}

/* input */

void editorProcessKeypress()
{
        char c = editorReadKey();

        switch(c) {
                case CTRL_KEY('q'): 
                        exit(0);
                        break;
        }
}

/* init */

int main()
{
        enableRawMode();

        while(1) {
                editorRefreshScreen();
                editorProcessKeypress();
        }

        return 0;
}
