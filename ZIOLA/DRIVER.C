#include <stdio.h>
#include <dos.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
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
#include "main.h"
#include "modem.h"

#define PARAM_LENGTH 20
#define PARAM_VALUE 20

#define ON 1
#define OFF 0



/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void ansi_param( char setting[], char type) {

  unsigned int param[PARAM_VALUE];
  char cut[5];
  register unsigned char counter=0;
  unsigned char length;
  unsigned char pass=0;


  if (!save_atrib)
  strcpy(save_ansi_param,setting);



  for (length=0;length<PARAM_VALUE;length++)
     param[length]=0;

  while( counter < (unsigned char) strlen(setting) ) {
     length=0;
     while (setting[counter] !=';' && setting[counter] != type) {
	cut[length]=setting[counter];
	length++;
	counter++;
     }
     counter++;
     cut[length] = '\0';
     param[pass++] = (unsigned int) atoi(cut);
  }

  scrn_ctrl(param,pass,type);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void scrn_ctrl(unsigned int test[],unsigned char sz,char type) {

  static int fore,back,attrib;
  register unsigned char counter=0;
  register unsigned char i=0;

  if (type !='m' && test[0] == 0)
     test[0]=1;

   switch (type) {

      case 'm':
	 for (counter=0;counter<sz;counter++) {

	    switch (test[counter]) {
	       case 0:
		  attrib = 0;
		  fore = 7;
		  back = 0;
		  break;

	       case 1:

		  attrib |=8;
		  break;

	       case 5:
		  attrib |=128;
		  break;

	       case 7:
		  fore = 0;
		  back = 112;
		  break;

	       case 8:
		  fore = 0;
		  back = 0;
		  attrib =0;
		  break;

	       case 30:
		  fore=0;
		  break;

	       case 31:
		  fore = 4;
		  break;

	       case 32:
		  fore = 2;
		  break;

	       case 33:
		  fore = 6;
		  break;

	       case 34:
		  fore = 1;
		  break;

	       case 35:
		  fore = 5;
		  break;

	       case 36:
		  fore=3;
		  break;

	       case 37:
		  fore = 7;
		  break;

	       case 40:
		  back = 0;
		  break;

	       case 41:
		  back = 64;
		  break;

	       case 42:
		  back = 32;
		  break;

	       case 43:
		  back = 96;
		  break;

	       case 44:
		  back = 16;
		  break;

	       case 45:
		  back = 80;
		  break;

	       case 46:
		  back = 48;
		  break;

	       case 47:
		  back = 112;
		  break;
	    }
       }
      break;

      case 'H':
      case 'F':
      case 'f':
	 if (test[1]==0)
	    test[1]=1;

	 xloc=test[0]-1;
	 yloc=test[1];
	 yloc=yloc-1;
	 break;

      case 'A':

	 xloc=xloc-test[0];
	 if (xloc<0) xloc=0;
	 break;

      case 'B':

	 xloc=xloc+test[0];
	 if (xloc>24) xloc=24;
	 break;

      case 'C':

	 yloc=yloc+test[0];
	 if (yloc>79) yloc=79;
	 break;

      case 'D':

	 yloc=yloc-test[0];
	 if (yloc<0) yloc=0;
	 break;

      case 's':

	 savex=xloc;
	 savey=yloc;
	 break;

      case 'u':
	 xloc=savex;
	 yloc=savey;
	 break;

      case 'J' :



	 cls(upx+1,upy,lowx+1,lowy);
	 xloc=0;
	 yloc=0;

	 break;

      case 'K' :

	for(i=(unsigned char) yloc;i<=79;i++) {
	if (xloc >= upx && xloc <= lowx && yloc >= upy && yloc <= lowy)
	    write_char(xloc+1,i,' ',7,1);
	yloc++;
	screen[index[xloc]].chrtr[i] = 32;
	screen[index[xloc]].atrib[i] = 7;
	}



   }
   goto_xy(xloc+1,yloc);


   atrib=fore+back+attrib;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void goto_xy(int x,int y) {

  union REGS r;
  r.h.ah = 2;
  r.h.dl = (unsigned char) y;
  r.h.dh = (unsigned char) x;
  r.h.bh = 0;

  int86(0x10,&r,&r);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void write_char(int x,int y,char ch,int attrib,char save) {

  union REGS r;
  char far *v;



   if ( direct_write == 1) {
     v = vid_mem;
     v += (x*160) + y*2;
     *v++ = ch;
     *v=(char) attrib;
  }
  else {
     goto_xy(x,y);
     r.h.ah=9;
     r.h.bh=0;
     r.x.cx=1;
     r.h.al=ch;
     r.h.bl= (char) attrib;
     int86(0x10,&r,&r);
  }
  if (save) {
	     }


}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

video_mode() {

  union REGS r;
  r.h.ah = 15;
  return int86(0x10,&r,&r) & 255;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_video() {

  int vmode;

  vmode = video_mode();
  /*if ((vmode != 2) && (vmode !=3) && (vmode !=7)) {
     printf("Must be in 80 column mode.");
     exit(1);
  } */

  if (vmode==7)
     vid_mem = (char far *) 0xB0000000;
  else
     vid_mem = (char far *) 0xB8000000;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void cls(startx,starty,endx,endy)

unsigned char startx,starty,endx,endy;

{

  union REGS r;

  r.h.ah= 6;
  r.h.al=0;
  r.h.ch =startx;
  r.h.cl=starty;
  r.h.dh=endx;
  r.h.dl = endy;
  r.h.bh = 7;

  int86(0x10,&r,&r);

}


/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void scroll_window(startx,starty,endx,endy,lines)

unsigned char startx,starty;
unsigned char endx,endy;
unsigned char lines;

{

  union REGS r;
  r.h.ah=6;

  r.h.al = lines;
  r.h.ch = starty;
  r.h.cl = startx;
  r.h.dh = endy;
  r.h.dl = endx;
  r.h.bh = 0;
  int86(0x10,&r,&r);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void save_scrn() {

   register unsigned char i,j,tmp;

   if (xloc >= bottom ) {
      scroll_window(upy,upx+1,lowy,lowx+1,1);
      if (file_scroll) {
	 for (tmp=upy;tmp<=lowy;tmp++) {
	    write_char(lowx+1,tmp,screen[index[lowx+1]].chrtr[tmp],
	    screen[index[lowx+1]].atrib[tmp],0);
	 }
      }
      else {
	 xloc=bottom-1;
	 for (i=0;i<79;i++)
	    write_char(xloc+1,i,32,7,0);
	goto_xy(xloc+1,yloc);
      }

      yloc=0;
      xloc=bottom-1;
      tmp= (unsigned char) index[0];
      for (i=0;i<bottom;i++)
	 index[i]=index[i+1];
      index[bottom]=tmp;

      for (j=0;j<80;j++) {
	 screen[index[bottom]].atrib[j]=7;
	 screen[index[bottom]].chrtr[j]=32;
      }
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


void p(char *ansi,char *str) {

   static unsigned int ansi_c;
   static unsigned char ansi_i[20];

   unsigned char sz,c;
   register unsigned char i,j,counter=0;
   char build[PARAM_LENGTH];
   unsigned int u=0;

   sz= (unsigned char) strlen (ansi);

   if ( user.ansi[0] !='1') {
      for (i=2;i<sz;i++) {
	 if (ansi[i] !=27 && i<sz) {
	    build[counter]=ansi[i];
	    counter++;
	 }
	 if (ansi[i]==27 || i==sz-1)  {
	    build[counter]='\0';
	    if (ansi[i] == 27)
	       i++;
	    ansi_param( build, build[counter-1]);
	    counter=0;
	 }
      }

	 for (i=0;i<sz;i++) {

	    if (local==0)
	    put_serial(ansi[i]);
	    delay_screen(delay_time);

	 }

   }

   for (j=0;j< (unsigned char) strlen(str);j++) {
      if (hot_of) {
	 c=0;
	 c=inkey();
	 if (c != NULL) {

	    c=upper(c);
	    send[0]=c;
	    send[1]='\0';

	    if (strstr(options,send) !=NULL) {
	       abort_file=ON;
	       grab_ansi=OFF;
	       key_pressed=c;
	       hot_of = 2;
	       return;
	    }
	 }
      }

      if (str[j] == '~') {
	 abort_file=2;
	 return;
      }
      else if (str[j] == 9) {
	 if (yloc<8)
	    yloc=9;
	 else if (yloc<16)
	    yloc=16;
	 else if (yloc<24)
	    yloc=24;
	 else if (yloc<32)
	    yloc=32;
	 else if (yloc<40)
	    yloc=40;
	 else if (yloc<48)
	    yloc=48;
	 else if (yloc<56)
	    yloc=56;
	 else if (yloc<64)
	    yloc=64;
	 else if (yloc<72)
	    yloc=72;
      }
      else if (str[j] == '\n') {
	 if (local==0) {
	    put_serial((unsigned char) 13);
	    put_serial((unsigned char) 10);
	 }

	 if (!file_scroll)
	    screen[index[xloc]].chrtr[yloc] = '\n';

	 xloc++;
	 yloc=0;
	 goto_xy(xloc+1,yloc);
	 save_scrn();

	 if (more_of==ON && more_r) {
	    more++;


	      more_prompt();

	    if (more_r==OFF) {
	       grab_ansi=OFF;
	       return;
	    }
	 }
      }
      else if (str[j] == '\r') ;
      else {
	 save_scrn();

	 delay_screen(delay_time);

	 if (str[j] == 27) {
	    grab_ansi=ON;
	    ansi_c=0;
	 }
	 else if (grab_ansi==ON) {
	    if (str[j] != '[')
	       ansi_i[ansi_c++]=str[j];
	    if (strchr("HFfABCDnsuJKm",str[j]) != NULL) {
	       ansi_i[ansi_c]='\0';
	       if (user.ansi[0] !='1') {
		  if (!local){
		     put_str_serial("[");
		     put_str_serial(ansi_i);
		  }
		  ansi_param(ansi_i,str[j]);
	       }
	       grab_ansi=OFF;
	    }
	 }
	 else {
	    if (xloc >= upx && xloc <= lowx && yloc >= upy && yloc <= lowy)
	       write_char(xloc+1,yloc,str[j],atrib,1);

	    screen[index[xloc]].chrtr[yloc] = str[j];
	    screen[index[xloc]].atrib[yloc] = atrib;
	    yloc++;
	    goto_xy(xloc+1,yloc);
	    if (yloc > 79) {
	       xloc++;
	       yloc=0;
	       goto_xy(xloc+1,yloc);
	       save_scrn();
	    }

	    grab_ansi=OFF;

	    if (!local)
	       put_serial(str[j]);
	 }
      }
      if (file_scroll) {
	 if (xloc>under_bar)
	     under_bar=xloc;
      }
   }
}


void system_log_break() {


  strcpy(response,"\n");
  strcat(response,line);
  response[78]='\n';
  response[79]='\0';

  write_system_log();

}


void write_system_log() {


      fputs(response,system_log);


}