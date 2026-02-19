#define __MSC

#include <stdio.h>
#include <ctype.h>
#include <bios.h>   /*keyboard*/
#include <string.h> /*string functions*/
#include <stdlib.h> /*convert atoi etc */
#include <time.h>
#include <io.h>
#include <setjmp.h>
#include <dos.h>

#include "serial.h"
#include "dansi.h"
#include "mess.h"
#include "menu.h"
#include "misc.h"
#include "file.h"
#include "ginput.h"
#include "rmess.h"
#include "wmess.h"
#include "main.h"
#include "modem.h"

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void cut_string() {

	char *ch;
	register col = 0, row = 0;
	unsigned char flag = 0;

	ch = response;

	while (*ch != '\0') {
		if (flag == 0) {
			cut[col].level[row] = *ch++;
			row++;
		}

		if (*ch == 32 || *ch == ',' || *ch == ';') {
			ch++;
			if (flag == 0) {
				cut[col].level[row] = '\0';
				col++;
				row = 0;
			}
			flag = 1;
		} else
			flag = 0;
	}

	cut[col].level[row] = '\0';
	row = 0;
	col++;
	cut[col].level[row] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void strip_blanks() {

	char tem[80];
	char *ch = response;
	int count = 0;
	unsigned char flag = 0;

	while (*ch != '\0') {
		while (*ch == 32 && flag == 0 || *ch == ',' && flag == 0 || *ch == ';' && flag == 0)
			ch++;
		flag = 1;
		tem[count] = *ch++;
		count++;
	}

	tem[count] = '\0';
	strcpy(response, tem);
	count = (strlen(response)) - 1;
	while (response[count] == 32)
		count--;
	response[++count] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_string(unsigned char length, char out) {

	int counter = 0;
	unsigned char c = ON;
	char up;

	clear_input();

	while (c != 13) {
		c = 0;

		if (counter == 0)
			up = 1;
		while (c == 0)
			c = (unsigned char)inkey();

		if (!is_valid(c)) {
		} else if (c == 9) {
		} else if (c == 27) {
		} else if (c == 8 && counter > 0) {
			counter--;
			delete (1);
			response[counter] = 32;
			if (response[counter - 1] == 32 || counter < 0 || response[counter - 1] == ',' || response[counter - 1] == ';')
				up = 1;
			else
				up = 0;
		}

		else if (counter < length && c != 8 && c != 13) {
			if (up == 1)
				c = upper(c);
			else
				c = lower(c);

			if (c == 32 || c == ',' || c == ';')
				up = 1;
			else
				up = 0;
			response[counter++] = c;
			if (out) {
				send[0] = 'X';
				p("", send);
			} else {
				send[0] = c;
				p("", send);
			}
		}
	}

	response[counter] = '\0';
	strip_blanks();
	cut_string();
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

int inkey() {

	union scan {
		int c;
		char ch[2];
	} sc;

	if (!(carrier()) && !local) {
		if (valid_user)
			log_out();
		longjmp(state, -1);
	}

	if (_bios_keybrd(_KEYBRD_READY) > 0) {
		user_input = OFF;
		sc.c = _bios_keybrd(_KEYBRD_READ);
		time(&time_out_start);

		if (sc.ch[0] == 0) {
			sc.ch[1] = (char)check_keys((int)sc.ch[1]);
			if (chat_flag) {
				chat_flag = OFF;
				return (int)13;
			}

			return sc.ch[1];
		} else
			return sc.ch[0];
	} else if (local == 0) {
		user_input = ON;

		if (in_ready()) {

			time(&time_out_start);
			sc.c = get_serial();
		} else
			sc.c = NULL;

		if (sc.ch[0] != NULL) {

			return sc.ch[0];
		}
	}

	if (!hot_of) {
		check_time();
	}
	return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void clear_input() {

	register int i;

	for (i = 0; i < 79; i++)
		response[i] = ' ';
	response[0] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void write_str(char *ch, int x, int y, int atrib) {

	while (*ch)
		write_char(x, y++, *ch++, atrib, 0);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char upper(unsigned char c) {

	if (islower(c))
		c = c - 32;
	return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char lower(unsigned char c) {

	if (isupper(c))
		c = c + 32;
	return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char get_yn(char *in) {

	unsigned char c = 0;

	while (c == 0)
		c = (unsigned char)inkey();
	c = upper(c);
	if (c == in[1]) {
		send[0] = in[1];
		p("", send);
		return in[1];
	} else if (mistake && c == 13) {
		return (unsigned char)1;
	} else {
		send[0] = in[0];
		p("", send);
		return in[0];
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_phone() {

	unsigned char c = 0;
	char flag = ON;
	unsigned char counter = 0;

	clear_input();
	p("", "[");

	while (flag) {
		c = 0;

		while (c == 0)
			c = (unsigned char)inkey();

		if (!is_valid(c)) {
		} else if (c == 27) {
		}

		else if (c == 8 && counter > 0) {
			counter--;
			if (counter == 2 || counter == 5) {
				delete (4);
			} else
				delete (1);
		} else if (isdigit(c)) {
			send[0] = c;
			p("", send);
			response[counter] = c;
			counter++;
			if (counter == 3 || counter == 6)
				p("", "]-[");

			else if (counter == 10) {
				p("", "]");
				flag = OFF;
			}
		} else if (c == 13)
			flag = OFF;
	}
	response[counter] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void delete (unsigned char number) {

	unsigned char a;
	unsigned char del[2];

	del[0] = (char)8;
	del[1] = '\0';

	for (a = 0; a < number; a++) {

		if (local == 0)
			put_serial((unsigned char)8);
		yloc--;
		goto_xy(xloc + 1, yloc);

		p("", " ");

		if (local == 0)
			put_serial((unsigned char)8);

		yloc--;

		goto_xy(xloc + 1, yloc);
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_date() {

	unsigned char c = 0;
	char flag = ON;
	unsigned char counter = 0;

	clear_input();
	p("", "[");

	while (flag) {
		c = 0;

		while (c == 0)
			c = (unsigned char)inkey();

		if (!is_valid(c)) {
		} else if (c == 27) {
		}

		else if (c == 8 && counter > 0) {
			counter--;
			response[counter] = 32;
			if (counter == 1 || counter == 3 || counter == 5) {
				delete (2);
			} else
				delete (1);
		} else if (isdigit(c)) {
			send[0] = c;
			p("", send);
			response[counter] = c;
			counter++;
			if (counter == 2 || counter == 4)
				p("", "-");

			else if (counter == 6) {
				p("", "]");
				flag = OFF;
			}
		} else if (c == 13)
			flag = OFF;
	}
	response[counter] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_number(unsigned char length, unsigned char in[80]) {

	unsigned char c = 0;
	unsigned char flag = ON;
	unsigned char counter = 0;

	if (strlen(in) > 0) {
		strcpy(response, in);
		counter = (unsigned char)strlen(in);
		p("", response);
	} else
		clear_input();

	while (flag) {
		c = 0;

		while (c == 0)
			c = (unsigned char)inkey();

		if (isdigit(c) && counter < length) {
			response[counter++] = c;
			send[0] = c;
			p("", send);
		} else if (c == 8 && counter > 0) {
			delete (1);
			response[counter--] = 32;
		} else if (c == 13)
			flag = OFF;
	}
	response[counter] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void m_or_f() {

	p("[36;40m", "\n\nMale or Female ");
	p("[34m", "[");
	p("[35m", "M/f");
	p("[34m", "]");
	p("[36m", ": ");
	p("[37;44m", " ");
	p("[1D", "");

	response[0] = get_yn("MF");
	response[1] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void voice() {
	char flag = ON;

	while (flag) {
		p("[33;40m", "\n\nPlease enter your ");
		p("[37m", "VOICE ");
		p("[33m", "phone number ");
		p("[34m", "[");
		p("[35m", "###");
		p("[34m", "]-[");
		p("[35m", "###");
		p("[34m", "]-[");
		p("[35m", "####");
		p("[34m", "]");
		p("[33m", ": ");

		if (user.ansi[0] != '1') {
			p("[37;44m", "                  ");
			p("[18D", "");
		}
		get_phone();

		if (strlen(response) == OFF && mistake == ON)
			flag = OFF;
		else if (strlen(response) < 10)
			p("[31;40m", "\n\nYou must enter your phone number.");
		else
			flag = OFF;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void data() {
	char flag = ON;

	while (flag) {
		p("[32;40m", "\n\nPlease enter your ");
		p("[37m", "DATA ");
		p("[32m", "phone number ");
		p("[34m", "[");
		p("[35m", "###");
		p("[34m", "]-[");
		p("[35m", "###");
		p("[34m", "]-[");
		p("[35m", "####");
		p("[34m", "]");
		p("[32m", ": ");
		if (user.ansi[0] != '1') {
			p("[37;44m", "                  ");
			p("[18D", "");
		}
		get_phone();

		if (strlen(response) == OFF && mistake == ON)
			flag = OFF;
		else if (strlen(response) < 10)
			p("[36;40m", "\n\nYou must enter your phone number.");
		else
			flag = OFF;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void bdate() {

	char flag = ON;
	int value;
	int value2;

	char trim[3];
	trim[2] = '\0';

	while (flag) {

		p("[31;40m", "\n\nPlease enter your birthdate ");
		p("[34m", "[");
		p("[35m", "MM-DD-YY");
		p("[34m", "]");
		p("[31m", ": ");
		if (user.ansi[0] != '1') {
			p("[37;44m", "          ");
			p("[10D", "");
		}
		get_date();

		if (strlen(response) == OFF && mistake == ON)
			flag = OFF;
		else if (strlen(response) < 6)
			p("[33;40m", "\n\nYou must enter your birth date.");
		else {
			trim[0] = response[0];
			trim[1] = response[1];

			value = atoi(trim);
			trim[0] = response[2];
			trim[1] = response[3];

			value2 = atoi(trim);

			if (value > 12 || value < 1)
				p("[33;40m", "\n\nMonth must be between 1 and 12.");
			else if (value2 > 31 || value2 < 1)
				p("[33;40m", "\n\nDay must be between 1 and 31.");
			else
				flag = OFF;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void screen_length() {

	char flag = ON;

	int value;

	while (flag) {
		p("[36;40m", "\n\nPlease enter your screen length ");
		p("[34m", "[");
		p("[35m", "15-500");
		p("[34m", "]");
		p("[36m", ": ");

		if (user.ansi[0] != '1') {
			p("[37;44m", "   ");
			p("[3D", "");
		}

		if (!mistake)
			get_number(3, "24");
		else
			get_number(3, "");

		value = atoi(response);

		if (strlen(response) == OFF && mistake == ON)
			flag = OFF;
		else if (value < 15 || value > 500)
			p("[32;40m", "\n\nScreen length must be between 15 and 500.");
		else
			flag = OFF;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void screen_clear() {

	p("[33;40m", "\n\nWould you like screen clearing ");
	p("[34m", "[");
	p("[35m", "Y/n");
	p("[34m", "]");
	p("[33m", ": ");

	if (user.ansi[0] != '1') {
		p("[37;44m", " ");
		p("[1D", "");
	}
	response[0] = get_yn("YN");
	response[1] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void password() {

	char flag = ON;

	char pass1[11];
	char pass2[11];

	while (flag) {
		p("[32;40m", "\n\nPlease select a password ");
		p("[34m", "[");
		p("[35m", "X's will echo");
		p("[34m", "]");
		p("[32m", ": ");
		if (user.ansi[0] != '1') {
			p("[37;44m", "          ");
			p("[10D", "");
		}
		get_string(10, 1);

		if (strlen(response) == 0 && mistake == ON)
			flag = OFF;
		else if (strlen(response) == 0)
			p("[36;40m", "\n\nYou must enter a password.");
		else if (strlen(response) < 4)
			p("[36;40m", "\n\nPassword must be 4 characters or longer.");
		else {
			strcpy(pass1, response);
			while (flag) {
				p("[31;40m", "\n\nPlease enter your password again to verify ");
				p("[34m", "[");
				p("[35m", "X's will echo");
				p("[34m", "]");
				p("[31m", ": ");

				if (user.ansi[0] != '1') {
					p("[37;44m", "          ");
					p("[10D", "");
				}
				get_string(10, 1);
				flag = OFF;
			}
			strcpy(pass2, response);

			if (strcmp(pass2, pass1)) {
				p("[36;40m", "\n\nIncorrect, try again.");
				flag = ON;
			}
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_name(void) {

	char flag = 1;

	while (flag) {
		p("[0;1;33;40m", "\n\nEnter your first and last name: ");

		if (user.ansi[0] != '1') {
			p("[34;44m", "                                        ");
			p("[40D", "");
		}
		p("[0;1;37;44m", "");

		get_string(40, 0);

		if (cut[0].level[0] == '\0' && cut[1].level[0] == '\0' && mistake == ON)
			flag = OFF;
		else if (cut[1].level[0] == '\0')
			p("[31;40m", "\n\nFirst and last name must be entered.");
		else if (strlen(cut[0].level) < 2 || strlen(cut[1].level) < 2)
			p("[31;40m", "\n\nA longer name must be used.");
		else
			flag = 0;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_city(void) {

	char flag = 1;

	while (flag) {
		p("[1;31;40m", "\n\nEnter you city and province ");
		p("[34m", "[");
		p("[35m", "ie: Cambridge, On");
		p("[34m", "]");
		p("[31m", ": ");
		if (user.ansi[0] != '1') {
			p("[34;44m", "                              ");

			p("[30D", "");
		}
		p("[37;44m", "");

		get_string(30, 0);

		if (cut[1].level[0] == '\0' && mistake == ON)
			flag = OFF;
		else if (cut[1].level[0] == '\0')
			p("[33;40m", "\n\nCity and province must be entered.");
		else if (strlen(cut[0].level) < 2)
			p("[33;40m", "\n\nA longer name must be used.");
		else if (strlen(cut[1].level) < 2)
			p("[33;40m", "\n\nProvince name should be longer.");
		else
			flag = 0;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void status_line(void) {

	char ontop[81];
	strcpy(ontop, "                                                                                ");
	/*draw_border(20,0,24,79,30,1,72);*/
	write_str(ontop, 0, 0, 79);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void g_date() {

	struct tm *local;
	time_t t;
	char pm = 0;
	char day[3], month[3], year[3];
	char hour[3], min[3];

	s_date[0] = '\0';
	s_time[0] = '\0';
	t = time(NULL);
	local = localtime(&t);

	itoa(local->tm_mday, day, 10);
	itoa((local->tm_mon) + 1, month, 10);
	itoa(local->tm_year, year, 10);

	if (local->tm_mon + 1 <= 9)
		strcat(s_date, "0");
	strcat(s_date, month);
	strcat(s_date, "-");
	if (local->tm_mday + 1 <= 10)
		strcat(s_date, "0");
	strcat(s_date, day);
	strcat(s_date, "-19");
	strcat(s_date, year);

	if (local->tm_hour > 11) {
		if (local->tm_hour != 12)
			local->tm_hour = local->tm_hour - 12;
		pm = 1;
	} else if (local->tm_hour == 0)
		local->tm_hour = 12;
	itoa(local->tm_hour, hour, 10);
	itoa(local->tm_min, min, 10);
	if (local->tm_hour < 10)
		strcat(s_time, "0");
	strcat(s_time, hour);
	strcat(s_time, ":");
	if (local->tm_min < 10)
		strcat(s_time, "0");
	strcat(s_time, min);

	if (pm == 0)
		strcat(s_time, " am");
	else
		strcat(s_time, " pm");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void read_file() {

	struct user_idx {
		unsigned char name[41];
		long int left;
		long int right;
	} user_tree;

	FILE *fp;

	size_t rec_size = sizeof(struct user_idx);
	long int number, i;

	user_file_name("idx");
	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.idx\n");
		exit(1);
	}

	fseek(fp, 0L, 0);
	fread(&user_tree, rec_size, 1, fp);
	number = user_tree.left;
	for (i = 0; i <= number; i++) {
		fseek(fp, i * rec_size, 0);
		fread(&user_tree, rec_size, 1, fp);
	}
	fclose(fp);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

char file_lock(char *filename) {

	FILE *fp;

	/* the file is locked already */
	if (access(filename, 00) == 0) {
		return 0;
	} else {
		/*      printf("Locking...");*/
		fp = fopen(filename, "wb");
		fclose(fp);
		return 1;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void file_unlock(char *filename) {

	/*   printf("Unlocking");*/
	unlink(filename);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void create_file() {

	FILE *fp;

	struct user_idx {
		unsigned char name[41];
		long int left;
		long int right;
	} user_tree;

	long int record;

	size_t rec_size = sizeof(struct user_idx);

	user_file_name("idx");

	if ((fp = fopen(reserve, "rb")) == NULL) {
		fclose(fp);
		user_file_name("idx");
		if ((fp = fopen(reserve, "wb+")) == NULL) {
			printf("cannot open users.idx\n");
			exit(1);
		}
		strcpy(user_tree.name, "                                        ");
		user_tree.left = 0;
		user_tree.right = 0;
		record = 0;

		fseek(fp, record * rec_size, 0);
		fwrite(&user_tree, rec_size, 1, fp);
	}
	fclose(fp);
	user_file_name("bbs");

	if ((fp = fopen(reserve, "rb")) == NULL) {
		fclose(fp);
		user_file_name("bbs");

		if ((fp = fopen(reserve, "wb+")) == NULL) {
			printf("cannot open users.idx\n");
			exit(1);
		}
	}
	fclose(fp);
	user_file_name("pnt");

	if ((fp = fopen(reserve, "rb")) == NULL) {
		fclose(fp);
		user_file_name("pnt");
		if ((fp = fopen(reserve, "wb+")) == NULL) {
			printf("cannot open users.pnt\n");
			exit(1);
		}
	}
	fclose(fp);
}

void login(void) {

	char flag = ON;
	char result;

	while (flag) {
		get_name();
		strcpy(first, cut[0].level);
		strcpy(last, cut[1].level);
		strcpy(temp, first);
		strcat(temp, " \0");
		strcat(temp, last);

		result = save_user(ON, OFF);

		if (result) {
			flag = OFF;
			get_password();
		} else {
			p("[32;40m", "\n\nYour name is ");
			p("[36m", cut[0].level);
			p("", " ");
			p("", cut[1].level);
			p("[32m", ", is this correct ");
			p("[34m", "[");
			p("[35m", "Y/n");
			p("[34m", "]");
			p("[32m", ": ");

			if (user.ansi[0] != '1') {
				p("[37;44m", " ");
				p("[1D", "");
			}
			if (get_yn("YN") == 'Y')
				flag = OFF;
		}
	}

	system_log_break();
	g_date();

	strcpy(response, s_date);
	strcat(response, "\n");
	strcat(response, s_time);
	strcat(response, "\n\n");

	write_system_log();

	strcpy(response, first);
	strcat(response, " ");
	strcat(response, last);

	strcat(response, " logging in at ");
	strcat(response, connect_save);
	strcat(response, "\n");
	write_system_log();

	if (!result) {

		ansi_level();
		strcpy(user.ansi, response);
		get_city();
		strcpy(user.city, response);
		m_or_f();
		strcpy(user.sex, response);
		voice();
		strcpy(user.voice, response);
		data();
		strcpy(user.data, response);
		bdate();
		strcpy(user.bdate, response);
		screen_length();
		strcpy(user.screen_length, response);
		screen_clear();
		strcpy(user.screen_clear, response);
		select_more_prompt();
		strcpy(user.more, "Y");
		password();
		strcpy(user.pword, response);

		strcpy(user.secur, "5");
		strcpy(user.total_calls, "0");
		strcpy(user.total_calls_today, "1");
		strcpy(user.last_date_called, s_date);
		strcpy(user.log_in_time, s_time);
		strcpy(user.log_out_time, "00:00 am");
		strcpy(user.time_used, "0");
		strcpy(user.user_comment, "");
		strcpy(user.doors_opened, "0");
		strcpy(user.messages_posted, "0");
		strcpy(user.bps_rate, "2400");
		strcpy(user.new_files, "Y");

		strcpy(user.dl_num, "0");
		strcpy(user.dl_kb, "0");
		strcpy(user.ul_num, "0");
		strcpy(user.ul_kb, "0");
		strcpy(user.dl_date, s_date);
		strcpy(user.dl_num_today, "0");
		strcpy(user.dl_kb_today, "0");
		strcpy(user.ul_date, s_date);
		strcpy(user.ul_num_today, "0");
		strcpy(user.ul_kb_today, "0");

		correct(1);
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

int check_keys(int c) {

	unsigned char more_save;

	if (c == 46 && !chat_flag) {

		chat_flag = ON;
		more_save = more_of;
		more_of = OFF;

		p("[0;1;32;40m", "\n\n");
		p("[44m", "ีออออออออออออออออออออออออออต");
		p("[33;41m", " Mark is here to chat ");
		p("[32;44m", "ฦอออออออออออออออออออออออออออธ");
		p("[40m", "\n\n");

		p("[36;40m", "Hi there ");
		p("[33m", first);
		p("[37m", "...\n\n");

		chat();

		p("[40m", "\n");
		p("[0;1;32;44m", "ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ");

		more_of = more_save;
	}

	if (c == 38)
		stat_bar();

	if (c == 20) {
		upx = 5;
		upy = 5;
		lowx = 14;
		lowy = 69;
		bottom = 15;
	}

	if (c == 47) {
		close_port();
		fifo(0);

		fclose(system_log);
		exit(1);
	}

	if (c == 32) {

		shell_to_dos();
	}
	if (c == 31) {

		p("[0;1;33;40m", "\n\nSecurity: ");
		get_string(5, 0);
		strcpy(user.secur, response);
		p("", "\n");
		men(save_menu);
		show = ON;
	}

	return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void ansi_level() {

	char flag = ON;

	int value;

	while (flag) {
		p("[36;40m", "\n\nWhat level of");
		p("[33m", " ANSI ");
		p("[36m", "graphics would you like: ");
		p("[32m", "\n\n1");
		p("[35m", ")");
		p("[0;37m", " None");
		p("[1;32m", "\n2");
		p("[35m", ")");
		p("[31m", " Some");
		p("[32m", "\n3");
		p("[35m", ")");
		p("[34m", " L");
		p("[35m", "o");
		p("[33m", "t");
		p("[31m", "s");
		p("[33m", "\n\nAnsi Level: ");
		p("[34m", "[");
		p("[35m", "1-3");
		p("[34m", "]");
		p("[36m", ": ");

		if (user.ansi[0] != '1') {
			p("[37;44m", " ");
			p("[1D", "");
		}

		if (!mistake)
			get_number(1, "3");
		else
			get_number(1, "");

		value = atoi(response);

		if (strlen(response) == OFF && mistake == ON)
			flag = OFF;
		else if (value < 1 || value > 3)
			p("[32;40m", "\n\nAnsi level must be between 1 and 3");
		else
			flag = OFF;

		atrib = 7;
	}

	if (response[0] == '3')
		blue_bar = ON;
	else
		blue_bar = OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void user_file_name(unsigned char ext[]) {

	strcpy(reserve, user_path);
	strcat(reserve, "users.");
	strcat(reserve, ext);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void correct(unsigned char save) {

	unsigned char scratch[40], c, flag = ON, result;

	mistake = ON;

	while (flag) {
		p("", "\n\n");
		p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
		p("[40m", "\n");
		p("[44m", "บ");
		p("[41;33m", " User Information ");
		p("[33;44m", "บ");
		p("[40m", "\n");
		p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
		p("[33;40m", "\n\nA");
		p("[31m", ") ");
		p("[36m", "Name       : ");
		strcpy(scratch, first);
		strcat(scratch, " ");
		strcat(scratch, last);

		format(scratch, 30);

		p("[37m", response);
		p("[33;40m", "F");
		p("[31m", ") ");
		p("[36m", "Ansi         : ");
		p("[37m", user.ansi);
		p("[33;40m", "\nB");
		p("[31m", ") ");
		p("[36m", "City       : ");

		format(user.city, 30);

		p("[37m", response);
		p("[33;40m", "G");
		p("[31m", ") ");
		p("[36m", "Screen Length: ");
		p("[37m", user.screen_length);
		p("[33;40m", "\nC");
		p("[31m", ") ");
		p("[36m", "Voice Phone: ");
		display_phone(user.voice);
		strcpy(scratch, "                ");
		p("", scratch);

		p("[33;40m", "H");
		p("[31m", ") ");
		p("[36m", "Birthdate    : ");
		display_bdate(user.bdate);
		p("[33;40m", "\nD");
		p("[31m", ") ");
		p("[36m", "Data Phone : ");
		display_phone(user.data);
		strcpy(scratch, "                ");
		p("", scratch);
		p("[33;40m", "I");
		p("[31m", ") ");
		p("[36m", "Sex          : ");
		display_sex();
		p("[33;40m", "\nE");
		p("[31m", ") ");
		p("[36m", "Password   : ");

		format(user.pword, 30);

		p("[37m", response);
		p("[33;40m", "J");
		p("[31m", ") ");
		p("[36m", "Screen Clear : ");
		display_yn(user.screen_clear);

		if (save) {
			p("[5;33;40m", "\n\nS");
			p("[0;1;31m", ") ");
			p("[32m", "Save Information ");
		} else {
			p("[5;33;40m", "\n\nQ");
			p("[0;1;31m", ")");
			p("[32m", "uit");
		}

		p("[31;40m", "\n\nEnter Selection: ");

		if (user.ansi[0] != '1') {
			p("[34;44m", " ");
			p("[1D", "");
		}

		strcpy(options, "ABCDEFGHIJ");

		if (save)
			strcat(options, "S");
		else
			strcat(options, "Q");

		p("[37;40m", "");
		c = hot_key();

		switch (c) {
		case 'A':
			get_name();
			if (cut[1].level[0] == '\0')
				p("[40;31m", "\n\nChange name has been aborted.");
			else {
				strcpy(temp, cut[0].level);
				strcat(temp, " \0");
				strcat(temp, cut[1].level);
				result = save_user(ON, OFF);
				if (!result) {
					strcpy(first, cut[0].level);
					strcpy(last, cut[1].level);
				} else {
					p("[40;31m", "\n\nThat user is already a user on this system.");
				}
			}
			break;

		case 'B':
			get_city();
			if (cut[1].level[0] == '\0')
				p("[40;31m", "\n\nChange city has been aborted.");
			else
				strcpy(user.city, response);
			break;

		case 'C':
			voice();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nVoice phone change has been aborted.");
			else
				strcpy(user.voice, response);
			break;

		case 'D':
			data();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nData phone change has been aborted.");
			else
				strcpy(user.data, response);
			break;

		case 'E':
			password();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nPassword change has been aborted.");
			else
				strcpy(user.pword, response);
			break;

		case 'F':
			ansi_level();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nAnsi level has been aborted.");
			else
				strcpy(user.ansi, response);
			break;

		case 'G':
			screen_length();
			if (strlen(response) == OFF)
				p("", "\n\naborted");
			else
				strcpy(user.screen_length, response);
			break;

		case 'H':
			bdate();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nBirthdate change has been aborted.");
			else
				strcpy(user.bdate, response);
			break;

		case 'I':
			m_or_f();
			if (response[0] == 1)
				p("[40;31m", "\n\nChange sex has been aborted. [thank god]");
			else
				strcpy(user.sex, response);
			break;

		case 'J':
			screen_clear();
			if (response[0] == 1)
				p("[40;31m", "\n\nClear screen change sex has been aborted.");
			else
				strcpy(user.screen_clear, response);
			break;

		case 'S':
			flag = OFF;
			save_user(OFF, ON);
			break;

		case 'Q':
			flag = OFF;
			break;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void mid(unsigned char str[], unsigned char start, unsigned char num) {

	unsigned char i;
	for (i = 0; i < num; i++)
		response[i] = str[i + start];
	response[i] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_phone(unsigned char phone[]) {

	p("[37m", "(");
	mid(phone, 0, 3);
	p("", response);
	p("", ") ");
	mid(phone, 3, 3);
	p("", response);
	p("", "-");
	mid(phone, 6, 4);
	p("", response);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_bdate(unsigned char bdate[]) {

	p("[37m", "[");
	mid(bdate, 0, 2);
	p("", response);
	p("", "-");
	mid(bdate, 2, 2);
	p("", response);
	p("", "-");
	mid(bdate, 4, 2);
	p("", response);
	p("", "]");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_sex() {

	if (user.sex[0] == 'M')
		p("[37m", "Male");
	else
		p("[37m", "Female");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_yn(unsigned char str[]) {

	if (str[0] == 'Y')
		p("[37m", "Yes");
	else
		p("[37m", "No");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void more_select(unsigned char toggle) {

	if (toggle) {
		more = OFF;
		more_of = ON;
		more_r = ON;
	} else
		more_of = OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void more_prompt() {

	unsigned char c, tmp;
	unsigned char kc[81], tst[81];

	long int d, sv;

	struct file_bbs temp;

	d = atol(user.screen_length);

	save_atrib = ON;

	strcpy(tst, response);

	if (more >= d - 1 && more_r == 2)
		more = 0;
	else if (more >= d - 1 && more_r != 2) {

		if (more_file) {

			/*   temp.delete=g_file_bbs.delete;*/

			strcpy(temp.who, g_file_bbs.who);

			strcpy(temp.bps_rate, g_file_bbs.bps_rate);
			temp.dl_number = g_file_bbs.dl_number;
			strcpy(temp.last_dl_date, g_file_bbs.last_dl_date);
			strcpy(temp.last_dl_time, g_file_bbs.last_dl_time);
			strcpy(temp.sysop_comment, g_file_bbs.sysop_comment);
			temp.attach_msg = g_file_bbs.attach_msg;
			temp.private = g_file_bbs.private;
			temp.start = g_file_bbs.start;
			temp.finish = g_file_bbs.finish;
			temp.one_line_start = g_file_bbs.one_line_start;
			strcpy(temp.detailed, g_file_bbs.detailed);

			p("[0;1;40;36m", "More ");
			p("[31m", "[");
			p("[33m", "Y");
			p("[31m", "]");
			p("[36m", "es ");
			p("[31m", "[");
			p("[33m", "N");
			p("[31m", "]");
			p("[36m", "o ");
			p("[31m", "[");
			p("[33m", "C");
			p("[31m", "]");
			p("[36m", "ontinuous ");
			p("[31m", "[");

			p("[33m", "#");
			p("[32m", ",");
			p("[33m", " #-#");
			p("[32m", ",");
			p("[33m", " #,#,#...");

			p("[31m", "]");
			p("[36m", " Tag: ");

		}

		else {

			p("[0;1;40;36m", "More ");
			p("[31m", "[");
			p("[33m", "Y");
			p("[31m", "]");
			p("[36m", "es ");
			p("[31m", "[");
			p("[33m", "N");
			p("[31m", "]");
			p("[36m", "o ");
			p("[31m", "[");
			p("[33m", "C");
			p("[31m", "]");
			p("[36m", "ontinuous: ");
		}
		more = 0;
		strcpy(options, "YNC");

		big = 0;

		if (more_file)
			c = hot_key_number();
		else
			c = hot_key();

		switch (c) {

		case 'N':
			more_r = OFF;
			break;
		case 13:

			p("", "Y");

			more_r = ON;
			break;
		case 'Y':
			more_r = ON;
			break;
		case 'C':
			more_r = 2;
			break;
		}

		more = 0;

		if (more_file) {

			if (big > 1)
				tmp = big - 1;
			else
				tmp = 0;

			delete (54 + tmp);

			if (big)
				tag_the_file();
			mark_file = 0;

			/*   g_file_bbs.delete=temp.delete;*/

			strcpy(g_file_bbs.who, temp.who);

			strcpy(g_file_bbs.bps_rate, temp.bps_rate);
			g_file_bbs.dl_number = temp.dl_number;
			strcpy(g_file_bbs.last_dl_date, temp.last_dl_date);
			strcpy(g_file_bbs.last_dl_time, temp.last_dl_time);
			strcpy(g_file_bbs.sysop_comment, temp.sysop_comment);
			g_file_bbs.attach_msg = temp.attach_msg;
			g_file_bbs.private = temp.private;
			g_file_bbs.start = temp.start;
			g_file_bbs.finish = temp.finish;
			g_file_bbs.one_line_start = temp.one_line_start;
			strcpy(g_file_bbs.detailed, temp.detailed);

		} else
			delete (31);

		if (user.ansi[0] != '1') {
			strcpy(kc, "[");
			strcat(kc, save_ansi_param);

			p(kc, "");
		}
	}
	strcpy(response, tst);
	save_atrib = OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void send_clear_screen() {

	if (user.screen_clear[0] == 'Y') {
		if (user.ansi[0] != '1') {
			p("[0;1;40m", "");
			p("[2J", "");
			p("[1;1H", "");
		} else {
			cls(upx + 1, upy, lowx + 1, lowy);
			xloc = 0;
			yloc = 0;

			if (!local)
				put_serial((unsigned char)12);
		}
	} else {
		p("[0;1;40m", "\n\n");
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_password() {

	unsigned char flag = ON;
	unsigned char count = 0;

	while (flag) {

		p("[0;1;40;36m", "\n\nEnter your password: ");

		if (user.ansi[0] != '1') {
			p("[37;44m", "          ");
			p("[10D", "");
		}
		get_string(10, 1);

		if (!strcmp(user.pword, response))
			flag = OFF;
		else
			p("[40;31m", "\n\nWrong!");

		count++;

		if (count >= 3 && flag) {
			p("[40;36m", "\n\nTo many attempts");
			close_port();
			longjmp(state, -1);
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void press_return() {

	unsigned char c = OFF;
	unsigned char lngth;

	p("[0;1;36;40m", "");

	if ((more_r && search_f) || (more_r && text_search) || (more_r && scan_new_files)) {
		p("", "\n[");
		p("[32m", "Please press");
		p("[33m", " RETURN ");
		p("[32m", "to Continue");
		p("[36m", "]  ");

		p("[36m", "[");
		p("[32m", "Q");
		p("[36m", "]");
		p("[33m", "uit: ");
		strcpy(options, "Q");
		lngth = 45;
	} else {

		p("", "\n<Please press");
		p("[33m", " RETURN ");
		p("[36m", "to Continue>: ");
		lngth = 36;
	}

	if (user.ansi[0] != '1') {
		p("[37;44m", " ");
		p("[1D", "");
	}

	while (c == 0)
		c = (unsigned char)inkey();

	send[0] = c;
	send[1] = '\0';
	strupr(send);
	c = send[0];

	if ((c) == 'Q')
		more_r = OFF;

	p("[40m", " ");

	if (user.ansi[0] != '1') {
		delete (lngth);
	}
}

void save_video(startx, endx, starty, endy, buf_ptr)

    int startx,
    endx, starty, endy;
unsigned char *buf_ptr;

{
	unsigned int i, j;
	char far *v, far *t;

	v = vid_mem;

	for (i = starty; i < endy; i++)

		for (j = startx; j < endx; j++) {

			t = v + (j * 160) + i * 2;
			*buf_ptr++ = *t++;
			*buf_ptr++ = *t;
		}
}

/*

void save_video(startx,endx,starty,endy,buf_ptr)

int startx,endx,starty,endy;
unsigned char *buf_ptr;

{
   register unsigned char i,j,k;
   char far *v,far *t;

   float check=0.0;
   k=0;
   v = vid_mem;

   check = (endx-startx)/2.0;

   if (check == (endx-startx)/2)
      check=0.0;
   else
      check=1.0;


   for (j=startx ; j<((endx-startx)/2)+startx;j++) {
      k++;
      for (i=starty;i<endy; i++)      {
     t= v + (j*160) + i*2;
     *buf_ptr++ = *t++;
     *buf_ptr++ = *t;
     t= v + ((endx-k)*160) + i*2;
     *buf_ptr++ = *t++;
     *buf_ptr++ = *t;
      }
   }

   k++;

   if (check==1.0) {
      for (i=starty;i<endy; i++)      {
     t= v + (j*160) + i*2;
     *buf_ptr++ = *t++;
     *buf_ptr++ = *t;
      }
   }
}

*/

void restore_video(startx, endx, starty, endy, buf_ptr)

    int startx,
    endx, starty, endy;
unsigned char *buf_ptr;

{

	int i, j;
	char far *v, far *t;

	v = vid_mem;
	t = v;

	for (i = starty; i < endy; i++)

		for (j = startx; j < endx; j++) {
			v = t;
			v += (j * 160) + i * 2;
			*v++ = *buf_ptr++;
			*v = *buf_ptr++;
		}
}

/*

void restore_video(startx,endx,starty,endy,buf_ptr)

int startx,endx,starty,endy;
unsigned char *buf_ptr;

{

   register unsigned char i,j,k;
   char far *v, far *t;
   float check=0.0;

   k=0;
   v = vid_mem;
   t=v;

   check = (endx-startx)/2.0;

   if (check == (endx-startx)/2)
      check=0.0;
   else
      check=1.0;

   for (j=startx;j<((endx-startx)/2)+startx;j++) {
      k++;
      for (i=starty;i<endy;i++) {
     v=t;
     v += (j*160) + i*2;
     *v++ = *buf_ptr++;
     *v = *buf_ptr++;
     v=t;
     v += ((endx-k)*160) + i*2;
     *v++ = *buf_ptr++;
     *v = *buf_ptr++;
      }
   }

   k++;

   if (check==1.0) {
      for (i=starty;i<endy;i++)     {
     v=t;
     v += (j*160) + i*2;
     *v++ = *buf_ptr++;
     *v = *buf_ptr++;
      }
   }
}

*/

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void select_more_prompt() {

	p("[36;40m", "\n\nWould you like a MORE prompt ");
	p("[34m", "[");
	p("[35m", "Y/n");
	p("[34m", "]");
	p("[36m", ": ");
	p("[37;44m", " ");
	p("[1D", "");

	response[0] = get_yn("YN");
	response[1] = '\0';
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char is_valid(unsigned char c) {

	switch (c) {

	case NULL:
		return 0;
		break;

	case 7:
		return 0;
		break;

	case 10:
		return 0;
		break;

	case 11:
		return 0;
		break;

	case 12:
		return 0;
		break;

	case 16:
		return 0;
		break;

	case 17:
		return 0;
		break;

	case 28:
		return 0;
		break;

	case 29:
		return 0;
		break;

	case 30:
		return 0;
		break;

	case 31:
		return 0;
		break;

	case 255:
		return 0;
		break;
	}

	return 1;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

char save_user(unsigned char search, char loc) {

	struct user_idx {
		unsigned char name[41];
		long int left;
		long int right;
	} user_tree;

	char result;
	char side;
	char flag = 1;

	long int count = 1;
	long int record;
	size_t rec_size = sizeof(struct user_idx);
	size_t rec_size2 = sizeof(struct user_info);
	long int number;

	FILE *fp;

	if (loc) {
		while (file_lock("user.loc") == 0) {
			printf("file is locked\n");
		}
	}

	user_file_name("idx");

	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("Cannot open users.idx\n");
		exit(1);
	}

	fseek(fp, 0L, 0);
	fread(&user_tree, rec_size, 1, fp);
	number = user_tree.left;

	if (number == 0) {
		if (search) {
			fclose(fp);
			return 0;
		}
		number++;
		strcpy(user_tree.name, temp);
		user_tree.left = 0;
		user_tree.right = 0;
		record = 1;
		fseek(fp, record * rec_size, 0);
		fwrite(&user_tree, rec_size, 1, fp);
		strcpy(user_tree.name, "");
		user_tree.left = number;
		user_tree.right = 0;
		fseek(fp, 0L, 0);
		fwrite(&user_tree, rec_size, 1, fp);
	} else {
		record = 1;
		while (flag) {
			fseek(fp, record * rec_size, 0);
			fread(&user_tree, rec_size, 1, fp);
			count = record;
			result = (char)strcmp(temp, user_tree.name);
			if (result > 0) {
				record = user_tree.right;

				side = 1;
			} else if (result < 0) {
				record = user_tree.left;
				side = 0;

			} else {
				fclose(fp);

				if (search == 2)
					return 1;

				user_file_name("bbs");
				if ((fp = fopen(reserve, "rb+")) == NULL) {
					printf("cannot open users.bbs\n");
					exit(1);
				}

				user_rec_num = count;

				fseek(fp, user_rec_num * rec_size2, 0);
				fread(&user, rec_size2, 1, fp);

				if (user.ansi[0] == '1') {
					atrib = 7;
				}
				fclose(fp);

				user_file_name("pnt");

				if ((fp = fopen(reserve, "rb+")) == NULL) {
					printf("cannot open users.pnt\n");
					exit(1);
				}

				fseek(fp, user_rec_num * sizeof(struct last_read), 0);
				fread(&lr_pnt, sizeof(struct last_read), 1, fp);
				fclose(fp);

				user_file_name("fil");

				if ((fp = fopen(reserve, "rb+")) == NULL) {
					printf("cannot open users.pnt\n");
					exit(1);
				}

				fseek(fp, user_rec_num * sizeof(struct last_scan_date), 0);
				fread(&l_scan_date, sizeof(struct last_scan_date), 1, fp);
				fclose(fp);

				return 1;
			}
			if (record == 0)
				flag = 0;
		}
		if (search) {
			fclose(fp);
			return 0;
		}
		number++;
		if (side == 1) {
			user_tree.right = number;
			record = count;
			fseek(fp, record * rec_size, 0);
			fwrite(&user_tree, rec_size, 1, fp);
		} else {
			user_tree.left = number;
			record = count;
			fseek(fp, record * rec_size, 0);
			fwrite(&user_tree, rec_size, 1, fp);
		}
		strcpy(user_tree.name, temp);
		user_tree.left = 0;
		user_tree.right = 0;
		record = number;
		fseek(fp, record * rec_size, 0);
		fwrite(&user_tree, rec_size, 1, fp);
		strcpy(user_tree.name, "");
		user_tree.left = number;
		user_tree.right = 0;
		fseek(fp, 0L, 0);
		fwrite(&user_tree, rec_size, 1, fp);
	}
	fclose(fp);

	user_file_name("bbs");
	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.bbs\n");
		exit(1);
	}

	record = number;
	user_rec_num = record;

	fseek(fp, record * rec_size2, 0);
	fwrite(&user, rec_size2, 1, fp);
	fclose(fp);

	user_file_name("pnt");
	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.pnt\n");
		exit(1);
	}

	for (count = 0; count < 50; count++) {
		lr_pnt.mes_pnt[count] = 1L;
		lr_pnt.base_pnt[count] = 1L;
	}
	for (count = 0; count < 50; count++) {

		if (count < number_of_mess_areas)
			lr_pnt.scan[count] = 'Y';
		else
			lr_pnt.scan[count] = 'N';
	}

	fseek(fp, record * sizeof(struct last_read), 0);
	fwrite(&lr_pnt, sizeof(struct last_read), 1, fp);
	fclose(fp);

	user_file_name("fil");
	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.pnt\n");
		exit(1);
	}

	g_date();

	for (count = 0; count < 10; count++) {

		if (count < number_of_file_areas) {
			l_scan_date.tag[count] = 'Y';
			strcpy(l_scan_date.last_date[count], s_date);
			strcpy(l_scan_date.last_time[count], s_time);

		} else {
			l_scan_date.tag[count] = 'N';
			strcpy(l_scan_date.last_date[count], "        ");
			strcpy(l_scan_date.last_time[count], "        ");
		}
	}

	fseek(fp, record * sizeof(struct last_read), 0);
	fwrite(&lr_pnt, sizeof(struct last_read), 1, fp);
	fclose(fp);

	if (loc)
		file_unlock("user.loc");
}
