/* includes */

#include <unistd.h>
#include <stdlib.h>  
#include <termios.h> 
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

/* defines */

#define CTRL_KEY(k) ((k) & 0x1f)

/* data */
struct editorConfi {
        struct termios orig_termios;
        int screenrows;
        int screencols;
};

struct editorConfi E;

/* append buffer */

struct abuf {
        char *s;
        int len;
};

#define ABUF_INIT {NULL, 0}

void append( struct abuf *ab, char *C, int l)
{
        char *new = realloc(ab->s, ab->len + l);
        if(new == NULL) return;
        memcpy(&new[ab->len], C, l);
        ab->s = new;
        ab->len += l;
}

void freebuf(struct abuf *ab)
{
        free(ab->s);
}

/* terminal */

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

void drawRows(struct abuf *ab)
{
        int i;
        for(i = 0; i < E.screenrows; i++) {
                append(ab, "~", 1);

                append(ab, "\x1b[K", 3);
                if(i < E.screenrows - 1) {
                        append(ab, "\r\n", 2);
                }
        }
}

void editorRefreshScreen()
{
        struct abuf ab = ABUF_INIT;
        append(&ab, "\x1b[?25l", 6); /* Hide the cursor before refresh */
        append(&ab, "\x1b[H", 3);

        drawRows(&ab);

        append(&ab, "\x1b[H", 3);
        append(&ab, "\x1b[?25h", 6); /* Show cursor after refresh */
        write(STDOUT_FILENO, ab.s, ab.len);

        freebuf(&ab);
}

/* input */

void editorProcessKeypress()
{
        char c = editorReadKey();

        switch(c) {
                case CTRL_KEY('q'): 
                        write(STDOUT_FILENO, "\x1b[2J", 4);
                        write(STDOUT_FILENO, "\x1b[H", 3);
                        exit(0);
                        break;
        }
}

/* init */

void initEditor()
{
        if(getWindowSize(&E.screenrows,&E.screencols) == -1) {
                die("getWindowSize");
        }
}

int main()
{
        enableRawMode();
        initEditor();

        while(1) {
                editorRefreshScreen();
                editorProcessKeypress();
        }

        return 0;
}
