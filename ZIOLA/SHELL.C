#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <direct.h>

main()

{
	char savdcolr, lbuf[2];
	char cmdcom[60];

	/*
	*cmdcom = '\0';
	strcpy(cmdcom, getenv("COMSPEC"));



	if (!*cmdcom)
	    strcpy(cmdcom, "COMMAND.COM");
	*/

	chdir("c:\\ziola\\files\\1");

	strcpy(cmdcom, "dsz.exe");

	spawnlp(P_WAIT, cmdcom, cmdcom, NULL); /* spawn COMMAND.COM */

	printf("dadsadsa");
}
