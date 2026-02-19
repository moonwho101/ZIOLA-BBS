#define __MSC

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

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void speed() {

	unsigned char i, j;
	double msec, msec2, del, del2;

	time_t cstart, cend;

	if (local) {
		delay_time = 0;
		write_factor = (double)1;
		machine_slower = 0;
		return;
	}

	cstart = clock();

	dummy_local = ON;

	local = ON;

	delay_time = (unsigned int)1000;
	/*
	for (i=0;i<15;i++) {
	   for (j=0;j<75;j++) {
	  p(""," ");
	  }
	  p("","\n");
	}

	cend = clock();
	msec = (((double) cend - cstart) / CLK_TCK) * (double) 100;
	msec2=(double) 99.0;

	if (msec2>msec) {
	   write_factor = msec2/msec;
	   machine_slower=0;
	}
	else if (msec2<msec) {
	   write_factor = msec/msec2;
	   machine_slower=1;
	}
	else if (msec2==msec) {
	   machine_slower=0;
	   write_factor=(double)1;

	}
	  */
	/*
	 printf( "\nclock:\t\t\t%4.2f seconds",((float)cend - cstart) / CLK_TCK );
	 printf( "\nslow: %d   write: %f   msec: %f   msec2: %f",machine_slower,write_factor,msec,msec2);
	  */
	machine_slower = 0;
	write_factor = (double)1;

	dummy_local = OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void init_modem() {

	unsigned int i, flag = ON;
	unsigned char c, d;

	open_port(1, 4096);

	/*
	   set_port(38400L,8,NO_PARITY,1);



	   fifo(0);


	   set_tx_rts(TRUE);
	   set_tx_xon(FALSE);
	   set_rx_rts(TRUE);
	   set_rx_xon(FALSE);


	   delay(1000);
	   set_dtr(OFF);
	   delay(1000);

	   set_dtr(ON);



	   goto_xy(1,1);
	   delay(1000);

	   purge_in();

	   while (flag) {
	      printf("\nSending ATZ\n");
	      put_str_serial("ATZ\r\n");
	      delay(1000);
	      flag=ok();
	   }

	   flag=ON;

	   while (flag) {
	      printf("\n\nSending ATS0=1\n");


	       put_str_serial("AT S0=1\r\n");
	      delay(1000);

	      flag=ok();
	   }

	  */
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char ok() {

	unsigned char str[80], k[80], flag = ON, c, counter = 0;
	time_t tstart, tend;

	time(&tstart);
	time(&tend);

	while (flag) {
		if (difftime(tend, tstart) >= (double)5) {
			printf("\nTime out");
			return 1;
		}

		time(&tend);

		if (in_ready()) {
			c = (unsigned char)get_serial();
			/*      printf("%c",c);*/
		} else
			c = NULL;

		if (c != NULL) {
			str[counter++] = c;
		}

		if (strstr(str, "OK") != NULL) {
			printf("\nModem responded with OK");
			return 0;
		} else if (strstr(str, "ERROR") != NULL) {
			printf("\nModem responded with ERROR");
			return 1;
		}
	}
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void wait_caller() {

	unsigned int i, flag = ON;
	unsigned char c, d, counter = 0;
	unsigned char connect[100];

	printf("\n\nWaiting for caller\n");

	/*
	   i=inp(base+2);

	   if (i&256 || i&128)
	     printf("\nUART 16550 detected\n\n");
	   else
	     printf("\nUART 16450 detected\n\n");

	  */
	while (flag) {

		if (in_ready()) {
			c = (unsigned char)get_serial();
			printf("%c", c);

		}

		else
			c = NULL;

		if (c != NULL)
			connect[counter++] = c;

		if (strstr(connect, "CONNECT") != NULL) {

			flag = OFF;
			delay(2000);
			get_str_serial();

			strcpy(connect, response);
			local = OFF;

		} else if (_bios_keybrd(_KEYBRD_READY) > 0) {
			d = (unsigned char)_bios_keybrd(_KEYBRD_READ);
			if (d == 'l') {
				flag = OFF;
				local = ON;
				put_str_serial("ATM0H1\r\n");
				strcpy(connect, "Local");

				/* gets(connect);*/
			} else if (d == 27) {
				put_str_serial("ATM0H1\r\n");
				fifo(0);
				close_port();

				exit(1);
			}
		}
	}

	erc = 0;

	if (strstr(connect, "1200") != NULL) {
		strcpy(connect_at, "1200");
		bps = 1;
		delay_time = 12000;
	} else if (strstr(connect, "2400") != NULL) {
		strcpy(connect_at, "2400");
		bps = 2;
		delay_time = 6000;
	} else if (strstr(connect, "4800") != NULL) {
		strcpy(connect_at, "4800");
		bps = 3;
		delay_time = 3000;
	}

	else if (strstr(connect, "7200") != NULL) {
		strcpy(connect_at, "7200");
		bps = 4;
		delay_time = 2000;
	}

	else if (strstr(connect, "9600") != NULL) {
		strcpy(connect_at, "9600");
		bps = 5;
		delay_time = 0;
	} else if (strstr(connect, "14400") != NULL) {
		strcpy(connect_at, "14400");
		bps = 6;
		delay_time = 0;
	} else if (strstr(connect, "16800") != NULL) {
		strcpy(connect_at, "16800");
		bps = 7;
		delay_time = 0;
	} else if (strstr(connect, "Local") != NULL) {
		strcpy(connect_at, "Local");
		bps = 2;
		delay_time = 0;
	} else {
		strcpy(connect_at, "300");
		bps = 0;
		delay_time = 24000;
	}

	/*   printf( "\ndelay_time: %d   write_factor: %f   ",delay_time,write_factor);*/

	if (strstr(connect, "ARQ") != NULL || strstr(connect, "LAP") != NULL) {

		erc = 1;
		delay_time = delay_time / 2;
	} else {
		erc = 0;
	}
	if (machine_slower)

		delay_time = delay_time / write_factor;
	else if (!machine_slower)
		delay_time = delay_time * write_factor;

	/*   printf( "\ndelay_time: %d   write_factor: %f   ",delay_time,write_factor);*/

	send_clear_screen();
	if (!local)
		detect_ansi_graphics();

	p("[0;1;33m", "\nConnected At: ");
	p("[36m", connect);

	strcpy(connect_save, connect);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void detect_ansi_graphics() {

	unsigned char c;

	put_str_serial("[6n");
	delay(2000);

	if (in_ready())
		c = (unsigned char)get_serial();
	else
		c = NULL;

	purge_in();

	if (c == '') {
		user.ansi[0] = '3';
		p("[0;1;40;33m", "");
		p("[2J", "");
		p("", "A");
		p("[35m", "N");
		p("[31m", "S");
		p("[36m", "I");
		p("[32m", " graphics have been detected.");
	} else {
		user.ansi[0] = '1';
		p("", "Did not detect ANSI graphics.");
	}

	p("", "\n");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void put_str_serial(unsigned char *s) {

	while (*s)
		put_serial(*s++);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_str_serial() {

	unsigned char counter = 0;

	while (in_ready() && counter < 80) {
		response[counter++] = (unsigned char)get_serial();
	}

	response[counter] = '\0';
}
