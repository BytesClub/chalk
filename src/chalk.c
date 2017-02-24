#include<stdio.h>
#include<stdlib.h>

int open(char* filename)
{
	//
}

int main(int argc, char **argv)
{
	if(argc!=2) {
		fprintf(stderr, "ERROR: Specify filename correctly\n");
		exit(1);
	}
	open(argv[1]);

	while(1) {
		/*
		 * Refresh screen
		 * Process Key Press
		 * */
	}
	return 0;
}
