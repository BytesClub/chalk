/*********************************************
 * chalk.h                                   *
 * Header File Containing Predefined MACROS  *
 * and include files                         *
 *                                           *
 * Author: Progyan Bhattacharya              *
 * Maintainer: Bytes Club                    *
 *********************************************/

// Start Header File
#ifndef __chalk_h
#define __chalk_h

// Define Configuration
#define CHALK_VERSION "0.0.1"

// Set glibc Standards
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// Include Additional Headers
#ifndef __ctype_h
#include <ctype.h>
#endif
#ifndef __errno_h
#include <errno.h>
#endif
#ifndef __stdio_h
#include <stdio.h>
#endif
#ifndef __stdlib_h
#include <stdlib.h>
#endif
#ifndef __string_h
#include <string.h>
#endif
#ifndef __sys_types_h
#include <sys/types.h>
#endif
#ifndef __sys_ioctl_h
#include <sys/ioctl.h>
#endif
#ifndef __terminos_h
#include <termios.h>
#endif
#ifndef __unistd_h
#include <unistd.h>
#endif

// Define Enum and Structs

// Keyboard Codes
enum moveKey {
        Left_Arrow = 1000,
        Right_Arrow,
        Up_Arrow,
        Down_Arrow,
        Page_Up,
        Page_Down,
        Home_Key,
        End_Key,
        Delete_Key
};

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}

// Editor Row
typedef struct erow {
        int len;
        char *data;
} erow;

// Editor Config
struct editorConfi {
        int cx, cy;
        struct termios orig_termios;
        int screenrows;
        int screencols;
    	int no_row;
	    erow *row;
};

// Append Buffer
struct abuf {
        char *s;
        int len;
};

// Function Prototypes

// Append Buffer
void append(struct abuf*, char*, int);
void freebuf(struct abuf*);

// Terminal
int getWindowSize(int*, int*);
void die(const char*);
void disableRawMode(void);
void enableRawMode(void);
int editorReadKey(void);

// Row Operation
void appendrow(char*, int);

// File I/O
void openEditor(char*);

// Output
void drawRows(struct abuf*);
void editorRefreshScreen(void);

// Input
void moveCursor(int);
void editorProcessKeypress(void);

// Init Process
void initEditor(void);


#endif