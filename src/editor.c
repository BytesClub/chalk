/**
 * editor.c
 * Functions related to Editor and Editor rows
 */

// include
#include <chalk.h>

// global variable
extern struct editorConfi E;

// row operations
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

// file i/o
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