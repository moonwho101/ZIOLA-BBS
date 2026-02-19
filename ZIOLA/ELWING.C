#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <bios.h>
#include <time.h>
#include <io.h>
#include <conio.h>
#include <process.h>
#include <time.h>
#include <dos.h>
#include <math.h>

#include "serial.h"
#include "dansi.h"
#include "mess.h"
#include "menu.h"
#include "misc.h"
#include "file.h"
#include "ginput.h"
#include "main.h"
#include "rmess.h"
#include "wmess.h"
#include "modem.h"
void execute_door();

enum notes {
	C0 = 262,
	D0 = 296,
	E0 = 330,
	F0 = 349,
	G0 = 392,
	A0 = 440,
	B0 = 494,
	C1 = 523,
	D1 = 587,
	E1 = 659,
	F1 = 698,
	G1 = 784,
	A1 = 880,
	B1 = 988,
	SHORT = 20,
	EIGHTH = 125,
	QUARTER = 250,
	HALF = 500,
	WHOLE = 1000,
	END = 0
} song[] = { C1, SHORT, G0, SHORT, E1, SHORT, END };

main() {

	unsigned char flag, tmp, tmp2;
	unsigned char snd[2];
	unsigned char spd = 0;
	unsigned char save_it[2];

	long int calc;
	long int area;

	time_allowed = 60;
	time_out = 6;

	number_of_mess_areas = 5L;
	number_of_file_areas = 4L;

	strcpy(menu_path, "C:\\BZ\\MENU\\");
	strcpy(mess_path, "C:\\BZ\\MESS\\");
	strcpy(user_path, "C:\\BZ\\USERS\\");
	strcpy(file_path, "C:\\");

	strcpy(mess_name[0].name, "Centre Ring");
	strcpy(mess_name[1].name, "Clowns");
	strcpy(mess_name[2].name, "The Tent");
	strcpy(mess_name[3].name, "Lion's Cage");
	strcpy(mess_name[4].name, "SysOp");

	strcpy(file_names[0].name, "Games");
	strcpy(file_names[1].name, "Utilities");
	strcpy(file_names[2].name, "Programmers");
	strcpy(file_names[3].name, "Windows/OS2");

	s_bar = OFF;
	save_file_desc = OFF;

	strcpy(line, "ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ");
	strcpy(blnk, "                                                                                ");

	new_mail_read = OFF;
	normal[0] = 6;
	normal[1] = 7;
	normal[2] = 6;
	normal[3] = 7;
	normal[4] = 6;
	normal[5] = 6;

	reply_s[0] = 6;
	reply_s[1] = 7;
	reply_s[2] = 8;
	reply_s[3] = 6;
	reply_s[4] = 7;
	reply_s[5] = 6;
	reply_s[6] = 6;

	if (setjmp(state) == 0)
		printf("\nsaved");
	else {
		spd = 1;
		if (local == OFF)
			fifo(0);
		close_port();
	}

	flag = ON;

	fclose(system_log);

	for (;;) {

		if ((system_log = fopen("system.log", "at+")) == NULL) {
			printf("can't open");
			exit(1);
		}

		header_scan = OFF;

		quick_scan = OFF;
		more_select(OFF);
		for (tmp = 0; tmp < 50; tmp++)
			file_index[tmp] = tmp;

		system_ul_dl_ratio = .1;

		valid_user = OFF;
		carrier_detect = ON;
		hot_of = OFF;
		mistake = OFF;
		local = OFF;
		file_scroll = OFF;
		tag = ON;

		text_search = OFF;

		tag_bypass = OFF;

		strcpy(free_dl, "3000");
		strcpy(ul_ratio, "1");
		strcpy(dl_ratio, "10");

		user.screen_clear[0] = 'Y';

		strcpy(mess_area[0].name, "M");
		/*unlink ("*.loc");*/
		chat_flag = OFF;
		items_in_queue = 0;
		add_to_queue = OFF;
		/*    create_file_areas();*/
		list_batch = OFF;
		no_more = OFF;
		/*
		      strcpy(menu[do_function].command,"1");
		      message_file();


		      strcpy(menu[do_function].command,"2");
		      message_file();
		      strcpy(menu[do_function].command,"3");
		      message_file();
		      strcpy(menu[do_function].command,"4");
		      message_file();
		      strcpy(menu[do_function].command,"5");
		      message_file();
		      strcpy(menu[do_function].command,"6");
		      message_file();
		*/
		for (tmp = 0; tmp < 30; tmp++) {
			index[tmp] = tmp;

			for (tmp2 = 0; tmp2 < 80; tmp2++) {
				screen[index[tmp]].atrib[tmp2] = 7;
				screen[index[tmp]].chrtr[tmp2] = 32;
			}
		}

		strcpy(save_menu, "0");

		user.ansi[0] = '3';
		obey_ansi_input = ON;
		grab_ansi = OFF;
		user_ansi = OFF;
		scan_new_files = OFF;
		search_f = OFF;
		save_atrib = OFF;

		other_areas = OFF;

		if (user.ansi[0] == '3')
			blue_bar = ON;
		else
			blue_bar = OFF;

		sysop_chat_color = 2;
		user_chat_color = 4;

		atrib = 6;
		send[1] = '\0';
		temp[0] = '\0';

		xloc = 0;
		yloc = 0;
		direct_write = ON;
		upx = 0;
		upy = 0;
		lowx = 23;
		lowy = 79;
		bottom = lowx + 1;

		/*unlink("users.*");*/
		create_file();
		get_video();

		cls(0, 0, 24, 79);

		if (!spd) {

			speed();
			spd = 1;
		}

		cls(0, 0, 24, 79);

		status_line();

		init_modem();

		wait_caller();
		/*  delay_time=0;*/

		g_date();
		strcpy(display_time, "60");
		time(&start_time);

		p("[0;1;32;40m", "\nCurrent Time: ");
		p("[36m", s_time);
		p("[32m", "\nCurrent Date: ");
		p("[36m", s_date);

		hot_of = ON;
		strcpy(options, "S");
		strcpy(reserve, menu_path);
		strcat(reserve, "main");
		time(&time_out_start);
		get_menu(reserve, 1);
		rfile(title_file, 0);
		hot_of = OFF;

		login();

		if (user.ansi[0] == '3')
			blue_bar = ON;
		else
			blue_bar = OFF;

		g_date();
		s_bar = OFF;
		stat_bar();

		if (strcmp(user.last_date_called, s_date)) {
			strcpy(user.time_used, "0");
			strcpy(user.dl_num_today, "0");
			strcpy(user.dl_kb_today, "0");
			strcpy(user.ul_num_today, "0");
			strcpy(user.ul_kb_today, "0");
		}

		strcpy(user.log_in_time, s_time);

		calc = atol(user.time_used);

		time_allowed = 60 - calc;

		valid_user = ON;
		check_time();

		show = OFF;
		key_pressed = OFF;

		hot_of = ON;
		strcpy(options, "S");
		strcpy(reserve, menu_path);
		strcat(reserve, "hello");
		time(&time_out_start);
		get_menu(reserve, 1);

		more_select(ON);

		rfile(title_file, 0);
		hot_of = OFF;
		more_select(OFF);
		press_return();

		for (tmp = 0; tmp < number_of_file_areas; tmp++) {

			strcpy(l_scan_date2.last_date[tmp], l_scan_date.last_date[tmp]);
			strcpy(l_scan_date2.last_time[tmp], l_scan_date.last_time[tmp]);
			l_scan_date2.touched[tmp] = 'N';
		}

		scan_new_mail();

		log_in_scan_files(1);

		send_clear_screen();

		key_pressed = OFF;

		men("0");

		flag = ON;

		total_t_time = 0;

		while (flag) {
			do_function = get_funct();
			key_pressed = OFF;

			if (do_function == 999) {

				show = OFF;

			}

			else {

				if (menu[do_function].function == 1) {

					strcpy(response, "Writing message in area #");

					strcat(response, menu[do_function].command);

					strcat(response, ", ");

					area = atol(menu[do_function].command);

					area--;
					strcat(response, mess_name[area].name);

					strcat(response, "\n");

					write_system_log();

					who(OFF);

				} else if (menu[do_function].function == 2) {

					strcpy(response, "Reading message in area #");

					strcat(response, menu[do_function].command);

					strcat(response, ", ");

					area = atol(menu[do_function].command);

					area--;
					strcat(response, mess_name[area].name);

					strcat(response, "\n");

					write_system_log();

					read_type();

				} else if (menu[do_function].function == 3) {
					flag = OFF;
					hot_of = ON;
					strcpy(options, "S");

					strcpy(reserve, menu_path);
					strcat(reserve, "bye");
					time(&time_out_start);
					get_menu(reserve, 1);
					rfile(title_file, 1);
					hot_of = OFF;

					log_out();
				} else if (menu[do_function].function == 4) {
					strcpy(options, "S");
					more_select(ON);
					hot_of = ON;
					rfile(menu[do_function].command, 0);
					hot_of = OFF;
					more_select(OFF);
					press_return();
				} else if (menu[do_function].function == 6) {
					correct(0);
				} else if (menu[do_function].function == 7) {
					strcpy(response, "Listing Users");

					strcat(response, "\n");

					write_system_log();

					send_clear_screen();
					list_users();

				} else if (menu[do_function].function == 8) {
					batch();
				} else if (menu[do_function].function == 10) {
					strcpy(reserve, menu[do_function].command);
					strcpy(save_menu, reserve);
					men(save_menu);

					show = ON;
				} else if (menu[do_function].function == 11) {
					scan_new_mail();
				} else if (menu[do_function].function == 12) {
					upload_header();
					list_protocols();

					if (proto != 13)
						upload();
					else {
						p("[0;1;33;40m", "\n\nUploading of file ");
						p("[31m", "ABORTED");
						p("[37m", ".\n");
					}
				} else if (menu[do_function].function == 13) {
					list_files();
				} else if (menu[do_function].function == 15) {

					strcpy(response, "Paging SysOp\n");
					write_system_log();

					page();
				} else if (menu[do_function].function == 16) {
					who(ON);
				} else if (menu[do_function].function == 20) {
					show_user();
					men(save_menu);
					show = ON;
				} else if (menu[do_function].function == 21) {
					new_file_search();
				} else if (menu[do_function].function == 22) {
					search_files();
				} else if (menu[do_function].function == 23) {

					get_text_to_search_for();
				} else if (menu[do_function].function == 24) {

					file_stats(0);

				} else if (menu[do_function].function == 25) {
					quick_scan = ON;

					read_type();
					quick_scan = OFF;
					more_select(OFF);

				} else if (menu[do_function].function == 26) {

					strcpy(save_it, user.ansi);

					if (user.ansi[0] == '3')
						user.ansi[0] = '2';
					header_scan = ON;
					read_type();
					header_scan = OFF;
					strcpy(user.ansi, save_it);
					more_select(OFF);
				}

				else if (menu[do_function].function == 27) {

					execute_door();
				}
			}

			if (!show && flag) {
				strcpy(options, mess_options);
				hot_of = ON;
				rfile(menu_file, 0);
				hot_of = OFF;
			} else
				show = OFF;
		}
	}
	return 0;
}

void log_out() {

	FILE *fp;
	FILE *id;

	struct index message_data;
	size_t rec_size_idx = sizeof(struct index);
	size_t rec_size2 = sizeof(struct user_info);

	long int counter, work, tmp;

	unsigned char r, c;

	g_date();
	strcpy(user.log_out_time, s_time);
	strcpy(user.last_date_called, s_date);

	work = atol(user.total_calls);
	work++;
	ltoa(work, user.total_calls, 10);

	work = atol(display_time);

	if (work <= 0)
		work = 0;

	work = 60L - work;
	ltoa(work, user.time_used, 10);

	for (counter = 0; counter < number_of_mess_areas; counter++) {
		if (lr_pnt.scan[counter] == 'Y') {

			file_name("idx", counter + 1);

			if ((id = fopen(reserve, "rb+")) == NULL) {
				printf("can't open");
				exit(1);
			}

			fseek(id, 0L, 0);
			fread(&message_data, rec_size_idx, 1, id);

			lr_pnt.mes_pnt[counter] = message_data.parent + 1;
			fclose(id);
		}
	}

	user_file_name("bbs");

	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.bbs\n");
		exit(1);
	}

	fseek(fp, user_rec_num * rec_size2, 0);
	fwrite(&user, rec_size2, 1, fp);
	fclose(fp);

	user_file_name("pnt");
	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.pntaaa\n");
		exit(1);
	}

	fseek(fp, user_rec_num * sizeof(struct last_read), 0);
	fwrite(&lr_pnt, sizeof(struct last_read), 1, fp);
	fclose(fp);

	user_file_name("fil");
	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.fil\n");
		exit(1);
	}

	for (tmp = 0; tmp < number_of_file_areas; tmp++) {

		g_date();

		if (l_scan_date2.touched[tmp] == 'Y')

			strcpy(l_scan_date.last_date[tmp], s_date);
		strcpy(l_scan_date.last_time[tmp], s_time);
	}

	fseek(fp, user_rec_num * sizeof(struct last_scan_date), 0);
	fwrite(&l_scan_date, sizeof(struct last_scan_date), 1, fp);

	p("[0;1;32;40m", "");

	fclose(fp);

	/*
	cls(0,0,24,79);

	for (r=0;r<24;r++) {
	   for (c=0;c<80;c++) {
	  write_char(r,c,screen[index[r]].chrtr[c],
	  screen[index[r]].atrib[c],0);
	   }
	}
   */

	delay(1000);

	g_date();
	strcpy(response, "Logged off at ");
	strcat(response, s_time);
	strcat(response, "\n\n");

	write_system_log();

	fclose(system_log);
	close_port();
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void rfile(unsigned char *filename, unsigned char fullscreen) {

	FILE *fp;

	unsigned char get_txt[81], sz, cnt, tmp, i, j, r, c, save_bottom;
	long int sum = 0, lngth, tot = 0, cnto = 79;

	file_scroll = ON;
	under_bar = 0;
	abort_file = OFF;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("cannot open %s111", filename);
		exit(1);
	}

	p("[0;1m", "");

	if (fullscreen) {
		save_bottom = bottom;
		bottom = 25;
	}

	while (!feof(fp)) {

		strcpy(get_txt, blnk);
		get_txt[80] = '\0';

		sum = fread(get_txt, 80, 1, fp);

		get_txt[81] = '\0';

		if (!sum) {
			while (get_txt[cnto] != '\n' && cnto >= 0) {
				cnto--;
			}
			get_txt[cnto] = '\0';
		}

		p("", get_txt);

		if (!more_r && more_of) {
			fclose(fp);
			break;
		}

		if (abort_file) {
			if (abort_file == 2) {
				p("[0;1;40;35m", "[");
				p("[33m", display_time);
				p("[36m", " min.");

				p("[35m", "]: ");
				p("[37m", "");
			}
			fclose(fp);
			break;
		}
	}
	fclose(fp);

	file_scroll = OFF;

	if (fullscreen) {

		if (under_bar >= 19 && s_bar || under_bar >= 24 && !s_bar) {
			if (s_bar)
				sz = under_bar - 19;
			else
				sz = under_bar - 24;

			for (cnt = 0; cnt < sz + 1; cnt++) {
				scroll_window(upy, upx + 1, lowy, lowx + 1, 1);
				for (tmp = upy; tmp < lowy + 1; tmp++) {
					write_char(lowx + 1, tmp, screen[index[lowx + 1]].chrtr[tmp],
					           screen[index[lowx + 1]].atrib[tmp], 0);
				}

				xloc = lowx;
				tmp = (unsigned char)index[0];

				for (i = 0; i < bottom; i++)
					index[i] = index[i + 1];

				index[bottom] = tmp;

				for (j = 0; j < 80; j++) {
					screen[index[bottom]].atrib[j] = 7;
					screen[index[bottom]].chrtr[j] = 32;
				}
			}
		}

		bottom = save_bottom;
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void delay_screen(unsigned int delay_loops) {

	unsigned int loop;

	for (loop = 0; loop < delay_loops; loop++) {
		do_nothing();
	}
}

/*
void delay_s(unsigned char seconds) {

    time_t tstart, tend;
    time( &tstart );
    time( &tend );


    while (difftime (tend, tstart) < (double) seconds) {
       time( &tend );
    }

}

*/
unsigned char file_exist(unsigned char *filename) {

	if (access(filename, 00) == 0) {
		return 0;
	} else
		return 1;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void scan_new_mail() {

	long int counter, area_cnt, num_mes;
	FILE *wh;
	FILE *id;

	struct who who_tf;
	struct index message_data;
	size_t rec_size_who = sizeof(struct who);
	size_t rec_size_idx = sizeof(struct index);

	unsigned char first_pass = OFF, work[40], small[10], tmp, display_area = ON;
	unsigned char mail_counter, rescan = ON, flag2, check, tot, c, flag = ON;

	new_mail_read = ON;
	send_clear_screen();

	p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
	p("[40m", "\n");
	p("[44m", "บ");
	p("[41;33m", "  Scanning for New Mail  ");
	p("[33;44m", "บ");
	p("[40m", "\n");
	p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

	p("[0;1;40;36m", "\n\nScanning");
	p("[33m", " TAGGED ");
	p("[36m", "message areas for new mail...");

	while (flag) {
		if (rescan) {
			mail_counter = 0;

			for (counter = 0; counter < number_of_mess_areas; counter++) {
				if (lr_pnt.scan[counter] == 'Y') {

					/*

					    ltoa(counter+1,response,10);

					    p("[33m",response);
					    p("[31m",", ");
					    p("[32m",mess_name[counter].name);

					*/

					file_name("who", counter + 1);

					if ((wh = fopen(reserve, "rb+")) == NULL) {
						printf("can't open!!");
						exit(1);
					}

					file_name("idx", counter + 1);
					if ((id = fopen(reserve, "rb+")) == NULL) {
						printf("can't open");
						exit(1);
					}

					fseek(id, 0L, 0);
					fread(&message_data, rec_size_idx, 1, id);

					num_mes = message_data.parent;
					strcpy(reserve, first);
					strcat(reserve, " ");
					strcat(reserve, last);

					for (area_cnt = lr_pnt.mes_pnt[counter]; area_cnt <= num_mes; area_cnt++) {

						fseek(wh, area_cnt * rec_size_who, 0);
						fread(&who_tf, rec_size_who, 1, wh);
						if (strcmp(who_tf.to, reserve) == NULL) {

							fseek(id, area_cnt * rec_size_idx, 0);
							fread(&message_data, rec_size_idx, 1, id);

							if (message_data.delete[0] != 'Y') {

								if (!first_pass) {
									p("", "\n\n");
									p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
									p("[40m", "\n");
									p("[44m", "บ");
									p("[41;33m", "  Area           Msg #   Private  From                Subject              ");
									p("[33;44m", "บ");
									p("[40m", "\n");
									p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
									p("[0;40m", "\n");
									first_pass = ON;
								}

								if (display_area) {
									ltoa(counter + 1, work, 10);
									p("[1;36m", " ");
									p("", work);
									tot = 0;
									tot = tot + strlen(work) + 2;
									p("[31m", ", ");
									p("[33m", mess_name[counter].name);
									tot = tot + strlen(mess_name[counter].name);
									work[0] = '\0';
									strncat(work, blnk, 18 - tot);
									p("", work);
									display_area = OFF;
								} else
									p("[36m", "                   ");

								new_mail[mail_counter].mes_num = area_cnt;
								new_mail[mail_counter].area = counter + 1;

								mail_counter++;

								ltoa(area_cnt, work, 10);

								format(work, 8);
								p("[36m", response);

								if (message_data.private[0] == 'Y')
									strcpy(work, "Yes");
								else
									strcpy(work, "No");

								format(work, 8);
								p("[31m", response);

								format(who_tf.from, 20);

								p("[32m", response);

								format(message_data.subject, 22);
								p("[36m", response);
								p("", "\n");
							}
						}
					}
					fclose(wh);
					fclose(id);
					display_area = ON;
				}
			}
		}

		rescan = OFF;
		first_pass = OFF;

		if (mail_counter) {

			p("[36;40m", "\nView");
			p("[35m", " [");
			p("[33m", "A");
			p("[35m", "]");
			p("[32m", "ll");

			p("[35m", " [");
			p("[33m", "M");
			p("[35m", "]");
			p("[32m", "essage Area");

			p("[35m", " [");
			p("[33m", "I");
			p("[35m", "]");
			p("[32m", "ndividual");

			p("[35m", " [");
			p("[33m", "L");
			p("[35m", "]");
			p("[32m", "ist");

			p("[35m", " [");
			p("[33m", "Q");
			p("[35m", "]");
			p("[32m", "uit");
			p("[31m", ": ");

			if (user.ansi[0] != '1') {
				p("[37;44m", " ");
				p("[1D", "");
			}

			strcpy(options, "AMILQ");

			c = hot_key();

			switch (c) {

			case 'Q':
				flag = OFF;
				break;

			case 'L':
				rescan = ON;
				break;

			case 'A':

				do_function = 0;

				for (counter = 0; counter < mail_counter; counter++) {
					ltoa(new_mail[counter].area, work, 10);
					strcpy(menu[do_function].command, work);
					read_message(new_mail[counter].mes_num, 'F');

					if (new_mail_read == 2) {
						new_mail_read = ON;
						break;
					}
				}
				p("", "\n");
				break;

			case 'M':

				flag2 = ON;

				while (flag2) {
					check = OFF;
					get_message_area();
					ltoa(number_of_mess_areas, response, 10);
					area_cnt = strlen(response);
					get_number((unsigned char)area_cnt, "");

					if (!strlen(response)) {
						p("[40m", "\n");
						break;
					}

					area_cnt = atol(response);

					if (area_cnt < 1 || area_cnt > number_of_mess_areas)
						p("[40;31m", "\n\nThat is not a valid message area.");
					else
						check = ON;

					tot = OFF;

					if (check) {
						for (counter = 0; counter < mail_counter; counter++) {
							if (new_mail[counter].area == area_cnt) {
								tot = ON;
								break;
							}
						}

						if (tot) {
							flag2 = OFF;

							for (counter = counter; counter < mail_counter; counter++) {
								ltoa(area_cnt, work, 10);
								strcpy(menu[do_function].command, work);
								if (new_mail[counter].area != area_cnt)
									break;
								read_message(new_mail[counter].mes_num, 'F');
								if (new_mail_read == 2) {
									new_mail_read = ON;
									break;
								}
							}
							p("", "\n");
						} else
							p("[40;31m", "\n\nThere is no mail waiting for you in that area.");
					}
				}
				break;

			case 'I':
				flag2 = ON;

				while (flag2) {
					check = OFF;

					get_message_area();
					ltoa(number_of_mess_areas, response, 10);
					area_cnt = strlen(response);
					get_number((unsigned char)area_cnt, "");

					if (!strlen(response)) {
						p("[40m", "\n");
						break;
					}

					area_cnt = atol(response);

					if (area_cnt < 1 || area_cnt > number_of_mess_areas)
						p("[40;31m", "\n\nThat is not a valid message area.");
					else
						check = ON;

					tot = OFF;

					if (check) {
						for (counter = 0; counter < mail_counter; counter++) {
							if (new_mail[counter].area == area_cnt) {
								tot = ON;
								break;
							}
						}

						if (tot)
							flag2 = OFF;
						else
							p("[40;31m", "\n\nThere is no mail waiting for you in that area.");
					}
				}

				if (!check)
					break;

				flag2 = ON;

				while (flag2) {
					p("[40;33m", "\n\nEnter");
					p("[32m", " message number ");
					p("[33m", "to read: ");

					if (user.ansi[0] != '1') {
						p("[37;44m", "     ");
						p("[5D", "");
					}

					get_number(5, "");

					if (!strlen(response)) {
						p("[40m", "\n");
						break;
					}

					num_mes = atol(response);

					tot = OFF;

					for (counter = 0; counter < mail_counter; counter++) {
						if (new_mail[counter].area == area_cnt && new_mail[counter].mes_num == num_mes) {
							tot = ON;
							break;
						}
					}

					if (tot) {
						flag2 = OFF;
						ltoa(area_cnt, work, 10);
						strcpy(menu[do_function].command, work);
						read_message(num_mes, 'F');

						if (new_mail_read == 2) {
							new_mail_read = ON;
							break;
						}
						p("", "\n");
					} else
						p("[40;31m", "\n\nThat message is not waiting for you to read.");
				}
				break;
			}
		} else {

			p("[40;32m", "\n\nSorry, there is no");
			p("[31m", " mail ");
			p("[32m", "waiting for you...\n");
			flag = OFF;
			press_return();
		}
	}
	new_mail_read = OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_message_area() {

	long int area_cnt;
	unsigned char work[41];

	p("[40;33m", "\n\nEnter the ");
	p("[32m", "message area ");
	p("[33m", "you wish to read you mail from ");
	p("[35m", "[");
	p("[36m", "1 - ");

	ltoa(number_of_mess_areas, response, 10);

	p("", response);
	p("[35m", "]");
	p("[33m", ": ");

	area_cnt = strlen(response);

	work[0] = '\0';
	strncat(work, blnk, (unsigned char)area_cnt);

	if (user.ansi[0] != '1') {
		p("[37;44m", work);
		work[0] = '\0';
		strcat(work, "[");
		ltoa((long int)strlen(response), response, 10);
		strcat(work, response);
		strcat(work, "D");
		p(work, "");
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void list_users() {

	unsigned char finish = OFF, flag = ON;

	struct user_idx {
		unsigned char name[41];
		long int left;
		long int right;
	} user_tree;

	struct user_info user_temp;

	long int stack[50];
	long int start = 1, sz = 0, counter = 0, last;

	size_t rec_size = sizeof(struct user_idx);
	size_t rec_size2 = sizeof(struct user_info);

	long int number;

	FILE *fp;
	FILE *in;

	more_select(ON);
	p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
	p("[40m", "\n");
	p("[44m", "บ");
	p("[41;33m", "  Name                 City                  Last Called       # of Calls  ");
	p("[33;44m", "บ");
	p("[40m", "\n");
	p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
	p("[0;40m", "\n"); /*|*/

	user_file_name("idx");

	if ((fp = fopen(reserve, "rb+")) == NULL) {
		printf("Cannot open users.idx\n");
		exit(1);
	}

	user_file_name("bbs");
	if ((in = fopen(reserve, "rb+")) == NULL) {
		printf("cannot open users.bbs\n");
		exit(1);
	}

	fseek(fp, 1L * rec_size, 0);
	fread(&user_tree, rec_size, 1, fp);

	stack[counter++] = 1L;

	while (flag) {
		while ((user_tree.left)) {
			if (user_tree.left) {
				fseek(fp, user_tree.left * rec_size, 0);
				stack[counter++] = user_tree.left;
				fread(&user_tree, rec_size, 1, fp);
			}
		}

		counter--;

		fseek(in, stack[counter] * rec_size2, 0);
		fread(&user_temp, rec_size2, 1, in);

		last = 0;
		while ((!user_tree.right) || (user_tree.right == last)) {

			last = stack[counter];
			if (!more_r && more_of) {
				grab_ansi = OFF;
				counter = -1;
				break;
			}

			display_users(&user_temp, user_tree.name);

			counter--;

			if (counter == -1)
				break;

			fseek(fp, stack[counter] * rec_size, 0);
			fread(&user_tree, rec_size, 1, fp);

			fseek(in, stack[counter] * rec_size2, 0);
			fread(&user_temp, rec_size2, 1, in);
		}

		if (counter == -1)
			flag = OFF;
		else {
			display_users(&user_temp, user_tree.name);
			user_tree.left = user_tree.right;
		}
	}
	fclose(fp);
	fclose(in);

	more_select(OFF);
	press_return();
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_users(struct user_info *user_temp, unsigned char name[]) {

	long int calls;

	p("[0;1;40;36m", "  ");
	format(name, 20);
	p("", response);

	format(user_temp->city, 20);
	p("[32m", response);

	format(user_temp->last_date_called, 11);

	mid(response, 0, 2);
	p("[33m", response);
	p("[35m", "-");

	mid(response, 3, 2);
	p("[33m", response);
	p("[35m", "-");

	mid(response, 8, 2);
	p("[33m", response);
	p("", "  ");

	format(user_temp->log_out_time, 14);
	p("[36m", response);

	p("[31m", user_temp->total_calls);

	p("", "\n");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void format(unsigned char p[], unsigned char length) {

	unsigned char counter, flag = OFF;

	strcpy(response, p);

	if (strlen(response) == length)
		return;

	for (counter = 0; counter < length; counter++) {
		if ((response[counter] == '\n') || (flag))
			response[counter] = 32;
		else if (response[counter] == '\0') {
			response[counter] = 32;
			flag = ON;
		}
	}

	if (!flag)
		response[counter - 1] = '%';

	response[counter] = '\0';
}

void page() {

	int note;
	int count, lngth;
	unsigned char c, flag = ON;
	long int more_save;

	while (flag) {
		p("[0;1;40;33m", "\n\nEnter chat subject: ");
		if (user.ansi[0] != '1') {
			p("[37;44m", "                                        ");
			p("[40D", "");
		}
		get_single_line(40, 1);
		if (response[0] == '\n') {
			p("[0;1;40;36m", "\n\nPage ");
			p("[31m", "Aborted");
			p("[37m", ".");
			return;
		} else if (strlen(response) < 5) {
			p("[0;1;40;32m", "\n\nPlease enter a longer");
			p("[31m", " chat ");
			p("[32m", "subject");
			p("[37m", "...");
		} else
			flag = OFF;
	}

	p("[0;1;40;36m", "\n\nPaging ");
	p("[32m", "Mark Longo");
	p("[36m", ": ");

	response[0] = '\0';

	for (count = 0; count < 20; count++) {
		note = 0;
		lngth = 14;

		c = (unsigned char)inkey();

		if (c == 'c' && user_input == OFF || c == 'C' && user_input == OFF) {

			chat_flag = ON;
			more_save = more_of;
			more_of = OFF;

			p("[0;1;32;40m", "\n\n");
			p("[44m", "ีออออออออออออออออออออออออออต");

			p("[33;41m", " Mark is here to chat ");
			p("[32;44m", "ฦอออออออออออออออออออออออออออธ\n\n");
			p("[36;40m", "Hi there ");
			p("[33m", first);
			p("[37m", "...\n\n");

			chat();

			p("[0;1;32;44m", "\nิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ");

			more_of = (unsigned char)more_save;

			chat_flag = OFF;

			return;
		}

		while (song[note])
			beep(song[note++], song[note++]);

		p("[0;31m", "þ");
		p("[32m", "Û");
		p("[31m", "Û");
		p("[33m", "Û");
		p("[34m", "Û");
		p("[35m", "Û");
		p("[36m", "Û");
		p("[37m", "Û");
		p("[1;32m", "Û");
		p("[31m", "Û");
		p("[33m", "Û");
		p("[34m", "Û");
		p("[35m", "Û");
		p("[36m", "Û");
		p("[37m", "Û");

		delay(200);
		delete ((unsigned char)lngth);
	}

	p("[0;1;40;33m", "\n\nMark Longo");
	p("[32m", " has been ");
	p("[31m", "paged");
	p("[37m", "...");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void beep(unsigned duration, unsigned frequency) {

	int control;

	if (frequency) {

		if (duration < 75)
			duration = 75;
		outp(0x43, 0xb6);
		frequency = (unsigned)(1193180L / frequency);
		outp(0x42, (char)frequency);
		outp(0x42, (char)(frequency >> 8));
		control = inp(0x61);
		outp(0x61, control | 0x3);
	}
	delay(100);
	if (frequency)
		outp(0x61, control);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void delay_speaker(clock_t wait) {

	clock_t goal;
	goal = wait + clock();
	while (goal > clock())
		;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void shell_to_dos() {

	char savdcolr, lbuf[2];
	char cmdcom[60];
	unsigned char *save_s;

	int x, y, atr;

	save_s = (unsigned char *)malloc(2 * (25) * (80));

	if (!save_s)
		exit(1);

	save_video(0, 25, 0, 80, save_s);
	cls(0, 0, 24, 79);

	x = xloc;
	y = yloc;

	xloc = 0;
	yloc = 0;

	goto_xy(xloc, yloc);
	atr = atrib;

	/*
	p("[0;1;5;40;31m","þ");
	p("[0;1;36m","Type ");
	p("[33m","EXIT ");
	p("[36m","to return to");
	p("[32m"," Ziola Bbs.");
	*/

	*cmdcom = '\0';
	strcpy(cmdcom, "COMMAND.COM");

	spawnlp(P_WAIT, cmdcom, cmdcom, NULL);

	restore_video(0, 25, 0, 80, save_s);
	xloc = x;
	yloc = y;

	goto_xy(xloc + 1, yloc);
	free(save_s);
	atrib = atr;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void chat() {

	unsigned char flag = ON;
	int c;
	unsigned char send[2], *r, *f;

	unsigned char chat_color, temp_c, save_b_bar;
	unsigned char colr = OFF;
	unsigned char build[41];
	register char counter = 0, back = 0;

	save_b_bar = blue_bar;
	blue_bar = OFF;

	r = &user_chat_color;
	f = &sysop_chat_color;
	send[1] = '\0';
	clear_input();

	chat_color = (unsigned char)!user_input;

	while (flag) {
		c = 0;
		while (c == 0 && colr == OFF)
			c = inkey();
		if (colr == ON) {
			c = temp_c;
			colr = OFF;
		}

		if (user_input == 1 && chat_color == 0) {
			chat_color = ON;
			message_color((int)user_chat_color);
		}

		else if (user_input == 0 && chat_color == 1) {
			message_color((int)sysop_chat_color);
			chat_color = OFF;
		}
		if (c == 1) {

			if (user_input)
				temp_c = get_colr(r);
			else
				temp_c = get_colr(f);
			colr = ON;
		}

		else if (c == 13) {
			p("", "\n");
			counter = 0;
		} else if (c == 27 && user_input == OFF)
			flag = OFF;

		else if (c == 8 && counter > 0) {
			delete (1);
			counter--;
			response[counter] = 32;

		} else if (c != 8 && counter < 80 && colr == OFF) {
			send[0] = (unsigned char)c;
			p("", send);
			response[counter] = (unsigned char)c;
			counter++;
		}

		if (counter > 78) {
			counter--;
			while (response[counter] != 32 && counter > 40) {
				build[back] = response[counter];
				back++;
				counter--;
			}
			build[back] = '\0';
			if (back != 38) {
				delete (back);
				p("", "\n");
				counter = (char)strlen(build);
				back = 0;
				strrev(build);
				strcpy(response, build);
				p("", build);

			} else {
				back = 0;
				p("", "\n");
				counter = 0;
			}
		}
	}

	blue_bar = save_b_bar;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char hot_key() {

	unsigned char flag = ON, c;
	while (flag) {
		c = 0;
		while (c == 0)
			c = (unsigned char)inkey();
		c = upper(c);
		send[0] = c;
		if (strstr(options, send) != NULL) {
			flag = OFF;
			p("", send);
		} else if (c == 13) {
			flag = OFF;
		}
	}
	return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void show_user() {

	unsigned char user_len, c, flag;

	while (flag) {
		strcpy(response, first);
		strcat(response, " ");
		strcat(response, last);

		user_len = strlen(response);

		p("[0;1;40m", "\n\n");

		p("[33;44m", "ีอต");
		p("[36;41m", " User stats for ");
		p("[32;41m", response);
		p("[33;41m", " ");
		p("[33;44m", "ฦ");

		response[0] = '\0';

		strncat(response, line, 54 - user_len);

		p("", response);
		p("", "ธ");
		p("[40m", "\n");
		p("[44m", "ณ");
		p("[41m", "                                                                          ");
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;36m", "  A");
		p("[35m", ") ");
		p("[41;32m", "City      : ");
		format(user.city, 20);
		p("[37m", response);

		p("[41;36m", "  F");
		p("[35m", ") ");
		p("[41;32m", "Ansi          : ");
		format(user.ansi, 16);
		p("[37m", response);
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;36m", "  B");
		p("[35m", ") ");
		p("[41;32m", "Voice     : ");
		display_phone(user.voice);

		p("[41;36m", "        G");
		p("[35m", ") ");
		p("[41;32m", "Screen length : ");
		format(user.screen_length, 16);
		p("[37m", response);
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;36m", "  C");
		p("[35m", ") ");
		p("[41;32m", "Data      : ");
		display_phone(user.data);

		p("[41;36m", "        H");
		p("[35m", ") ");
		p("[41;32m", "More Prompt   : ");
		format(user.more, 16);
		p("[37m", response);
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;36m", "  D");
		p("[35m", ") ");
		p("[41;32m", "Password  : ");
		format(user.pword, 20);
		p("[37m", response);

		p("[41;36m", "  I");
		p("[35m", ") ");
		p("[41;32m", "Screen Clear  : ");
		format(user.screen_clear, 16);
		p("[37m", response);
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;36m", "  E");
		p("[35m", ") ");
		p("[41;32m", "Birthdate : ");

		display_bdate(user.bdate);

		p("[41;36m", "            J");
		p("[35m", ") ");
		p("[41;32m", "Sex           : ");
		format(user.sex, 16);
		p("[37m", response);
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41m", "                                                                          ");
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Security Level  : ");
		format(user.secur, 15);
		p("[37m", response);

		p("[41;33m", "Time Used         : ");
		format(user.time_used, 16);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Total Calls     : ");
		format(user.total_calls, 15);
		p("[37m", response);

		p("[41;33m", "Total Calls Today : ");
		format(user.total_calls_today, 16);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Last Called     : ");
		format(user.last_date_called, 15);
		p("[37m", response);

		p("[41;33m", "Log In Time       : ");
		format(user.log_in_time, 16);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Messages Posted : ");
		format(user.messages_posted, 15);
		p("[37m", response);

		p("[41;33m", "Bps rate          : ");
		format(connect_at, 16);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41m", "                                                                          ");
		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Total Download # : ");
		format(user.dl_num, 14);
		p("[37m", response);

		p("[41;33m", "Total Download KBytes : ");
		format(user.dl_kb, 12);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Download # Today : ");
		format(user.dl_num_today, 14);
		p("[37m", response);

		p("[41;33m", "Download KBytes Today : ");
		format(user.dl_kb_today, 12);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Total Upload #   : ");
		format(user.ul_num, 14);
		p("[37m", response);

		p("[41;33m", "Total Upload KBytes   : ");
		format(user.ul_kb, 12);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41;33m", "     Upload # Today   : ");
		format(user.ul_num_today, 14);
		p("[37m", response);

		p("[41;33m", "Upload KBytes Today   : ");
		format(user.ul_kb_today, 12);
		p("[37m", response);

		p("[44m", "ณ");
		p("[40m", "\n");

		p("[44m", "ณ");
		p("[41m", "                                                                          ");
		p("[44m", "ณ");

		p("[40m", "\n");

		p("[33;44m", "ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ");
		p("[40m", "\n");
		mistake = ON;

		p("[31;40m", "\nEnter Selection: ");

		if (user.ansi[0] != '1') {
			p("[34;44m", " ");
			p("[1D", "");
		}

		strcpy(options, "ABCDEFGHIJ");

		c = hot_key();
		switch (c) {

		case 13:
			flag = OFF;
			break;
		case 'A':
			get_city();
			if (cut[1].level[0] == '\0')
				p("[40;31m", "\n\nChange city has been aborted.");
			else
				strcpy(user.city, response);
			break;

		case 'B':
			voice();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nVoice phone change has been aborted.");
			else
				strcpy(user.voice, response);
			break;

		case 'C':
			data();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nData phone change has been aborted.");
			else
				strcpy(user.data, response);
			break;

		case 'D':
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
				p("", "\n\nScreen length aborted.");
			else
				strcpy(user.screen_length, response);
			break;

		case 'H':
			select_more_prompt();
			if (response[0] == 1)
				p("[40;31m", "\n\nSelect more prompt has been aborted.");
			else
				strcpy(user.more, response);
			break;

		case 'E':
			bdate();
			if (strlen(response) == OFF)
				p("[40;31m", "\n\nBirthdate change has been aborted.");
			else
				strcpy(user.bdate, response);
			break;

		case 'J':
			m_or_f();
			if (response[0] == 1)
				p("[40;31m", "\n\nChange sex has been aborted. [thank god]");
			else
				strcpy(user.sex, response);
			break;

		case 'I':
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

void do_nothing() {
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char hot_key_number() {

	unsigned char flag = ON, c, dash;
	unsigned char it[80], cnt = 0, sz = 0;
	unsigned char lngth[20], num_val = 0, space = OFF;

	p("[40;37m", "");
	big = 0;

	while (flag) {
		c = 0;
		while (c == 0)
			c = (unsigned char)inkey();

		if (isdigit(c)) {

			it[cnt++] = c;
			sz++;

			send[0] = c;
			p("", send);

			dash = OFF;

			while (c != 13) {
				c = 0;
				while (c == 0)
					c = (unsigned char)inkey();

				if (isdigit(c) && sz < 3) {
					send[0] = c;
					p("", send);
					it[cnt++] = c;
					sz++;
				} else if (c == '-' && !dash && !space) {
					dash = ON;
					p("[32m", "-");
					p("[37m", "");
					it[cnt++] = c;
					sz = 0;
				} else if (c == ' ' && num_val < 5 && sz > 0 && !dash || c == ',' && num_val < 5 && sz > 0 && !dash) {
					space = ON;
					it[cnt++] = ' ';
					p("[32m", ",");
					p("[37m", "");
					lngth[num_val++] = sz;
					sz = 0;
				} else if (c == 8 && cnt > 0) {
					delete (1);
					cnt--;
					sz--;
					if (it[cnt] == '-') {
						sz = cnt;
						dash = OFF;
					} else if (it[cnt] == ' ') {
						num_val--;
						if (num_val == 0)
							space = OFF;
						sz = lngth[num_val];
					}

					if (cnt == 0)
						c = 13;
				} else if (c == 13) {
					flag = OFF;
					it[cnt] = '\0';
					big = strlen(it);
					if (it[cnt - 1] == '-')
						cnt--;

					it[cnt] = '\0';

					if (it[0] == '\0')
						one[0] = '\0';
					else {
						cnt = 0;
						sz = 0;
						kind = OFF;
						if (dash) {
							while (it[cnt] != '-' && it[cnt] != '\0')
								one[sz++] = it[cnt++];
							one[sz] = '\0';

							if (it[cnt] == '-') {
								cnt++;
								sz = 0;
								while (it[cnt] != '\0')
									two[sz++] = it[cnt++];

								two[sz] = '\0';
							} else
								two[0] = '\0';
						}

						else {
							kind = ON;
							strcpy(response, it);
							cut_string();
						}
					}
				}
			}
			c = 0;
		} else {
			c = upper(c);
			send[0] = c;

			if ((search_f && c == 'Q') || (text_search && c == 'Q') || (scan_new_files && c == 'Q')) {
				more_r = OFF;
				strcat(options, "Q");
			}

			if (strstr(options, send) != NULL) {
				flag = OFF;
				p("", send);

			} else if (c == 13) {
				flag = OFF;
			}
		}
	}
	return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void new_file_search(void) {

	unsigned char trim[15], flag = ON, why[15], save_it[15], c, the_same = OFF, tmp;

	unsigned char true_val[15];

	long value, value2;

	long int area;

	send_clear_screen();

	display_f_area(4);

	p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
	p("[40m", "\n");
	p("[44m", "บ");
	p("[41;33m", "  New File Search  ");
	p("[33;44m", "บ");
	p("[40m", "\n");
	p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

	while (flag) {

		p("[0;1;33;40m", "\n\nLast scan date for this area");

		p("[34m", " [");
		p("[35m", "MM-DD-YY");
		p("[34m", "]");
		p("[31m", ": ");
		if (user.ansi[0] != '1') {
			p("[37;44m", "        ");
			p("[8D", "");
		}

		area = atol(menu[do_function].command);
		strcpy(why, l_scan_date.last_date[area - 1]);

		get_date_scan(why);

		trim[0] = response[0];
		trim[1] = response[1];
		trim[2] = '\0';
		value = atoi(trim);
		trim[0] = response[3];
		trim[1] = response[4];

		value2 = atoi(trim);

		if (value > 12 || value < 1)
			p("[33;40m", "\n\nMonth must be between 1 and 12.");
		else if (value2 > 31 || value2 < 1)
			p("[33;40m", "\n\nDay must be between 1 and 31.");
		else {
			flag = OFF;
			response[8] = response[6];
			response[9] = response[7];
			response[6] = '1';
			response[7] = '9';
			response[10] = '\0';
			strcpy(true_val, response);
		}
	}

	if (strstr(l_scan_date.last_date[area - 1], true_val))
		the_same = ON;

	search_which();

	strcpy(options, "ATQ");

	c = hot_key();

	if (c == 'A') {

		if (!the_same) {

			for (tmp = 0; tmp < number_of_file_areas; tmp++) {

				strcpy(l_scan_date.last_date[tmp], true_val);
				strcpy(l_scan_date.last_time[tmp], "12:00 am");
			}
		}

		log_in_scan_files(0);

		if (!the_same) {

			for (tmp = 0; tmp < number_of_file_areas; tmp++) {

				strcpy(l_scan_date.last_date[tmp], l_scan_date2.last_date[tmp]);
				strcpy(l_scan_date.last_time[tmp], l_scan_date2.last_time[tmp]);
			}
		}

	}

	else if (c == 'T') {

		g_date;

		strcpy(save_it, l_scan_date.last_date[area - 1]);

		strcpy(l_scan_date.last_date[area - 1], true_val);

		scan_new_files = ON;
		list_files();
		scan_new_files = OFF;

		strcpy(l_scan_date.last_date[area - 1], save_it);

		if (found_new_files == OFF)
			p("[0;1;40;36m", "\n\nSorry, no new files found since your last login.\n");
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void log_in_scan_files(unsigned char skip) {

	int areas;
	unsigned char save_it[30];
	unsigned char found_files = OFF;

	strcpy(save_it, menu[do_function].command);

	if (skip) {

		send_clear_screen();

		p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
		p("[40m", "\n");
		p("[44m", "บ");
		p("[41;33m", "  Scanning for New Files  ");
		p("[33;44m", "บ");
		p("[40m", "\n");
		p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

		p("[36;40m", "\n\nScan for ");
		p("[33m", "NEW ");
		p("[36m", "files ");
		p("[34m", "[");
		p("[35m", "Y/n");
		p("[34m", "]");
		p("[36m", ": ");
		p("[37;44m", " ");
		p("[1D", "");

		response[0] = get_yn("YN");
		response[1] = '\0';
	} else
		response[0] = 'Y';

	if (response[0] == 'Y') {
		p("[0;1;40;32m", "\n\nScanning tagged file areas...");
		for (areas = 1; areas <= 4; areas++) {
			itoa(areas, menu[do_function].command, 10);
			scan_new_files = ON;
			list_files();
			if (found_new_files && found_files == OFF)
				found_files = ON;

			scan_new_files = OFF;

			if (!more_r)
				break;
		}

		strcpy(menu[do_function].command, save_it);

		if (found_files == OFF)
			p("[0;1;40;36m", "\n\nSorry, no new files found since your last login.\n");
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char cut_file(unsigned char test[80]) {

	unsigned char c1 = 0, c2 = 0;
	unsigned char left[12], right[4], counter2;
	unsigned char counter, flag = 1;
	unsigned char w1[30], w2[30];
	unsigned char save[40];
	unsigned char front, back;

	counter = 0;
	counter2 = 0;

	while (flag) {

		while (test[counter] != '.' && test[counter] != NULL)
			left[counter++] = test[counter];

		left[counter] = '\0';
		flag = 0;

		if (test[counter++] == '.') {
			while (test[counter] != NULL)
				right[counter2++] = test[counter++];
		}

		right[counter2] = '\0';
	}

	counter = 0;
	counter2 = 0;
	flag = ON;

	strcpy(test, search_name);

	while (flag) {

		while (test[counter] != '.' && test[counter] != NULL)
			w1[counter++] = test[counter];

		w1[counter] = '\0';
		flag = 0;

		if (test[counter++] == '.') {
			while (test[counter] != NULL)
				w2[counter2++] = test[counter++];
		}
		w2[counter2] = '\0';
	}

	strcpy(save, search_name);
	strcpy(search_name, w1);

	front = wild(left);

	strcpy(search_name, w2);
	back = wild(right);

	strcpy(search_name, save);

	if (front && back)
		return 1;
	else
		return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char wild(unsigned char l[30]) {

	unsigned char sl, sw, c1 = 0, c2 = 0;
	unsigned char w[30];

	strcpy(w, search_name);

	sl = strlen(l);
	sw = strlen(w);

	for (;;) {
		if (c1 == sl && sw == c2)
			return 1;

		if (w[c1] == '?') {
			c1++;
			c2++;

			if (c2 >= sl)
				return 1;
		}

		if (w[c1] != '?') {
			if (l[c1] == w[c2]) {
				c1++;
				c2++;

				if (c1 == sl && sw > sl && w[c2] != '*')
					return 0;
			} else if (w[c2] == '*') {
				return 1;

			} else
				return 0;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void search_files(void) {

	unsigned char flag = ON, c;
	unsigned char save_it[30];
	unsigned char all_area_found = OFF;

	long int areas = 0, area;
	strcpy(save_it, menu[do_function].command);

	other_areas = OFF;

	while (flag) {

		if (!other_areas) {
			search_f = ON;
			send_clear_screen();
			display_f_area(2);
			p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
			p("[40m", "\n");
			p("[44m", "บ");
			p("[41;33m", "  Searching for a Filename  ");
			p("[33;44m", "บ");
			p("[40m", "\n");
			p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

			p("[40m", "\n");

			p("[1;32;40m", "\nEnter ");
			p("[33m", "FILENAME ");
			p("[32m", "to search for ");
			p("[34m", "[");
			p("[35m", "*,? are ok");
			p("[34m", "]");
			p("[31m", ": ");

			if (user.ansi[0] != '1') {
				p("[34;44m", "            ");
				p("[12D", "");
			}

			p("[37;44m", "");

			get_files(12);

			if (strlen(response) == 0) {
				p("[32;40m", "\n\nSearch for file has been aborted...");
				search_f = OFF;
				return;
			}

			strupr(response);
			strcpy(search_name, response);

			search_which();

			strcpy(options, "ATQ");

			c = hot_key();

			if (c == 'A')
				other_areas = ON;
			else if (c == 13) {
				p("", "A");
				other_areas = ON;
			} else if (c == 'T')
				other_areas = OFF;
			else {
				search_f = OFF;
				return;
			}
		}

		list_files();

		if (other_areas && more_r) {
			if (found_new_files)
				all_area_found = ON;

			areas++;

			if (areas == atol(save_it))
				areas++;

			if (areas > number_of_file_areas) {
				flag = OFF;
				if (!all_area_found) {
					p("[31m", "\n\nNo matches where made in any area...\n");
					more_r = OFF;
					press_return();
				}

			} else
				ltoa(areas, menu[do_function].command, 10);
		} else {
			flag = OFF;
		}
	}
	search_f = OFF;
	other_areas = OFF;
	strcpy(menu[do_function].command, save_it);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_text_to_search_for() {

	unsigned char flag = ON, c;
	unsigned char count;

	unsigned char save_it[30];
	unsigned char all_area_found = OFF;

	long int areas = 0, area;

	strcpy(save_it, menu[do_function].command);

	text_search = ON;

	other_areas = OFF;

	while (flag) {

		st = OFF;

		if (!other_areas) {
			send_clear_screen();

			display_f_area(3);
			p("[0;1;33;44m", "ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
			p("[40m", "\n");
			p("[44m", "บ");
			p("[41;33m", "  Searching for Text  ");
			p("[33;44m", "บ");
			p("[40m", "\n");
			p("[0;1;33;44m", "ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
			p("[33;40m", "\n\nEnter text to search for: ");

			if (user.ansi[0] != '1') {
				p("[37;44m", "                                        ");
				p("[40D", "");
			}

			get_single_line(40, 1);

			if (strlen(response) == 1) {
				p("[32;40m", "\n\nSearch for text has been aborted...");
				text_search = OFF;
				return;
			}

			strcpy(search_for, response);

			count = strlen(search_for);
			search_for[count - 1] = '\0';

			search_which();

			strcpy(options, "ATQ");

			c = hot_key();

			if (c == 'A')
				other_areas = ON;
			else if (c == 13) {
				p("", "A");
				other_areas = ON;
			} else if (c == 'T')
				other_areas = OFF;
			else {
				search_f = OFF;
				return;
			}
		}

		list_files();

		if (other_areas && more_r) {
			if (found_new_files)
				all_area_found = ON;

			areas++;

			if (areas == atol(save_it))
				areas++;

			if (areas > number_of_file_areas) {
				flag = OFF;
				if (!all_area_found) {
					p("[31m", "\n\nNo matches where made in any area...\n");
					more_r = OFF;
					press_return();
				}
			} else
				ltoa(areas, menu[do_function].command, 10);
		} else {
			flag = OFF;
		}
	}

	text_search = OFF;

	other_areas = OFF;
	strcpy(menu[do_function].command, save_it);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void search_which() {

	p("[36;40m", "\n\nSearch ");
	p("[35m", "[");
	p("[33m", "T");
	p("[35m", "]");
	p("[32m", "his Area ");
	p("[35m", "[");
	p("[33m", "A");
	p("[35m", "]");
	p("[32m", "ll Areas ");
	p("[35m", "[");
	p("[33m", "Q");
	p("[35m", "]");
	p("[32m", "uit Search ");
	p("[36m", ": ");
	p("[37;44m", " ");
	p("[1D", "");
}

void file_stats(unsigned char p_return) {

	unsigned char user_len, c, flag;

	long int ul, dl, fin;

	double frac, nt;

	if (more_of == OFF)
		p_return = OFF;

	if (p_return)
		more_select(OFF);

	strcpy(response, first);
	strcat(response, " ");
	strcat(response, last);

	user_len = strlen(response);

	p("[0;1;40m", "\n\n");

	p("[33;44m", "ีอต");
	p("[36;41m", " File stats for ");
	p("[32;41m", response);
	p("[33;41m", " ");
	p("[33;44m", "ฦ");

	response[0] = '\0';

	strncat(response, line, 54 - user_len);

	p("", response);
	p("", "ธ");
	p("[40m", "\n");
	p("[44m", "ณ");
	p("[41m", "                                                                          ");
	p("[44m", "ณ");
	p("[40m", "\n");

	p("[44m", "ณ");
	p("[41;33m", "   Total Download #       : ");
	format(user.dl_num, 10);
	p("[37m", response);

	p("[41;33m", "Total Download KBytes : ");
	format(user.dl_kb, 12);
	p("[37m", response);

	p("[44m", "ณ");
	p("[40m", "\n");

	p("[44m", "ณ");
	p("[41;33m", "   Download # Today       : ");
	format(user.dl_num_today, 10);
	p("[37m", response);

	p("[41;33m", "Download KBytes Today : ");
	format(user.dl_kb_today, 12);
	p("[37m", response);

	p("[44m", "ณ");
	p("[40m", "\n");

	p("[44m", "ณ");
	p("[41m", "                                                                          ");
	p("[44m", "ณ");

	p("[40m", "\n");

	p("[44m", "ณ");
	p("[41;33m", "   Total Upload #         : ");
	format(user.ul_num, 10);
	p("[37m", response);

	p("[41;33m", "Total Upload KBytes   : ");
	format(user.ul_kb, 12);
	p("[37m", response);

	p("[44m", "ณ");
	p("[40m", "\n");

	p("[44m", "ณ");
	p("[41;33m", "   Upload # Today         : ");
	format(user.ul_num_today, 10);
	p("[37m", response);

	p("[41;33m", "Upload KBytes Today   : ");
	format(user.ul_kb_today, 12);
	p("[37m", response);

	p("[44m", "ณ");
	p("[40m", "\n");

	p("[44m", "ณ");
	p("[41m", "                                                                          ");
	p("[44m", "ณ");

	p("[40m", "\n");

	p("[33;44m", "ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ");
	p("[40m", "\n");

	/*
	    p("[44m",   "ณ");
	    p("[41;33m","   Free KBytes today      : ");
	    format(free_dl,10);
	    p("[37m",response);
	    p("[41;33m","System UL/DL Ratio    : ");

	    strcpy(reserve,ul_ratio);
	    strcat(reserve,"/");
	    strcat(reserve,dl_ratio);


	    format(reserve,12);
	    p("[37m",response);


	    p("[44m",   "ณ");
	    p("[40m","\n");



	      p("[44m",   "ณ");
	    p("[41;33m","   Free KBytes left today : ");
	    format(user.ul_kb_today,10);
	    p("[37m",response);

	    p("[41;33m","Your UL/DL Ratio      : ");


	    ul=atol(user.ul_kb);

	    dl=atol(user.dl_kb);

	    ul_dl_ratio=(double)((double)ul/(double)dl);


	    modf(ul_dl_ratio,&nt);


	    fin = (long int) (nt);


	    ltoa(fin,reserve,10);


	    strcpy(response,reserve);
	    strcat(response,"/1");


	    format(response,12);



	    p("[37m",response);
	    p("[44m",   "ณ");
	    p("[40m","\n");



	      p("[44m",   "ณ");
	    p("[41m",         "                                                                          ");
	    p("[44m",   "ณ");

	    p("[40m","\n");











	    p("[44m",   "ณ");
	    p("[41m",         "                                                                          ");
	    p("[44m",   "ณ");

	    p("[40m","\n");


	    if (system_ul_dl_ratio==.1)
	  p("[0;1;40;32m","\nYou have violated the system dl/ul ratio.\n");
	*/

	press_return();

	if (p_return)
		more_select(ON);
}

void get_date_scan(unsigned char put_it[12]) {

	unsigned char c = 0;
	char flag = ON;
	unsigned char counter = 8;

	clear_input();

	put_it[6] = put_it[8];
	put_it[7] = put_it[9];

	put_it[8] = '\0';

	p("", put_it);

	strcpy(response, put_it);

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
			if (counter == 2 || counter == 5) {
				counter--;
				delete (2);
			} else
				delete (1);
		} else if (isdigit(c) && counter < 8) {
			send[0] = c;
			p("", send);
			response[counter] = c;
			counter++;
			if (counter == 2 || counter == 5) {
				p("", "-");
				response[counter++] = '-';
			}

			else if (counter == 12) {
				p("", "]");
				flag = OFF;
			}
		} else if (c == 13)
			flag = OFF;
	}
	response[counter] = '\0';
}

void execute_door() {
	create_dorinfo();
}

void create_dorinfo() {

	int r;
	unsigned char *save_s;
	unsigned char *my_env[5];
	unsigned char *args[15], prog[80];

	FILE *id;

	char savdcolr, lbuf[2];
	char cmdcom[60];

	int x, y, atr;

	send_clear_screen();

	p("[0;1;33;40m", "Loading the game ");
	p("[32m", "...\n\n");

	if ((id = fopen("dorinfo1.def", "wt")) == NULL) {
		printf("can't open");
		exit(1);
	}

	fputs("The Circus Room\n", id);

	fputs("MARK\n", id);
	fputs("LONGO\n", id);

	if (local) {

		fputs("COM0\n", id);
		fputs("0 BAUD,N,8,1\n", id);
	} else {
		fputs("COM1\n", id);
		fputs("38400 BAUD,N,8,1\n", id);
	}

	fputs("0", id);
	fputs("\n", id);

	fputs(first, id);
	fputs("\n", id);
	fputs(last, id);
	fputs("\n", id);

	fputs(user.city, id);
	fputs("\n", id);

	if (user.ansi[0] == '1')
		fputs("0", id);
	else
		fputs("1", id);

	fputs("\n", id);

	fputs(user.secur, id);
	fputs("\n", id);

	fputs(display_time, id);
	fputs("\n", id);

	fclose(id);

	save_s = (unsigned char *)malloc(2 * (25) * (80));

	if (!save_s)
		exit(1);

	save_video(0, 25, 0, 80, save_s);

	cls(0, 0, 24, 79);

	x = xloc;
	y = yloc;

	xloc = 0;
	yloc = 0;

	goto_xy(xloc, yloc);
	atr = atrib;

	*cmdcom = '\0';
	strcpy(cmdcom, "COMMAND.COM");

	args[0] = cmdcom;
	args[1] = "/c";
	args[2] = menu[do_function].command;
	args[3] = NULL;

	r = spawnvp(P_WAIT, cmdcom, args);

	time(&time_out_start);
	xloc = x;
	check_time();

	restore_video(0, 25, 0, 80, save_s);

	xloc = x;
	yloc = y;

	goto_xy(xloc + 1, yloc);

	free(save_s);

	atrib = atr;
}

/*

void create_dorinfo() {

  int r;
   unsigned char *save_s;
   unsigned char *my_env[5];
   unsigned char *args[15], prog[80];



    char    savdcolr, lbuf[2];
   char    cmdcom[60];


   int x,y,atr;


      *cmdcom = '\0';
   strcpy(cmdcom, getenv("COMSPEC"));
   if (!*cmdcom)
      strcpy(cmdcom, "COMMAND.COM");

        args[0] = cmdcom;
   args[1] = "/c";
   args[2] = "ds.bat";
   args[3] = NULL;


   r = spawnv( P_WAIT, cmdcom, args);





}

*/