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
	raw.c_iflag &= ~(ICRNL | IXON);	
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main()
{
	enableRawMode();   
	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if(iscntrl(c)) {
			printf("%d\n",c);
		} else {
			printf("%d ('%c')\n", c, c);
		}
	}
	return 0;
}
