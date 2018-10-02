/* includes */
#include <chalk.h>

/* Editor Config Variable */
struct editorConfi E;

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
