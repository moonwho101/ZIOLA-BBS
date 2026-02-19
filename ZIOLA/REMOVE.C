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
char save_it (unsigned char search,char loc);


   unsigned char answer[80],check=OFF,flag=ON;
   long int number,user_rec_num,which;
   int c;


   struct file_idx file_tree ;
   struct file_bbs file_tmp  ;

   FILE *fp,*id;


   FILE *din,*bin;

   FILE *dout,*bout;

   long int counter=0,h,ass,spaz,de,final;

   long int calc1,calc2,calc3,calc4,add_it=0;

   unsigned char dt[11];
   unsigned char tm[9];
   unsigned char str[FSIZE];


main() {




   long int number,i;


   size_t rec_size = sizeof (struct file_idx);
   size_t rec_size2 = sizeof (struct file_bbs);










   unsigned char work[81];



   cls(0,0,24,79);
   goto_xy(1,1);

      if ((bout=fopen("file2.bbs","wb+")) == NULL) {
	 printf("cannot open file.idx\n");
	 exit(1);
      }


      if ((fp=fopen("file2.idx","wb+")) == NULL) {
	 printf("cannot open file.idx\n");
	 exit(1);
      }

      fclose(fp);


      if ((dout=fopen("file2.des","wb+")) == NULL) {
	 printf("cannot open file.idx\n");
	 exit(1);
      }


  if ((din=fopen("file.des","rb+")) == NULL) {
	 printf("cannot open file.idx\n");
	 exit(1);
      }

  if ((bin=fopen("file.bbs","rb+")) == NULL) {
	 printf("cannot open file.idx\n");
	 exit(1);
      }


/*
      strcpy(g_file_idx.name,"                                ");
      g_file_idx.left=0;
      g_file_idx.right=0;


      fseek(fp,0L*rec_size,0);
      fwrite (&g_file_idx, rec_size,1,fp);
      fclose(fp);

  */


   if ((fp=fopen("file.idx","rb+")) == NULL) {
      printf("Cannot open file.idx\n");
      exit(1);
   }

   mark_file=0;

   fseek(fp,0L*rec_size,0);
   fread(&file_tree,rec_size,1,fp);


   number= file_tree.left;




   for(i=1;i<=number;i++)  {





       fseek(fp,i*rec_size,0);
       fread(&file_tree,rec_size,1,fp);



       fseek(bin,i*rec_size2,0);
       fread(&file_tmp,rec_size2,1,bin);


       strcpy(file_temp,file_tree.name);
       strcpy(dt,file_tree.date);
       strcpy(tm,file_tree.time);



       if (file_tree.delete == 'N') {


       add_it++;
       save_it(OFF,OFF);

       /*
       printf("\n\n%s ",file_temp);

       printf("start: %d",file_tmp.start);
       printf(" one_line: %d",file_tmp.one_line_start);
       printf(" finish: %d\n",file_tmp.finish);
         */


/*
       if (file_tmp.detailed[0]=='N')

		  file_tmp.start = file_tmp.one_line_start;*/


       fseek(bin,(i+1)*rec_size2,0);
       fread(&file_tmp,rec_size2,1,bin);

       ass=file_tmp.start;

       fseek(bin,0L,0);
       fread(&file_tmp,rec_size2,1,bin);

       final = file_tmp.start;

       fseek(bin,(i)*rec_size2,0);
       fread(&file_tmp,rec_size2,1,bin);

       spaz =file_tmp.start;

       if (final==file_tmp.finish)
	     ass=final;


       calc1=counter;

       for (h=spaz;h<ass;h++) {




	 fseek(din,h*80,0);



	 fread(str,80,1,din);



/*	 printf("%s",str);*/




	 fwrite (str, 80,1,dout);




	 counter++;

       }

	 calc2 = file_tmp.one_line_start - file_tmp.start;
	 calc3 = file_tmp.finish - file_tmp.start;


	 file_tmp.start = calc1;
	 file_tmp.finish = calc1+calc3;
	 file_tmp.one_line_start = calc1+calc2;

	 fseek(bout,(add_it)*rec_size2,0);
	 fwrite(&file_tmp,rec_size2,1,bout);




       }

       else {
	 printf("deleting %s\n",file_tree.name);
	 unlink(file_tree.name);
       }
   }



   file_tmp.start = counter;
   fseek(bout,0L,0);
   fwrite(&file_tmp,rec_size2,1,bout);

   file_tree.left = add_it;

   fseek(fp,0L*rec_size,0);
   fwrite(&file_tree,rec_size,1,fp);

   fclose(fp);

   fclose(bin);
   fclose(din);
   fclose(bout);
   fclose(dout);

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
  printf("  Name: %s ",file_tree.name);
  printf("  Del: %c ",file_tree.delete);





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



char save_it (unsigned char search,char loc) {

   char result;
   char side;
   char flag=1;

   long int count=1;
   long int record;

   size_t rec_size = sizeof (struct file_idx);
   size_t rec_size2 = sizeof (struct file_bbs);
   long int number;

   FILE *fp;




   if ((fp=fopen("file2.idx","rb+")) == NULL) {
      printf("Cannot open file.idx11\n");
      exit(1);
   }

   fseek(fp,0L,0);
   fread(&g_file_idx,rec_size,1,fp);
   number=g_file_idx.left;

   if (number==0) {
      if (search) {
	 fclose(fp);
	 return 0;
      }
      number++;
      strcpy(g_file_idx.name,file_temp);
      g_file_idx.left=0;
      g_file_idx.right=0;
      g_file_idx.delete='N';
      strcpy(g_file_idx.date,dt);
      strcpy(g_file_idx.time,tm);


      record=1;
      fseek(fp,record*rec_size,0);
      fwrite (&g_file_idx, rec_size,1,fp);
      strcpy(g_file_idx.name,"");
      g_file_idx.left=number;
      g_file_idx.right=0;
      fseek(fp,0L,0);
      fwrite (&g_file_idx, rec_size,1,fp);
   }
   else {
      record=1;
      while (flag) {
	 fseek(fp,record*rec_size,0);
	 fread(&g_file_idx,rec_size,1,fp);
	 count=record;
	 result=(char) strcmp(file_temp,g_file_idx.name);
	 if (result > 0) {
	    record=g_file_idx.right;

	    side=1;
	 }
	 else if (result <0) {
	    record=g_file_idx.left;
	    side=0;

	 }
	 else {
	    fclose(fp);

/*
	     if (g_file_idx.delete=='N')
	      return 1;
	     else
	      return 0;
*/
	  if (search==2) {


	      return 1;


	  }


	    if ((fp=fopen("file2.bbs","rb+")) == NULL) {
	       printf("cannot open files.bbs\n");
	       exit(1);
	    }


	    file_rec_num=count;

	    fseek(fp,file_rec_num*rec_size2,0);
	    fread(&g_file_bbs,rec_size2,1,fp);

	    if (user.ansi[0]=='1')
	       atrib=7;

	    fclose(fp);

	    return 1;
	 }
	 if (record==0)
	    flag=0;
      }
      if (search) {
	 fclose(fp);
	 return 0;
      }
      number++;
      if (side==1) {
	 g_file_idx.right=number;
	 record=count;
	 fseek(fp,record*rec_size,0);
	 fwrite (&g_file_idx, rec_size,1,fp);
      }
      else {
	 g_file_idx.left=number;
	 record=count;
	 fseek(fp,record*rec_size,0);
	 fwrite (&g_file_idx, rec_size,1,fp);
      }
      strcpy(g_file_idx.name,file_temp);
      g_file_idx.left=0;
      g_file_idx.right=0;
      g_file_idx.delete='N';
      strcpy(g_file_idx.date,dt);
      strcpy(g_file_idx.time,tm);
      record=number;
      fseek(fp,record*rec_size,0);
      fwrite (&g_file_idx, rec_size,1,fp);
      strcpy(g_file_idx.name,"");
      g_file_idx.left=number;
      g_file_idx.right=0;
      fseek(fp,0L,0);
      fwrite (&g_file_idx, rec_size,1,fp);
   }
   fclose(fp);



   record=number;
   file_rec_num=record;




   fseek(bout,record*rec_size2,0);
   fwrite (&file_tmp, rec_size2,1,bout);




   /*
   if (loc)
      unlock("user.loc");
   */

}

/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/

