/* includes */
#include <chalk.h>

/* Editor Config Variable */
struct editorConfi E;

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
