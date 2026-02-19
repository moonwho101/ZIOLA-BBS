#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "bios.h"
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <process.h>
#include <direct.h>
#include <math.h>

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


#define OFF 0
#define ON 1


int inkey();
void display();
void cls(unsigned char startx,unsigned char starty,unsigned char endx,unsigned char endy);
void goto_xy(int x,int y);


   unsigned char answer[80],check=OFF,flag=ON;
   long int number,user_rec_num,which;
   int c;


main() {



   size_t rec_size = sizeof (struct file_idx);
   size_t rec_size2 = sizeof (struct file_bbs);

   FILE *fp,*id;

   if ((fp=fopen("file.idx","rb+")) == NULL) {
      printf("Cannot open users.idx\n");
      exit(1);
   }


   if ((id=fopen("file.bbs","rb+")) == NULL) {
      printf("Cannot open users.idx\n");
      exit(1);
   }

   fseek(fp,0L,0);
   fread(&g_file_idx,rec_size,1,fp);
   number=g_file_idx.left;

   which=1L;


   cls(0,0,24,79);
   goto_xy(1,1);


      while  (flag) {

   fseek(fp,which*rec_size,0);
   fread(&g_file_idx,rec_size,1,fp);


   fseek(id,which*rec_size2,0);
   fread(&g_file_bbs,rec_size2,1,id);

     display();


    c=0;
    while (c==0)
    c=(unsigned char) inkey();

    if (c=='q') {
     fclose(fp);
      fclose(id);
     exit(1);
    }
    else if(c=='+')
       which++;
    else if(c=='-')
       which--;
    else if(c=='d') {

if(    g_file_idx.delete=='N')
       g_file_idx.delete='Y';
else
       g_file_idx.delete='N';

      fseek(fp,which*rec_size,0);
   fwrite(&g_file_idx,rec_size,1,fp);


    }


   if(which<1)
     which=number;
   if (which>number)
      which=1L;


   }





  fclose(fp);
  fclose(id);





}

int inkey()  {

   union scan {
      int c;
      char ch[2];
   } sc;


   if (_bios_keybrd(_KEYBRD_READY) >0 ) {

      sc.c = _bios_keybrd(_KEYBRD_READ);


      if (sc.ch[0]==0)    {
	 return sc.ch[1];
      }
      else
	 return sc.ch[0];
   }

   return 0;
}


void display() {



  printf("\nNum: %d",which);
  printf("  Name: %s ",g_file_idx.name);
  printf("  Del: %c ",g_file_idx.delete);





}


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


void goto_xy(int x,int y) {

  union REGS r;
  r.h.ah = 2;
  r.h.dl = (unsigned char) y;
  r.h.dh = (unsigned char) x;
  r.h.bh = 0;

  int86(0x10,&r,&r);
}

