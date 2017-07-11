#include <unistd.h>
#include <stdlib.h>  
#include <termios.h> 
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

void disableRawMode()
{
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios);
}

void enableRawMode()
{
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw=orig_termios; 
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_oflag &= ~(OPOST);
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_cflag |= (CS8);
	raw.c_cc[VMIN] = 0; /* Return each byte, or 0 on timeout */
	raw.c_cc[VTIME] = 1; /* 100 ms timeout */

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main()
{
	enableRawMode();

	while(1) {
		char c = '\0';
		read(STDIN_FILENO, &c, 1);
		if(iscntrl(c)) {
			printf("%d\r\n",c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}

		if (c == 'q')
			break;
	}

	return 0;
}
