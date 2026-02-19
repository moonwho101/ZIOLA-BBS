#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <bios.h>
#include <time.h>
#include <io.h>

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

void stat_bar() {

	unsigned char how_far_up;
	unsigned char save_it[81];

	if (s_bar) {
		upx = 0;
		upy = 0;
		lowx = 23;
		lowy = 79;
		bottom = 24;
		draw_border(20, 0, 24, 79, 0, 1, 0);
		s_bar = OFF;

	} else {
		strcpy(save_it, response);

		if (xloc > 19) {
			how_far_up = (unsigned char)xloc - (unsigned char)18;
			scroll_window(upy, upx + 1, lowy, lowx + 1, how_far_up);
			xloc = xloc - how_far_up;
			goto_xy(xloc + 1, yloc);
		}

		upx = 0;
		upy = 0;
		lowx = 18;
		lowy = 79;
		bottom = 19;

		clean_bar((unsigned char)bottom);

		draw_border(20, 0, 24, 79, 30, 1, 72);
		s_bar = ON;

		write_str("Name", 21, 2, 78);
		write_str(":", 21, 8, 74);

		strcpy(reserve, first);
		strcat(reserve, " ");
		strcat(reserve, last);

		format(reserve, 20);

		write_str(response, 21, 10, 75);
		write_str("City", 21, 26, 78);
		write_str(":", 21, 30, 74);

		format(user.city, 20);

		write_str(response, 21, 32, 75);
		write_str("Bps", 21, 50, 78);
		write_str(":", 21, 54, 74);

		format(connect_at, 7);

		write_str(response, 21, 56, 75);
		write_str("Secur", 21, 63, 78);
		write_str(":", 21, 68, 74);

		format(user.secur, 7);

		write_str(response, 21, 70, 75);
		write_str("Voice", 23, 2, 78);
		write_str(":", 23, 8, 74);
		write_str("Sex", 22, 2, 78);
		write_str(":", 22, 8, 74);

		if (user.sex[0] == 'M')
			write_str("Male", 22, 10, 75);
		else
			write_str("Female", 22, 10, 75);

		write_str("Age", 22, 26, 78);
		write_str(":", 22, 30, 74);

		g_date();
		age(user.bdate, s_date);

		write_str(response, 22, 32, 75);
		write_str("ARQ ", 22, 50, 78);
		write_str(":", 22, 54, 74);

		if (erc)
			write_str("Yes", 22, 56, 75);
		else
			write_str("No", 22, 56, 75);

		write_str("Call", 22, 63, 78);
		write_str(":", 22, 68, 74);
		write_str(user.total_calls, 22, 70, 75);

		strcpy(reserve, "(");

		mid(user.voice, 0, 3);
		strcat(reserve, response);
		strcat(reserve, ") ");

		mid(user.voice, 3, 3);

		strcat(reserve, response);
		strcat(reserve, "-");

		mid(user.voice, 6, 4);
		strcat(reserve, response);

		write_str(reserve, 23, 10, 75);
		write_str("Data", 23, 26, 78);
		write_str(":", 23, 30, 74);

		strcpy(reserve, "(");

		mid(user.voice, 0, 3);
		strcat(reserve, response);
		strcat(reserve, ") ");

		mid(user.voice, 3, 3);

		strcat(reserve, response);
		strcat(reserve, "-");

		mid(user.voice, 6, 4);
		strcat(reserve, response);

		write_str(reserve, 23, 32, 75);
		write_str("Ansi", 23, 50, 78);
		write_str(":", 23, 54, 74);
		write_str(user.ansi, 23, 56, 75);
		write_str("Posts", 23, 63, 78);
		write_str(":", 23, 68, 74);
		write_str(user.messages_posted, 23, 70, 75);

		strcpy(response, save_it);
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void file_name(unsigned char ext[], long int mes_area) {

	unsigned char work[10];

	strcpy(reserve, mess_path);
	strcat(reserve, mess_area[0].name);

	if (mes_area > 0) {
		ltoa(mes_area, work, 10);
		strcat(reserve, work);
	} else
		strcat(reserve, menu[do_function].command);
	strcat(reserve, ".");
	strcat(reserve, ext);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

long int m_bound(long int mes_num) {

	long int m_num, size = 0;

	size_t rec_size_idx = sizeof(struct index);

	struct index message_data;

	FILE *id;

	file_name("idx", 0L);

	if ((id = fopen(reserve, "rb+")) == NULL) {
		printf("\n->%s<-", reserve);
		exit(1);
	}

	fseek(id, 0L, 0);
	fread(&message_data, rec_size_idx, 1, id);
	fclose(id);
	m_num = message_data.parent;

	if (mes_num > m_num)
		return 0;

	return m_num;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void intro_line_editor(unsigned char who[], unsigned char public) {

	if (!save_file_desc)
		send_clear_screen();
	else {
		p("[40m", "\n\n");
	}

	p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
	p("[40m", "\n");
	p("[44m", "บ");
	p("[41;33m", "  The Line Editor  ");
	p("[33;44m", "บ");
	p("[40m", "\n");
	p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

	if (user.ansi[0] == '1') {
		p("[33;40m", "\n\n   ESC    ");
		p("[32m", "= ");
		p("[36m", " Line Editor Options        ");
		p("[33m", "CTRL T");
		p("[32m", " = ");
		p("[36m", "Time and Date");
	} else {
		p("[5;33;40m", "\n\n   ESC    ");
		p("[0;1;32m", "= ");
		p("[36m", " Line Editor Options        ");
		p("[33m", " CTRL A + 1-24");
		p("[32m", " = ");
		p("[36m", " Color Change");
		p("[33m", "\n   CTRL C");
		p("[32m", " = ");
		p("[36m", " Color Chart                 ");
		p("[33m", "CTRL T       ");
		p("[32m", " = ");
		p("[36m", " Time and Date");
	}

	if (!save_file_desc) {
		p("[36;40m", "\n\n  This is a ");

		if (public == 'N')
			p("[33m", "Public");
		else
			p("[33m", "Private");

		p("[36m", " message addressed to ");
		p("[33m", who);
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_m_area() {

	long int area, m_bnd;

	p("[0;1;33;44m", "Message Area ");
	p("[31m", "#");

	format(menu[do_function].command, 3);
	p("[36m", response);

	area = atol(menu[do_function].command);

	p("[37m", ", ");

	format(mess_name[area - 1].name, 30);

	p("[32m", response);
	p("[33m", "Active Messages: ");

	m_bnd = m_bound(0L);
	ltoa(m_bnd, reserve, 10);

	format(reserve, 13);
	p("[32m", response);
	p("[40m", "\n\n");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void message_file() {

	FILE *fp;
	unsigned char i, f_message[MSIZE];

	struct index idx;
	struct who who_tf;

	size_t rec_size_bbs = sizeof(f_message);
	size_t rec_size_idx = sizeof(struct index);
	size_t rec_size_who = sizeof(struct who);

	file_name("idx", 0L);

	if ((fp = fopen(reserve, "wb+")) == NULL) {
		printf("Cannot Open");
		exit(1);
	}
	strcpy(idx.private, " ");
	strcpy(idx.date, "        ");
	strcpy(idx.time, "        ");
	strcpy(idx.subject, "                                                                      ");

	idx.parent = 0;
	idx.sib = 1;
	idx.child = 0;
	idx.start = 0;
	idx.end = 0;
	idx.reply = 0;

	fseek(fp, 0L, 0);
	fwrite(&idx, rec_size_idx, 1, fp);

	fclose(fp);

	file_name("bbs", 0L);

	if ((fp = fopen(reserve, "wb+")) == NULL) {
		printf("Cannot Open");
		exit(1);
	}

	for (i = 0; i < MSIZE - 1; i++)
		f_message[i] = 32;
	f_message[i] = '\0';

	fwrite(f_message, rec_size_bbs, 1, fp);
	fclose(fp);

	file_name("who", 0L);

	if ((fp = fopen(reserve, "wb+")) == NULL) {
		printf("Cannot Open");
		exit(1);
	}

	strcpy(who_tf.received, " ");
	strcpy(who_tf.to, "                                        ");
	strcpy(who_tf.from, "                                        ");
	fwrite(&who_tf, rec_size_who, 1, fp);

	fclose(fp);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void read_type() {

	unsigned char c, scratch, d;
	long int number, m_bnd;
	long int mes_num, cnt, area;
	unsigned char end[20], save_it[20], skip;

	send_clear_screen();

	display_m_area();

	p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
	p("[40m", "\n");
	p("[44m", "บ");
	p("[41;33m", "  Reading Messages  ");
	p("[33;44m", "บ");
	p("[40m", "\n");
	p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

	p("[0;1;33;40m", "\n\n");

	p("[36m", "[");
	p("[33m", "N");
	p("[36m", "]");
	p("[32m", "ew Messages\n");

	p("[36m", "[");
	p("[33m", "F");
	p("[36m", "]");
	p("[32m", "orward\n");

	p("[36m", "[");
	p("[33m", "R");
	p("[36m", "]");
	p("[32m", "everse");

	p("[36;40m", "\n\nEnter Selection ");
	p("[32m", "[");
	p("[33m", "N R F");
	p("[32m", "]");
	p("[36m", ": ");

	strcpy(options, "NRF");

	p("[37m", "");

	c = hot_key();

	switch (c) {
	case 'F':
	case 'R':
		scratch = ON;
		while (scratch) {
			if (c == 'R') {
				mes_num = m_bound(1L);

				ltoa(mes_num, end, 10);
				number = jump("Enter starting number:", end);
			} else
				number = jump("Enter starting number:", "1");

			if (!number) {
				scratch = OFF;
				p("[31;40m", "\n\nRead messages has been aborted.\n\n");
				return;
			} else {
				m_bnd = m_bound(number);
				if (!m_bnd) {
					p("[31;40m", "\n\nThat is not a valid message number.");
				} else {
					scratch = OFF;

					/*		  if (c=='F')
					          lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=number;*/
				}
			}
		}
		break;

	case 'N':

		p("[0;1;33;44m", "\n\nNew Message Read");

		p("[0;1;40;36m", "\n\n[");
		p("[31m", "T");
		p("[36m", "]");
		p("[33m", "his Area\n");

		p("[36m", "[");
		p("[31m", "A");
		p("[36m", "]");
		p("[33m", "ll Areas\n");

		p("[36m", "[");
		p("[31m", "Q");
		p("[36m", "]");
		p("[33m", "uit");

		p("[32;40m", "\n\nEnter Selection ");
		p("[36m", "[");
		p("[33m", "T A Q");
		p("[36m", "]");
		p("[32m", ": ");

		strcpy(options, "TAQ");
		p("[37m", "");

		d = hot_key();

		switch (d) {

		case 'A':

			stop_all_read = OFF;
			skip = OFF;

			strcpy(save_it, menu[do_function].command);
			strcpy(menu[do_function].command, "1");

			cnt = 0;
			p("[0;1;33;40m", "\n\nSwitching to Message Area ");
			p("[31m", "#");
			p("[36m", "1");
			p("[37m", ", ");
			p("[33;44m", mess_name[cnt].name);
			p("[40m", "\n");
			skip_area();

			strcpy(options, "AS");

			d = hot_key();

			if (d == 'A') {
				strcpy(menu[do_function].command, save_it);

				p("[0;1;33;40m", "\n\nRead all message areas aborted. ");

				return;
			}

			else if (d == 'S')
				skip = ON;

			while (cnt < number_of_mess_areas - 1) {

				if (!skip) {

					number = lr_pnt.base_pnt[cnt];

					m_bnd = m_bound(number);
				}
				c = 'F';
				if (!m_bnd || stop_all_read || skip) {

					if (!m_bnd && !stop_all_read && !skip) {

						p("[0;1;32;40m", "\n\nNo ");
						p("[31m", "NEW ");
						p("[32m", "messages in this base.\n");
					}

					skip = OFF;

					stop_all_read = OFF;

					if (quick_scan)
						quick_scan = 1;
					else if (header_scan)
						header_scan = 1;
					/*
					          if (!m_bnd)
					          p("[31;40m","\n\nEnd Of Message File.");
					  */

					cnt++;

					if (cnt >= number_of_mess_areas - 1)
						break;

					ltoa(cnt + 1, response, 10);
					strcpy(menu[do_function].command, response);

					p("[0;1;33;40m", "\n\nSwitching to Message Area ");
					p("[31m", "#");
					p("[36m", menu[do_function].command);
					area = atol(menu[do_function].command);
					p("[37m", ", ");

					p("[33;44m", mess_name[cnt].name);
					p("[40m", "\n");

					skip_area();

					strcpy(options, "AS");

					d = hot_key();

					if (d == 'A') {
						strcpy(menu[do_function].command, save_it);

						p("[0;1;33;40m", "\n\nRead all message areas aborted. ");

						return;
					}

					else if (d == 'S')
						skip = ON;

				}

				else {

					/*	    lr_pnt.base_pnt[cnt]=number;*/

					read_message(number, c);
				}
			}
			strcpy(menu[do_function].command, save_it);
			return;
			break;

		case 'T':

			number = lr_pnt.base_pnt[atol(menu[do_function].command) - 1L];

			m_bnd = m_bound(number);
			c = 'F';
			if (!m_bnd) {
				p("[31;40m", "\n\nEnd Of Message File.");
				return;
			}
			/*	 lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=number;*/

			break;

		case 'Q':
		case 13:
			return;
			break;
		}
		break;

	case 13:
		return;
		break;
	}
	read_message(number, c);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void draw_border(startx, starty, endx, endy, b_col, fil, f_col)

    unsigned char startx,
    starty, endx, endy;
unsigned char b_col;
unsigned char fil, f_col;
{

	register int i, j;
	char far *v, far *t;
	v = vid_mem;

	t = v;
	for (i = startx + 1; i < endx; i++) {
		v += (i * 160) + starty * 2;
		*v++ = 179;
		*v = b_col;
		v = t;
		v += (i * 160) + endy * 2;
		*v++ = 179;
		*v = b_col;
		v = t;
	}
	for (i = starty + 1; i < endy; i++) {
		v += (startx * 160) + i * 2;
		*v++ = 196;
		*v = b_col;
		v = t;
		v += (endx * 160) + i * 2;
		*v++ = 196;
		*v = b_col;
		v = t;
	}
	write_char(startx, starty, 218, b_col, 0);
	write_char(startx, endy, 191, b_col, 0);
	write_char(endx, starty, 192, b_col, 0);
	write_char(endx, endy, 217, b_col, 0);

	if (fil) {
		for (i = startx + 1; i < endx; i++) {
			for (j = starty + 1; j < endy; j++)
				write_char(i, j, 32, f_col, 0);
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void age(unsigned char date1[], unsigned char date2[]) {

	unsigned char first[3], second[3];
	long int years;

	mid(date1, 4, 2);
	strcpy(first, response);
	mid(date2, 8, 2);
	strcpy(second, response);

	years = atol(second) - atol(first);
	years--;
	mid(date1, 0, 2);
	strcpy(first, response);
	mid(date2, 0, 2);
	strcpy(second, response);

	if (atol(second) > atol(first))
		years++;
	else if (atol(second) == atol(first)) {
		mid(date1, 2, 2);
		strcpy(first, response);
		mid(date2, 3, 2);
		strcpy(second, response);

		if (atol(second) >= atol(first))
			years++;
	}
	ltoa(years, response, 10);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void days(unsigned char date1[], unsigned char date2[]) {

	unsigned char first[3], second[3];
	long int years, days;

	mid(date1, 6, 2);
	strcpy(first, response);
	mid(date2, 6, 2);
	strcpy(second, response);

	days = 0;
	if (atol(second) > atol(first)) {
		years = second - first;
		days = years * 365L;
	}
	mid(date1, 3, 2);
	strcpy(first, response);
	mid(date2, 3, 2);
	strcpy(second, response);

	if (atol(second) > atol(first)) {
		years = atol(second) - atol(first);
		days = days + (years * 30L);
	}

	mid(date1, 0, 2);
	strcpy(first, response);
	mid(date2, 0, 2);
	strcpy(second, response);

	if (atol(second) > atol(first)) {
		years = atol(second) - atol(first);
		days = days + years;
	}
	ltoa(days, response, 10);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void clean_bar(unsigned char start) {

	unsigned char count;
	unsigned char inside;

	for (count = start; count < 25; count++) {
		for (inside = 0; inside < 80; inside++) {
			screen[index[count]].chrtr[inside] = ' ';
			screen[index[count]].atrib[inside] = 7;
		}
	}
}

void skip_area() {

	p("[32m", "\n[");
	p("[36m", "Press ");
	p("[31m", "Enter ");
	p("[36m", "to Continue");
	p("[32m", "]  ");

	p("[32m", "[");

	p("[31m", "A");

	p("[32m", "]");

	p("[36m", "bort New Mail Read");

	p("[32m", "  [");

	p("[31m", "S");

	p("[32m", "]");

	p("[36m", "kip this area");

	p("[31m", ": ");

	if (user.ansi[0] != '1') {
		p("[37;44m", " ");
		p("[1D", "");
	}
}