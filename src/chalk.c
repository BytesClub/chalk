/* includes */
#include <chalk.h>

/* Editor Config Variable */
struct editorConfi E;


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

/* row operations */

void appendrow(char *c, int l)
{
        E.row = realloc(E.row, sizeof(erow) * (E.no_row + 1));

        int in = E.no_row;
        E.row[in].len = l;
        E.row[in].data = malloc(l + 1);
        memcpy(E.row[in].data, c, l);
        E.row[in].data[l] = '\0';
        E.no_row++;
}

/* file i/o */

void openEditor(char *fname)
{
        FILE *f = fopen(fname,"r");
        if(!f) {
                die("fopen");
        }
        char *val = NULL;
        size_t linecap = 0;
        ssize_t linelen;
       // linelen = getline(&val, &linecap, f);
        while((linelen = getline(&val, &linecap, f)) != -1) {
                while (linelen > 0 && (val[linelen - 1] == '\n' || val[linelen - 1] == '\r'))
                        linelen--;

                appendrow(val, linelen);
        }
        free(val);
        fclose(f);
}

/* output */

void drawRows(struct abuf *ab)
{
        int y;
        for(y = 0; y < E.screenrows; y++) {
                if(y >= E.no_row) {
                        if(E.no_row == 0) {
                                if (y == E.screenrows / 3) {
                                        char welcome[80];
                                        int welcomelen = snprintf(welcome, sizeof(welcome), "Chalk editor --version %s", CHALK_VERSION);

                                        if (welcomelen > E.screencols)
                                                welcomelen = E.screencols;

                                        int padding = (E.screencols - welcomelen) / 2;
                                        if (padding) {
                                                append(ab, "~", 1);
                                                padding--;
                                        }

                                        while (padding--)
                                                append(ab, " ", 1);
                                        append(ab, welcome, welcomelen);
                                } else if (y == (E.screenrows / 3) + 1) {
                                        char welcome[80];
                                        int welcomelen = snprintf(welcome, sizeof(welcome), "by Rudra Nil Basu and Satyam Kumar");

                                        if (welcomelen > E.screencols)
                                                welcomelen = E.screencols;

                                        int padding = (E.screencols - welcomelen) / 2;
                                        if (padding) {
                                                append(ab, "~", 1);
                                                padding--;
                                        }

                                        while (padding--)
                                                append(ab, " ", 1);
                                        append(ab, welcome, welcomelen);
                                } else if (y == (E.screenrows / 3) + 4) {
                                        char welcome[80];
                                        int welcomelen = snprintf(welcome, sizeof(welcome), "Bytes Club");

                                        if (welcomelen > E.screencols)
                                                welcomelen = E.screencols;

                                        int padding = (E.screencols - welcomelen) / 2;
                                        if (padding) {
                                                append(ab, "~", 1);
                                                padding--;
                                        }

                                        while (padding--)
                                                append(ab, " ", 1);
                                        append(ab, welcome, welcomelen);
                                } else {
                                        append(ab, "~", 1);
                                }
                        } else {
                                append(ab, "~", 1);
                        }
                } else {
		int len = E.row[y].len;
		if(len > E.screencols ) {
			len = E.screencols;
		}
		append(ab, E.row[y].data, len);
	}

                append(ab, "\x1b[K", 3);
                if(y < E.screenrows - 1) {
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

        char buf[32];
        snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
        append(&ab, buf, strlen(buf));

        append(&ab, "\x1b[?25h", 6); /* Show cursor after refresh */
        write(STDOUT_FILENO, ab.s, ab.len);

        freebuf(&ab);
}

/* input */

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

/* init */

void initEditor()
{
        E.cx = 0, E.cy = 0, E.no_row = 0, E.row = NULL;
        if(getWindowSize(&E.screenrows,&E.screencols) == -1) {
                die("getWindowSize");
        }
}

int main(int argc, char *argv[])
{
        enableRawMode();
        initEditor();
        if (argc >= 2) {
                openEditor(argv[1]);
        }

        while(1) {
                editorRefreshScreen();
                editorProcessKeypress();
        }

        return 0;
}
