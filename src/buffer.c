/**
 * buffer.c
 * Method related to buffer storage
 */

// include base header
#include <chalk.h>

void append(struct abuf *ab, char *C, int l)
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