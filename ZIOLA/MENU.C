#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "serial.h"
#include "dansi.h"
#include "mess.h"
#include "menu.h"
#include "misc.h"
#include "file.h"
#include "ginput.h"
#include "rmess.h"
#include "wmess.h"
#include "modem.h"
#include "main.h"

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void men(unsigned char fl[100]) {

	unsigned char menu_def[161], temp, counter = 2, cnt = 0, pass = 0;
	unsigned char options_c = 0;

	unsigned char scratch[81];
	unsigned int security;

	FILE *fp;

	strcpy(scratch, menu_path);
	strcat(scratch, fl);
	strcat(scratch, ".mnu");

	if ((fp = fopen(scratch, "r")) == NULL) {
		printf("cannot open %s\n", scratch);
		exit(1);
	}

	while (!feof(fp)) {

		cnt = 0;
		counter = 2;

		if (fgets(menu_def, 160, fp) != NULL) {
			temp = menu_def[0];

			while (menu_def[counter] != ',')
				scratch[cnt++] = menu_def[counter++];

			scratch[cnt] = '\0';
			security = atoi(scratch);

			if (atoi(user.secur) >= security) {
				mess_options[options_c++] = temp;
				menu[pass].key = temp;
				menu[pass].security = security;
				cnt = 0;
				counter++;

				while (menu_def[counter] != ',')
					scratch[cnt++] = menu_def[counter++];

				scratch[cnt] = '\0';

				strcpy(menu[pass].display, scratch);
				cnt = 0;
				counter++;

				while (menu_def[counter] != ',')
					scratch[cnt++] = menu_def[counter++];

				scratch[cnt] = '\0';

				strcpy(menu[pass].password, scratch);

				cnt = 0;
				counter++;
				while (menu_def[counter] != ',')
					scratch[cnt++] = menu_def[counter++];

				scratch[cnt] = '\0';
				strcpy(menu[pass].command, scratch);
				cnt = 0;

				counter++;

				while (menu_def[counter] != '\n')
					scratch[cnt++] = menu_def[counter++];

				scratch[cnt] = '\0';

				menu[pass].function = atoi(scratch);

				pass++;
			}
		}
	}

	mess_options[options_c] = '\0';
	fclose(fp);

	strcpy(scratch, menu_path);
	strcat(scratch, fl);
	get_menu(scratch, 0);

	strcpy(options, mess_options);
	check_time();
	hot_of = ON;
	rfile(menu_file, 0);
	hot_of = OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned int get_funct() {

	unsigned char c = 0, flag = ON;
	int menu_num, i;
	unsigned int counter = 0;

	strcpy(options, mess_options);

	if (!key_pressed) {

		while (flag == ON) {
			c = hot_key();

			if (c == 13) {
				return 999;
			} else
				flag = OFF;
		}
		flag = ON;
	} else {
		c = key_pressed;
	}

	while (flag) {
		if (menu[counter].key == c) {
			return counter;
		}
		counter++;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_menu(unsigned char *filename, unsigned char type) {

	unsigned char on_disk, flag = ON, tmp;
	unsigned char reserve[60], partial[60];

	tmp = (unsigned char)atoi(user.ansi);

	strcpy(reserve, filename);

	if (!type) {
		strcat(reserve, "-");
		strcat(reserve, user.secur);
	}

	strcpy(partial, reserve);

	while (flag) {
		if (tmp == 1)
			strcat(reserve, ".asc");
		else if (tmp == 2)
			strcat(reserve, ".an1");
		else if (tmp == 3)
			strcat(reserve, ".an2");

		on_disk = file_exist(reserve);

		if (on_disk) {
			strcpy(reserve, partial);
			tmp--;

			if (!tmp) {
				flag = OFF;
				printf("\nAnsi/Ascii file not made for this menu.");
				exit(1);
			}
		} else {

			if (!type)
				strcpy(menu_file, reserve);
			else
				strcpy(title_file, reserve);

			flag = OFF;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void check_time() {

	double diff, convert, diff2;

	time(&new_time);

	diff2 = difftime(new_time, time_out_start);
	diff = difftime(new_time, start_time);

	if ((long int)diff2 / 60L >= 3L) {
		p("[0;1;33;40m", "\n\nYou are taking too long to respond, logging you off...\n");
		if (valid_user)
			log_out();
		longjmp(state, -1);
	}

	convert = (double)((long int)time_allowed - (long int)diff / 60);

	if (convert < 0)
		convert = 0;

	ltoa((long int)convert, display_time, 10);

	if (convert <= 0) {
		p("[0;1;35;40m", "\n\nSorry, your time is up, call back after ");
		p("[32m", "midnight");
		p("[37m", ".");

		if (valid_user)
			log_out();
		longjmp(state, -1);
	}
}
