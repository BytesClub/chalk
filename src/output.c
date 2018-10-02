/**
 * output.c
 * Functions related to formatting output screen
 */

// include
#include <chalk.h>

// global variable
extern struct editorConfi E;

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