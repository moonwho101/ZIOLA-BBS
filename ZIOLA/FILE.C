#define __MSC

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

char save_file (unsigned char search,char loc) {

   char result;
   char side;
   char flag=1;

   long int count=1;
   long int record;

   size_t rec_size = sizeof (struct file_idx);
   size_t rec_size2 = sizeof (struct file_bbs);
   long int number;

   FILE *fp;

   /*
   if (loc) {
      while (lock("user.loc")==0) {
	 printf("file is locked\n");
      }
   }
   */

   file_name_files("idx",1L);

   if ((fp=fopen(reserve,"rb+")) == NULL) {
      printf("Cannot open file.idx\n");
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

	    file_name_files("bbs",1L);
	    if ((fp=fopen(reserve,"rb+")) == NULL) {
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
      strcpy(g_file_idx.date,s_date);
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

   file_name_files("bbs",1L);
   if ((fp=fopen(reserve,"rb+")) == NULL) {
      printf("cannot open files.bbs\n");
      exit(1);
   }

   record=number;
   file_rec_num=record;

   fseek(fp,record*rec_size2,0);
   fwrite (&g_file_bbs, rec_size2,1,fp);
   fclose(fp);


   fclose(fp);
   /*
   if (loc)
      unlock("user.loc");
   */

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


void file_name_files(unsigned char ext[],long int file_area) {

   unsigned char work[10];


   strcpy(reserve,file_path);

   if (list_batch)
      strcat(reserve,true_area);
   else
   strcat(reserve,menu[do_function].command);


   strcat(reserve,"\\");
   strcat(reserve,"file");

   strcat(reserve,".");
   strcat(reserve,ext);

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void create_file_areas() {

   FILE *fp;

   long int record,areas;

   size_t rec_size = sizeof (struct file_idx);
   size_t gfilebbs = sizeof (struct file_bbs);

   for (areas=1;areas<5;areas++) {


   do_function=0;

   itoa(areas,response,10);
   strcpy(menu[do_function].command,response);

   file_name_files("idx",1L);

    if ((fp=fopen(reserve,"rb")) == NULL) {
      fclose(fp);



      file_name_files("idx",1L);
      if ((fp=fopen(reserve,"wb+")) == NULL) {
	 printf("cannot open file.idx\n");
	 exit(1);
      }
      strcpy(g_file_idx.name,"                                ");
      g_file_idx.left=0;
      g_file_idx.right=0;
      record=0;

      fseek(fp,record*rec_size,0);
      fwrite (&g_file_idx, rec_size,1,fp);

   }
   fclose(fp);
   file_name_files("bbs",1L);

   if ((fp=fopen(reserve,"rb")) == NULL) {
      fclose(fp);
      file_name_files("bbs",1L);

      if ((fp=fopen(reserve,"wb+")) == NULL) {
	 printf("cannot open file.bbs\n");
	 exit(1);
      }


      g_file_bbs.start=0;
      fseek(fp,0L*gfilebbs,0);
      fwrite (&g_file_bbs, gfilebbs,1,fp);


   }
   fclose(fp);
   file_name_files("des",1L);

   if ((fp=fopen(reserve,"rb")) == NULL) {
      fclose(fp);
      file_name_files("des",1L);
      printf("\n\n%s",reserve);
      if ((fp=fopen(reserve,"wb+")) == NULL) {
	 printf("cannot open file.bbs\n");
	 exit(1);
      }

   }
   }
   fclose(fp);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_file_name() {

  unsigned char result;

  get_string(12,0);

  strcpy(file_temp,strupr(response));
  result = save_file (ON,OFF);

  if (!result) {
      printf("saving");
      save_file (OFF,OFF);

  }
  display_file(file_temp);

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_file(unsigned char fname[]) {

   struct find_t find;
   long size;

   strcpy(response,file_path);
   strcat(response,fname);

   if ( !_dos_findfirst( response, _A_NORMAL, &find ) ) {
      printf("found");
      printf("\n%d %d %d",(find.wr_date >>9) + 1980,(find.wr_date & 0x1E0)>> 5, (find.wr_date & 0x1F));
      printf("\n%d %d %d    %d",(find.wr_date >>9) + 1980,(find.wr_date & 0x1E0)>> 5, (find.wr_date & 0x1F));
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void batch() {

   unsigned char counter=0;
   unsigned char cnt=0;
   unsigned char flag=ON,flag2=OFF;

   strcpy(response,"t");

   mark_file=0;
   tag=OFF;

   send_clear_screen();

   display_f_area(0);
   p("[0;1;41;33m","ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ");
   p("[40m","\n");
   p("[41m","ณ");
   p("[44;33m","  Downloading Files  ");
   p("[41;33m","ณ");
   p("[40m","\n");
   p("[41;33m","ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");
   p("[40m","\n");


   if (items_in_queue) {
      p("[32m","\nThere ");
      if (items_in_queue==1)
	 p("","is ");
      else
	 p("","are ");

      ltoa(items_in_queue,response,10);
      p("[33m",response);

      if (items_in_queue==1)
	 p("[32m"," file");
      else
	 p("[32m"," files");

      p("[32m"," in your queue waiting to be downloaded.\n");
   }

   p("[36m","\nYou may enter");
   p("[31m"," WILDCARDS ");
   p("[35m","[");
   p("[33m","*");
   p("[37m",",");
   p("[33m","?");
   p("[35m","] ");
   p("[36m","when entering ");
   p("[32m","FILENAMES");
   p("[36m",".\n");


   while (flag) {
      p("[40;36m","\nEnter filename ");

      p("[35m","[");


      if (items_in_queue) {
	 p("[33m","RETURN");
	 p("[32m"," for Queue Menu");
      }

      else {
	 p("[33m","RETURN");
	 p("[32m"," Aborts");
      }

      p("[35m","]");
      p("[36m",": ");


      if (user.ansi[0] !='1') {
	 p("[37;44m","            ");
	 p("[12D","");
      }

      get_files(12);

      if (strlen(response) > 0) {
	 p("","\n");
	 find_files();
      }
      else{
	 p("","\n");
	 flag2=ON;
	 flag=OFF;
      }
   }

   if (items_in_queue) {
      srt();
      if (!add_to_queue)
	 edit_queue();
   }

}


/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_files(unsigned char length) {

   int counter=0;
   unsigned char c=ON;
   char up=OFF;
   unsigned char pos=1;

   clear_input();

   while (c!=13) {
      c=0;

      while (c==0)
	 c=(unsigned char) inkey();

      if (!is_valid(c)) {
      }
      else if (c==8 && counter >0) {
	 counter--;

	 if (response[counter]=='.')
	    up=OFF;

	 if (up)
	   pos--;

	 delete(1);
	 response[counter]=32;
      }
      else if (c==',' || c=='\\' ||  c==' ' ||  c==':');
      else if (c=='.' && up==OFF && counter>0){
	 up=ON;
	 p("",".");
	 response[counter++]='.';
	 pos=0;
      }
      else if (c=='.');
      else if (counter<length && c !=8 && c !=13 && pos<3) {
	 response[counter++]=c;

	 if (up)
	 pos++;
	 c=upper(c);
	 send[0]=c;
	 p("",send);

       }
   }

   response[counter]='\0';
   strip_blanks();
   cut_string();
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void srt() {

   int a,b;
   unsigned char t;

   for (a=1;a<items_in_queue;++a) {
      for (b=items_in_queue-1;b>=a;--b) {
	 if ( strcmp(file_q[file_index[b-1]].name , file_q[file_index[b]].name) > OFF) {
	    t=file_index[b-1];
	    file_index[b-1]=file_index[b];
	    file_index[b]=t;
	 }
      }
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void edit_queue() {

   unsigned char c,flag=ON,counter,flag2=ON,flag3,tmp;

   FILE *fp,*id;
   struct file_idx file_tree ;
   size_t rec_size = sizeof (struct file_idx);

   long int answer,answer2,cnt,calc,temp,i,sum,temp2;

   while (flag) {
      p("[0;1;36;40m","\nDownload Queue ");
      p("[35m","[");
      p("[33m","L");
      p("[35m","]");
      p("[32m","ist  ");

      p("[35m","[");
      p("[33m","A");
      p("[35m","]");
      p("[32m","dd  ");


      p("[35m","[");
      p("[33m","R");
      p("[35m","]");
      p("[32m","emove  ");

      p("[35m","[");
      p("[33m","E");
      p("[35m","]");
      p("[32m","rase  ");


      p("[35m","[");
      p("[33m","D");
      p("[35m","]");
      p("[32m","ownload  ");


      p("[35m","[");
      p("[33m","Q");
      p("[35m","]");
      p("[32m","uit");
      p("[31m",": ");


      if (user.ansi[0] !='1') {
	 p("[37;44m"," ");
	 p("[1D","");
      }


      strcpy(options,"LAREDQ");
      c=hot_key();

      switch (c) {


	 case 'R':

	     flag2=ON;
	     flag3=OFF;

	     if (items_in_queue==0) {
		queue_is_empty();
		break;
	     }

	     while (flag2) {
		p("[33;40m","\n\n");
		p("","Enter starting file TAG number:" );
		p("[31m"," [");
		p("[36m","1-");

		ltoa(items_in_queue,response,10);

		p("",response);
		p("[31m","]");
		p("[33m",": ");

		if (user.ansi[0] !='1') {
		   p("[37;44m","     ");
		   p("[5D","");
		}

		get_number(5,"");

		if (strlen(response)==0) {
		   flag3=ON;
		   p("[40;31m","\n\nRemove tag ");
		   p("[33m","has been aborted");
		   p("[37m",".\n");
		   break;
		}

		answer=atol(response);

		if (answer<1 || answer>items_in_queue) {
		   p("[40;33m","\n\nThat is not a valid ");
		   p("[31m","FILE TAG");
		   p("[37m",".");
		}
		else
		   flag2=OFF;
	     }

	     flag2=ON;

	     while (flag2 && !flag3 ) {

		p("[33;40m","\n\n");
		p("","Enter ending file TAG number:" );
		p("[31m"," [");
		ltoa(answer,response,10);
		p("[36m",response);
		p("[36m","-");

		ltoa(items_in_queue,response,10);

		p("",response);
		p("[31m","]");
		p("[33m",": ");

		if (user.ansi[0] !='1') {
		   p("[37;44m","     ");
		   p("[5D","");
		}

		get_number(5,"");

		if (strlen(response)==0) {
		   flag3=ON;
		   p("[40;31m","\n\nRemove tag ");
		   p("[33m","has been aborted");
		   p("[37m",".\n");
		   break;
		}

		answer2=atol(response);

		if (answer2<answer || answer2>items_in_queue) {
		   p("[40;33m","\n\nThat is not a valid ");
		   p("[31m","FILE TAG");
		   p("[37m",".");
		}
		else
		   flag2=OFF;
	     }
	     if (!flag3) {
		delete_from_queue(answer2,answer);

		ltoa((answer2-answer)+1L,response,10);

		p("[0;1;31;40m","\n\n");

		p("",response);
		p("[36m"," line(s) have");
		p("[36m"," been deleted");
		p("[37m",".\n");
	     }

	 break;


	 case 'A':
	    tag=OFF;
	    add_to_queue=ON;
	    batch();
	    add_to_queue=OFF;
	    break;


	 case 'E':

	    if (items_in_queue==0) {
	       queue_is_empty();
	       break;
	    }


	    p("[0;1;40;36m","\n\nAre you sure you wish to erase your ");
	    p("[31m","Download Queue ");
	    p("[35m","[");
	    p("[33m","y");
	    p("[37m","/");
	    p("[33m","N");
	    p("[35m","]");

	    p("[36m",": ");

	    if (user.ansi[0] !='1') {
	       p("[37;44m"," ");
	       p("[1D","");
	    }

	    p("[37m","");


	    response[0]=get_yn("NY");

	    if (response[0]=='Y') {
	       items_in_queue=0;
	       p("[0;1;40;37m","\n\nYour ");
	       p("[31m","Download Queue ");
	       p("[36m","has been erased");
	       p("[37m",".\n");

	       total_t_time=0L;
	       for (tmp=0;tmp<50;tmp++)
		   file_index[tmp]=tmp;
	    }

	    break;

	 case 'Q':
	    flag=OFF;
	    break;

	 case 'L':

	    if (items_in_queue==0) {
	       queue_is_empty();
	       break;
	    }

	    mark_file=0;
	    tag=ON;
	    p("[40m","\n\n");
	    file_header_bar();

	    list_batch=ON;
	    for (counter=0;counter<items_in_queue;counter++) {
	       file_rec_num=file_q[file_index[counter]].record;
	       strcpy(true_area,file_q[file_index[counter]].area);
	       display_file_info(&file_q[file_index[counter]],0);
	    }
	    list_batch=OFF;
	    break;


	 case 'D':

	    if (items_in_queue==0) {
	       queue_is_empty();
	       break;
	    }

	    if ((id=fopen("file.1","w")) == NULL) {
	       exit(1);
	    }

	    file_name_files("idx",1L);

	    if ((fp=fopen(reserve,"rb+")) == NULL) {
	       printf("Cannot open file.idx\n");
	       exit(1);
	    }

	    for (counter=0;counter<items_in_queue;counter++) {

	       strcpy(response,file_path);
	       strcat(response,file_q[file_index[counter]].area);
	       strcat(response,"\\");
	       strcat(response,file_q[file_index[counter]].name);

	       if (counter==OFF)
		  strcpy(xname,response);

	       strcat(response,"\n");

	       fputs(response,id);
	    }

	    fclose(fp);
	    fclose(id);

	    p("[40m","\n");
	    list_protocols();

	    if (proto !=13) {
	       dsz_upload(OFF);
	       dsz_log(OFF);


	       flag=OFF;
	       p("","\n");
	       press_return();
	    }
	    else {
	       p("[0;1;33;40m","\n\nDownloading a file ");
	       p("[31m","ABORTED");
	       p("[37m",".\n");
	    }

	    break;
      }
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void find_files() {

   int status;

   unsigned char ask=ON;

   unsigned char g_file[13];


   long int remain,areas=0,area;
   struct find_t fileinfo;

   unsigned char work[81],result,first_pass=OFF,found_one=ON;
   unsigned char flag=ON;



   unsigned char save_it[30];

   char check;

   strcpy(save_it,menu[do_function].command);
   strcpy(g_file,response);

   while (flag) {

      if(items_in_queue>=50) {
	 if (!tag_bypass) {
	    p("[0;1;40;32m","\nYour download queue can only hold");
	    p("[31m"," 50 ");
	    p("[32m","files");
	    p("[37m",".\n");
	 }
	 strcpy(menu[do_function].command,save_it);
	 return;
      }

      /*
      else if ( ul_dl_ratio<=system_ul_dl_ratio) {




	 file_stats(1);
	 strcpy(menu[do_function].command,save_it);

	 return;



      }
      */


      complete_file_name(g_file);

      status = _dos_findfirst(reserve,_A_NORMAL,&fileinfo);

      if (!tag_bypass) {
	 if (status !=0){
	    if (ask) {
	       p("[40;33m","\n");
	       p("",response);
	       p("[31m"," could not be found in this area.");
	       p("[32;40m","\n\nSearch other areas ");
	       p("[34m","[");
	       p("[35m","Y/n");
	       p("[34m","]");
	       p("[36m",": ");
	       p("[37;44m"," ");
	       p("[1D","");
	       response[0]=get_yn("YN");
	       p("","\n");

	       ask=OFF;

	       if (response[0]=='N')
                  return;


	    }
	    else
	       response[0]='Y';

	    if (response[0]=='Y') {

	       areas++;

	       if (areas==atol(save_it))
		  areas++;

	       if (areas>number_of_file_areas) {
		  p("[0;1;40;31m","\nNo matches where made in any area...\n");
		  flag=OFF;

	       }
	       else {
		  ltoa(areas,menu[do_function].command,10);
		  p("[0;1;33;40m","\nSearching");
		  p(""," file area ");
		  p("[31m","#");
		  p("[36m",menu[do_function].command);
		  area=atol(menu[do_function].command);
		  p("[37m",", ");
		  p("[32m",file_names[area-1].name);
		  p("","...\n");
	       }
	    }
	 }
	 else {
	    p("","\n");
	    flag=OFF;
	    file_header_bar();
	 }

	 if (strcmp(strupr(response),fileinfo.name)==0 && status !=0) {
	    first_pass=ON;
	 }

      }
      else {
	 flag=OFF;
	 status=0;
      }
   }

   while (status==0) {

      strcpy(file_temp,strupr(fileinfo.name));

      if (!first_pass)
	 result=save_file (ON,OFF);
      else
	 result=save_file (ON,OFF);

      if (result) {
	 strcpy(response,file_temp);
	 check=binary();

	 if (check>=0) {
	    if (!tag_bypass) {
	       p("[0;1;40;33m","      ");
	       strcpy(reserve,response);
	       format(reserve,13);
	       p("",response);
	       p("[36m"," is already in your download queue.\n");
	    }
	 }
	 else {
	    strcpy(file_q[file_index[items_in_queue]].name,fileinfo.name);
	    ltoa(fileinfo.size/1000,response,10);
	    strcpy(file_q[file_index[items_in_queue]].size,response);
	    file_q[file_index[items_in_queue]].true_size=fileinfo.size;
	    strcpy(file_q[file_index[items_in_queue]].des,"Description");
	    file_date(&fileinfo);
	    strcpy(file_q[file_index[items_in_queue]].date,response);
	    file_q[file_index[items_in_queue]].record=file_rec_num;
	    strcpy(file_q[file_index[items_in_queue]].area,menu[do_function].command);

	    calc_t_time(fileinfo.size);

	    total_t_time=total_t_time+true_time;

	    remain=atol(display_time);

	    if (total_t_time>(double) remain) {
	       if (!tag_bypass) {
		  p("[0;1;40;33m","      You don't have enough time to download ");
		  p("[32m",fileinfo.name);
		  p("","\n");
	       }
	       total_t_time=total_t_time-true_time;
	    }
	    else {
	       items_in_queue++;

	       if (!tag_bypass)
		  display_file_info(&file_q[file_index[items_in_queue-1]],0);
	    }

	    if (items_in_queue>=50) {
	       if (!tag_bypass) {
		  p("[0;1;40;32m","\nYour download queue can only hold");
		  p("[31m"," 50 ");
		  p("[32m","files");
		  p("[37m",".\n");
	       }
	       strcpy(menu[do_function].command,save_it);
	       return;
	    }
	 }
      }
      first_pass=OFF;
      status= _dos_findnext(&fileinfo);
   }
   strcpy(menu[do_function].command,save_it);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_file_info(struct files *fdata,unsigned char detailed) {

   unsigned char response_length=79,do_it,a,b,c;
   long int count,items_read,detailed_desc;
   unsigned char f_message[FSIZE],inside,flag=ON,small,first=ON,scratch[10],flag2;

   size_t rec_size2 = sizeof (struct file_bbs);

   FILE *fp;
   FILE *id;


   if (text_search) {

      do_it =   find_text();

      if (!do_it)
	 return;

      if (!st) {
	 found_new_files=ON;
	 send_clear_screen();
	 display_f_area(3);
	 file_header_bar();
	 st=ON;
      }
   }

   p("[0;1;40;32m","");

   if (tag) {
      p("","  ");
      if (more_r!=2) {
	 mark_file++;
	 itoa(mark_file,response,10);
      }
      else
	 response[0]='\0';

      format(response,4);
      p("[33m",response);
   }
   else
      p("","      ");


   format(fdata->name,14);
   p("[36m",response);
   strcpy(tag_files[mark_file].tag,fdata->name);

   strcpy(response,fdata->size);

   format(response,3);
   p("[33m",response);
   p("[35m","K ");

   strcpy(response,fdata->date);

   response[6]=response[8];
   response[7]=response[9];
   response[8]='\0';

   format(response,10);

   p("[32m",response);

   file_name_files("bbs",1L);

   if ((fp=fopen(reserve,"rb+")) == NULL) {
      printf("cannot open files.bbs\n");
      exit(1);
   }

   fseek(fp,file_rec_num*rec_size2,0);
   fread(&g_file_bbs,rec_size2,1,fp);

   fclose(fp);

   ltoa(g_file_bbs.dl_number,scratch,10);

   format(scratch,4);
   p("[31m",response);

   calc_t_time(fdata->true_size);


   format(transfer_time,4);
   p("[36m",response);
   p("[35m","min ");

   file_name_files("des",0L);

   if ((id=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   p("[32m","");


   if (!text_search) {

      if (detailed && g_file_bbs.detailed[0]=='Y') {
	 small=0;

	 for (count=g_file_bbs.start;count<g_file_bbs.one_line_start;count++) {
	    fseek(id,count*sizeof(f_message),0);
	    fread(f_message,sizeof(f_message),1,id);
	    inside=0;
	    flag=ON;
	    flag2=OFF;

	    while (flag) {

	       if (f_message[inside]=='\n' || small>=response_length){
		  if (small>=response_length) {
		     response[response_length]='\0';
		     p("",response);
		  }
		  else {
		     response[small++]='\n';
		     response[small]='\0';
		     inside++;
		     p("",response);

		     if (!more_r && more_of) {
			flag=OFF;
			fclose(id);
			grab_ansi=OFF;
			return;
		     }

		     if (count ==g_file_bbs.one_line_start-1 && f_message[inside]=='\0');
		     else
			p("","                                               ");
		  }
		  small=0;
	       }
	       else {
		  response[small]=f_message[inside];
		  small++;
		  inside++;
	       }

	       if (count==g_file_bbs.one_line_start-1 && f_message[inside]=='\0')
		  flag=OFF;

	       if (inside>=FSIZE-1)
		  flag=OFF;
	    }
	 }
      }
      else {
	 if (g_file_bbs.detailed[0]=='N')
	    detailed_desc=g_file_bbs.start;
	 else
	    detailed_desc=g_file_bbs.one_line_start;

	 for (count=detailed_desc;count<g_file_bbs.finish;count++) {
	    fseek(id,count*sizeof(f_message),0);
	    fread(f_message,sizeof(f_message),1,id);
	    p("",f_message);

	    if (more_r==OFF && more_of) {
	       flag=OFF;
	       grab_ansi=OFF;
	       fclose(id);
	       return;
	       break;
	    }
	 }
      }
   }
   else {
      for(count=0;count<=num_text_lines;count++) {
	 if (count)
	    p("","                                               ");

	 strupr(strcpy(reserve,text[count].line));
	 a=strlen(reserve);
	 b=0;

	 while (b<a) {
	    mid(reserve,b,strlen(search_for));

	    if (strstr(response,search_for)!=NULL) {
	       mid(text[count].line,b,strlen(search_for));
	       p("[33;44m",response);
	       p("[0;1;32;40m","");
	       b=b+strlen(search_for);
	    }
	    else {
	       send[0]=text[count].line[b];
	       send[1]='\0';
	       p("",send);
	       b++;
	    }
	 }
      }
   }

   if (detailed) {
      p("[36m","                                               Uploaded By ");
      p("[33m",g_file_bbs.who);
      p("","\n");
   }

   fclose(id);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

char binary() {

   char low,high,mid;

   low=0;
   high=items_in_queue-1;

   for (low=0;low<items_in_queue;low++) {
      if ( strcmp(response,file_q[file_index[low]].name)==0)
	 return low;
   }
   return -1;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void file_date(struct find_t *find) {

   unsigned char work[5],num;
   long int val;

   val = (find->wr_date & 0x1E0)>> 5;

   ltoa (val,work,10);

   if (val<10) {
      strcpy(response,"0");
      strcat(response,work);
   }
   else
      strcpy(response,work);


   strcat(response,"-");

   val = (find->wr_date & 0x1F);

   ltoa (val,work,10);

   if (val<10) {
      strcat(response,"0");
      strcat(response,work);
   }
   else
      strcat(response,work);

   strcat(response,"-");

   val = (find->wr_date >>9) + 1980;
   ltoa (val,work,10);
   strcat(response,work);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void upload() {

   unsigned char counter=0;
   unsigned char cnt=0;

   save_file_desc=ON;

   if (!batch_ul) {
      p("[40;36m","\n\nEnter filename ");
      p("[35m","[");
      p("[33m","RETURN");
      p("[32m"," Aborts");
      p("[35m","]");
      p("[36m",": ");


      if (user.ansi[0] !='1') {
	 p("[37;44m","            ");
	 p("[12D","");
      }
      get_files(12);

      if (strlen(response) > 0) {
	 p("","\n");
	 file_exist_upload();


      }
      else {
	 p("[0;1;33;40m","\n\Uploading a file has been ");
	 p("[31m","ABORTED");
	 p("[37m",".\n");
      }
   }
   else {
      dsz_upload(ON);
      dsz_log(ON);
   }

  save_file_desc=OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void file_exist_upload() {

   int status;

   struct find_t fileinfo;

   unsigned char work[81],result,first_pass=OFF,check;

   strcpy(xname,response);

   complete_file_name(response);

   status = _dos_findfirst(reserve,_A_NORMAL,&fileinfo);

   if (status==0) {
     p("[0;1;40;31m","\n");

     p("",strupr(xname));
     p("[36m"," is already on disk");
     p("[37m",".");

   }
   else {
      dsz_upload(ON);
      dsz_log(ON);
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

long int save_file_des (struct mess message[],unsigned char l_number,unsigned char loc[]) {


   unsigned char i,j,colr=6,counter,trc,z;
   unsigned char build[20];
   unsigned char f_message[FSIZE],one_time=OFF;

   long int m_num,size=0;
   long int n_spot;

   size_t rec_size_bbs = sizeof (struct file_bbs);

   struct file_bbs f_file_bbs;

   FILE *fp,*id,*wh;

/*
   file_name_files("loc",0L);

   while (lock(reserve)==0) {
      printf("file is locked\n");
   }
*/
   strcpy(message[loc[l_number]].line,file_response);
   strcpy(message[loc[l_number]].color,file_r_color);
   l_number++;

   file_name_files("bbs",0L);

   if ((fp=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   file_name_files("des",0L);

   if ((id=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   fseek(fp,0L,0);
   fread(&f_file_bbs,rec_size_bbs,1,fp);

   m_num=f_file_bbs.start;

   fseek(id,m_num*sizeof(f_message),0);



   g_file_bbs.start=m_num;

   counter=0;

   for (j=0;j<l_number;j++) {

      i=0;

      while (message[loc[j]].line[i] !='\n') {

	 if (j==l_number-1 && one_time==OFF) {
	    f_message[counter]='\0';
	    fwrite(f_message,sizeof(unsigned char),FSIZE,id);
	    one_time=ON;
	    size++;
	    counter=0;
	    g_file_bbs.one_line_start=m_num+size;
	    i=0;
	 }

	 if (colr !=message[loc[j]].color[i]) {
	    colr=message[loc[j]].color[i];

	    if ( colr>0 && colr <7) {
	       strcpy(build,"[0;1;");
	       trc=colr;
	    }
	    else if (colr>6 && colr <13) {
	       strcpy(build,"[0;1;5;");
	       trc=colr-6;
	    }
	    else if (colr>12 && colr <19) {
	       strcpy(build,"[0;");
	       trc=colr-12;
	    }
	    else if (colr>18 && colr <25) {
	       strcpy(build,"[0;5;");
	       trc=colr-18;
	    }

	    switch (trc) {
	       case 1:
		   strcat(build,"31m");
		   break;
	       case 2:
		   strcat(build,"32m");
		   break;
	       case 3:
		   strcat(build,"33m");
		   break;
	       case 4:
		   strcat(build,"35m");
		   break;
	       case 5:
		   strcat(build,"36m");
		   break;
	       case 6:
		   strcat(build,"37m");
		   break;
	    }
	    for (z=0;z<(unsigned char) strlen(build);z++){
	       if (counter>=FSIZE-1) {
		  f_message[counter]='\0';
		  fwrite(f_message,sizeof(unsigned char),FSIZE,id);


		  size++;
		  counter=0;
	       }
	       f_message[counter] = build[z];
	       counter++;
	    }
	 }

	 if (counter>=FSIZE-1) {
	    f_message[counter]='\0';
	    fwrite(f_message,sizeof(unsigned char),FSIZE,id);
	    size++;
	    counter=0;
	 }
	 f_message[counter] = message[loc[j]].line[i];
	 counter++;
	 i++;
      }



      if (counter>=FSIZE-1) {
	 f_message[counter]='\0';
	 size++;
	 fwrite(f_message,sizeof(unsigned char),FSIZE,id);
	 counter=0;
      }
      f_message[counter] = '\n';
      counter++;
   }

   if (counter>0){
      f_message[counter++]='\0';
      for (i=counter;i<FSIZE-1;i++)
	 f_message[i]=32;
      size++;
      fwrite(f_message,sizeof(unsigned char),FSIZE,id);
   }



   g_file_bbs.finish=m_num+size;


   strcpy(response,first);
   strcat(response," ");
   strcat(response,last);


   strcpy(g_file_bbs.who,response);

/*   g_file_bbs.delete='N';*/



   strcpy(g_file_idx.time,s_time);
   strcpy(g_file_bbs.bps_rate,connect_at);

   strcpy(g_file_bbs.last_person_dl,"None");

   g_file_bbs.attach_msg=0L;

   strcpy(g_file_bbs.sysop_comment,"");
   strcpy(g_file_bbs.last_dl_date,"00-00-00");


   f_file_bbs.start=m_num+size;

   fseek(fp,0L,0);
   fwrite(&f_file_bbs,rec_size_bbs,1,fp);


   fclose(id);
   fclose(fp);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void file_header_bar() {

   /*p("","\n");*/


   p("[0;1;33;44m","ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
   p("[40m","\n");
   p("[44m","บ");
   p("[41;33m"," #   File Name    Size  Date      DL  Zmodem  Description                    ");
   p("[33;44m","บ");
   p("[40m","\n");
   p("[0;1;33;44m","ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
   p("[0;40m","\n");                                                                     /*|*/
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void list_files() {

   unsigned char finish=OFF,flag=ON;



   struct file_idx file_tree ;
   struct file_bbs file_tmp  ;

   long int stack[50];
   long int start=1,sz=0,counter=0,last,area;

   size_t rec_size = sizeof (struct file_idx);
   size_t rec_size2 = sizeof (struct file_bbs);

   long int number;


   FILE *fp;

   int status;

   struct find_t fileinfo;

   struct files diskfile;

   unsigned char work[81];

   more_file=ON;

   if (scan_new_files)
     detail=1;
   else if (text_search)
     detail=0;
   else {
      if (!other_areas || other_areas==1) {
	 if (other_areas==1)
	    other_areas=2;

	 p("[0;1;40;36m","\n\nDetailed Descriptions ");
	 p("[35m","[");
	 p("[33m","Y");
	 p("[37m","/");
	 p("[33m","n");
	 p("[35m","]");
	 p("[36m",": ");

	 if (user.ansi[0] !='1') {
	    p("[37;44m"," ");
	    p("[1D","");
	 }

	 response[0]=get_yn("YN");

	 if (response[0]=='Y' || response[0]==13)
	    detail=1;
	 else
	    detail=0;

      }
   }

   more_select(ON);

   tag=ON;

   if (search_f || text_search) {
      p("[0;1;33;40m","\n\nSearching");
      p(""," file area ");
      p("[31m","#");
      p("[36m",menu[do_function].command);

      area=atol(menu[do_function].command);

      p("[37m",", ");
      p("[32m",file_names[area-1].name);
      p("","...");
   }

   if (scan_new_files || search_f || text_search)
      found_new_files=OFF;
   else {
      send_clear_screen();
      display_f_area(0);
      file_header_bar();
   }

   file_name_files("idx",1L);

   if ((fp=fopen(reserve,"rb+")) == NULL) {
      printf("Cannot open file.idx\n");
      exit(1);
   }

   mark_file=0;

   fseek(fp,1L*rec_size,0);
   fread(&file_tree,rec_size,1,fp);

   stack[counter++]=1L;

   while (flag) {
      while ( (file_tree.left) ) {
	 if (file_tree.left) {
	    fseek(fp,file_tree.left*rec_size,0);
	    stack[counter++]=file_tree.left;
	    fread(&file_tree,rec_size,1,fp);
	 }
      }

      counter--;
      last=0;
      while ( (!file_tree.right) || (file_tree.right==last)) {


	 last=stack[counter];

	 complete_file_name(file_tree.name);
	 status = _dos_findfirst(reserve,_A_NORMAL,&fileinfo);

	 strcpy(diskfile.name,fileinfo.name);

	 file_rec_num=last;

	 diskfile.true_size=(long int) fileinfo.size;
	 ltoa(fileinfo.size/1000L,response,10);

	 strcpy(diskfile.size,response);
	 strcpy(diskfile.des,"Description");

	 file_date(&fileinfo);

	 strcpy(diskfile.date,response);

	 diskfile.record=file_rec_num;

	 if (scan_new_files) {

	   area=atol( menu[do_function].command);
	   l_scan_date2.touched[area-1]='Y';
		 if (check_file_date(l_scan_date.last_date[area-1],file_tree.date,file_tree.time) && file_tree.delete=='N') {
		    if (!found_new_files) {
		       send_clear_screen();
		       display_f_area(1);
		       file_header_bar();
		       found_new_files=ON;
		    }
		    display_file_info(&diskfile,detail);
		 }
	 }
	 else if (search_f && file_tree.delete=='N') {
	    if (cut_file(file_tree.name)) {
	       if (!found_new_files) {
		  send_clear_screen();
		  display_f_area(2);
		  found_new_files=ON;
		  file_header_bar();
		  found_new_files=ON;
	       }
	       display_file_info(&diskfile,detail);
	    }
	 }
	 else if (file_tree.delete=='N')
	    display_file_info(&diskfile,detail);

	 counter--;

	 if (counter==-1)
	    break;

	 fseek(fp,stack[counter]*rec_size,0);
	 fread(&file_tree,rec_size,1,fp);
      }


      if (more_r==OFF && more_of)
	 break;

      if (counter==-1)
	 flag=OFF;
      else {
	 complete_file_name(file_tree.name);
	 status = _dos_findfirst(reserve,_A_NORMAL,&fileinfo);
	 strcpy(diskfile.name,fileinfo.name);

	 file_rec_num=stack[counter];

	 ltoa(fileinfo.size/1000L,response,10);

	 diskfile.true_size=(long int) fileinfo.size;
	 strcpy(diskfile.size,response);

	 strcpy(diskfile.des,"Description");
	 file_date(&fileinfo);
	 strcpy(diskfile.date,response);
	 diskfile.record=file_rec_num;

	 if (scan_new_files ) {

	    area=atol( menu[do_function].command);
	    l_scan_date2.touched[area-1]='Y';

	    if (check_file_date(l_scan_date.last_date[area-1],file_tree.date,file_tree.time) && file_tree.delete=='N') {
	       if (!found_new_files) {
		  send_clear_screen();
		  display_f_area(1);
		  file_header_bar();
		  found_new_files=ON;
	       }
	       display_file_info(&diskfile,detail);
	    }
	 }
	 else if (search_f && file_tree.delete=='N') {
	    if (cut_file(file_tree.name)) {
	       if (!found_new_files) {
		  send_clear_screen();
		  display_f_area(2);
		  found_new_files=ON;
		  file_header_bar();
		  found_new_files=ON;
	       }
	       display_file_info(&diskfile,detail);
	    }
	 }
	 else if (file_tree.delete=='N')
	    display_file_info(&diskfile,detail);

	 file_tree.left=file_tree.right;
      }

      if (more_r==OFF && more_of)
	 break;
   }

   fclose(fp);

   more_select(OFF);
   more_file=OFF;

   if (mark_file && more_r!=2) {

      p("[0;1;40;31m","\n[");
      p("[33m","Press Return");
      p("[31m","]  ");
      p("","[");
      p("[33m","#");
      p("[32m",", ");
      p("[33m","#-#");
      p("[32m",",");
      p("[33m"," #,#,#...");
      p("[31m","]");

      if (text_search || search_f || scan_new_files) {
	 p("[36m"," Tag Files ");
	 p("[31m","[");
	 p("[33m","Q");
	 p("[31m","]");
	 p("[32m","uit: ");
     }
     else {
	p("[36m"," Tag Files : ");
     }

     strcpy(options,"");

     hot_key_number();

     if (big)
	tag_the_file();
     }
     else {

	if (found_new_files==OFF && scan_new_files);
	else if ((!found_new_files && search_f) || (text_search && st==OFF)) {
	   if (!other_areas) {
	      p("[0;1;31;40m","\n\nNo matches where made in this area.");
	      p("[32;40m","\n\nSearch other areas ");
	      p("[34m","[");
	      p("[35m","Y/n");
	      p("[34m","]");
	      p("[36m",": ");
	      p("[37;44m"," ");
	      p("[1D","");

	      response[0]=get_yn("YN");

	      if (response[0]=='Y') {
		 other_areas=2;
	      }
	   }
	}
	else
	   press_return();
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void list_protocols() {

   p("[0;1;35;40m","\n[");
   p("[33m","A");
   p("[35m","]");
   p("[31m"," Z");
   p("[36m","modem Batch");

   p("[35m","\n[");
   p("[33m","B");
   p("[35m","]");
   p("[31m"," Y");
   p("[36m","modem Batch");

   p("[35m","\n[");
   p("[33m","C");
   p("[35m","]");
   p("[31m"," Y");
   p("[36m","modem");
   p("[37m","-");
   p("[31m","G");

   p("[35m","\n[");
   p("[33m","D");
   p("[35m","]");
   p("[31m"," X");
   p("[36m","modem");

   p("[35m","\n[");
   p("[33m","E");
   p("[35m","]");
   p("[31m"," X");
   p("[36m","modem");
   p("[37m","-");
   p("[31m","1");
   p("[31m","K");

   p("[33m","\n\nEnter Protocol: ");

   if (user.ansi[0] !='1') {
      p("[37;44m"," ");
      p("[1D","");
   }

   strcpy(options,"ABCDE");
   proto=hot_key();

   if (proto==13){
   }
   else if (proto=='D' || proto=='E')
      batch_ul=OFF;
   else
      batch_ul=ON;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void dsz_upload(unsigned char ul) {


   double diff;






   unsigned char cmdcom[60];

   unsigned char *save_s;
   unsigned char *my_env[5], prog[80];
   static unsigned char *args[25];
   int r;

   my_env[0] = "DSZLOG=C:\\BZ\\DSZ.1";
   my_env[1] = NULL;



   *cmdcom = '\0';
   strcpy(cmdcom, "COMMAND.COM");

   args[0] = cmdcom;
   args[1] = "/c";
   args[2] = "dsz.exe";
   args[3] = "1";
   args[4] = "speed";
   args[5] = "38400";
   args[6] = "restrict";













   switch (proto) {

      case 'A':

	 p("[0;1;33;40m","\n\nBegin ");

	 if (ul)
	    p("","sending");
	 else
	    p("","receiving");

	 p(""," files via");
	 p("[31m"," Z");
	 p("[36m","modem Batch");
	 p("[33m",".  ");

	 if (ul) {
	    args[7] = "pV1";
	    args[8] = "pa9000";
	    args[9] = "pd1";
	    args[10] = "z";
	    args[11] = "pp50";
	    args[12] = "pt100";
	    args[13] = "rz";
	    args[14] = NULL;

	 }
	 else {
	    args[6] = "pV1";
	    args[7] = "pa9000";
	    args[8] = "z";
	    args[9] = "pL1024";
	    args[10] = "pT600";
	    args[11] = "sz";
	    args[12]= "-k";
	    args[13] = "@c:\\bz\\file.1";
	    args[14] = NULL;
	 }
	 break;

      case 'B':

	 p("[0;1;33;40m","\n\nBegin Sending files via");
	 p("[31m"," Y");
	 p("[36m","modem Batch");
	 p("[33m",".  ");

	 if (ul) {
	    args[7] = "pV1";
	    args[8] = "pa9000";
	    args[9] = "pd1";
	    args[10] = "rb";
	    args[11] = "-k";
	    args[12] = NULL;
	 }
	 else {
	    args[6] = "pV1";
	    args[7] = "pa9000";
	    args[8] = "sb";
	    args[9] = "-k";
	    args[10] = "@c:\\bz\\file.1";
	    args[11] = NULL;
	 }
	 break;
      case 'C':

	 p("[0;1;33;40m","\n\nBegin Sending files via");
	 p("[31m"," Y");
	 p("[36m","modem-G");
	 p("[33m",".  ");

	 if (ul) {
	    args[7] = "pV1";
	    args[8] = "pa9000";
	    args[9] = "pd1";
	    args[10] = "rb";
	    args[11] = "-g";
	    args[12] = "-k";
	    args[13] = NULL;
	 }
	 else {
	    args[6] = "pV1";
	    args[7] = "pa9000";
	    args[8] = "sb";
	    args[9] = "-g";
	    args[10] = "-k";
	    args[11] = "@c:\\bz\\file.1";
	    args[12] = NULL;
	 }
	 break;

      case 'D':

	 p("[0;1;33;40m","\n\nBegin Sending ");
	 p("[31m",xname);
	 p("[33m"," via");
	 p("[31m"," X");
	 p("[36m","modem");
	 p("[33m",".  ");

	 if (ul) {
	    args[7] = "pV1";
	    args[8] = "rx";
	    args[9] = xname;
	    args[10] = NULL;
	 }
	 else {
	    args[6] = "pV1";
	    args[7] = "sx";
	    args[8] = xname;
	    args[9] = NULL;
	 }
	 break;

      case 'E':

	p("[0;1;33;40m","\n\nBegin Sending ");
	p("[31m",xname);
	p("[33m"," via");
	p("[31m"," X");
	p("[36m","modem");

	p("[37m","-");
	p("[31m","1");
	p("[31m","K");
	p("[33m",".  ");

	if (ul) {
	   args[7] = "pV1";
	   args[8] = "rx";
	   args[9] = "-k";
	   args[10] = xname;
	   args[11] = NULL;
	}
	else {
	   args[6] = "pV1";
	   args[7] = "sx";
	   args[8] = "-k";
	   args[9] = xname;
	   args[10] = NULL;
	}
	break;
   }

   p("[0;1;40;32m","\n\nPress");
   p("[31m"," CTRL");
   p("[37m","-");
   p("[31m","X");
   p("[32m"," several times to abort");
   p("[37m",".\n\n");

   save_s = (unsigned char *) malloc(2 * (25) * (80));

   if (!save_s)
      exit(1);

   save_video(0,25,0,80,save_s);
   cls(0,0,24,79);

   unlink("dsz.1");

   if (ul) {
      strcpy(response,file_path);
      strcat(response,menu[do_function].command);

/*      _chdrive(4);*/


      chdir(response);
   }
   /*r = spawnvpe( P_WAIT, prog, args, my_env );*/



   g_date();


   time (&in_door);


   delay(1000);

   r = spawnvp( P_WAIT, cmdcom, args);

	 time (&time_out_start);

   time (&out_door);

      check_time();


   delay(500);

   if (ul) {
/*      _chdrive(3);*/
      chdir("C:\\BZ");
   }


   purge_in();

   restore_video(0,25,0,80,save_s);
   goto_xy(xloc+1,yloc);
   free(save_s);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void dsz_log(unsigned char ul) {

   FILE *fp;

   struct index idx;
   struct who who_tf;

   struct find_t fileinfo;

   unsigned char dsz_line[141],success,first_time=OFF;
   unsigned char work[80],cnt,loop=0,flag=ON,chk=OFF,cnt2;
   double diff;
   char where;

   int lines,status,count;

   long int dsz_size,tot=0,get_it,answer,answer2,calc;

   if ((fp=fopen("dsz.1","r")) == NULL) {
      p("[32m","\n\nNo files where sent.");
      fclose(fp);
      return;
   }

   lines=0;

   while (!feof(fp)) {
      lines++;
      fgets(dsz_line,140,fp);
   }
   fclose(fp);

   ltoa(lines-1,work,10);

   if ((fp=fopen("dsz.1","r")) == NULL) {
      exit(1);
   }


   while (!feof(fp)) {

      if (fgets(dsz_line,140,fp) !=NULL)  {
	 mid(dsz_line,50,70);
	 strcpy(work,response);

	 cnt=0;
	 while(work[cnt]!=' ')
	    cnt++;

	 work[cnt]='\0';

	 strupr(work);
	 strcpy(response,work);

	 if (!ul) {
	    cnt=strlen(work);

	    while (work[cnt--]!='\\');
	       mid(work,cnt+2,12);
	       strcpy(work,response);
	       where=binary();
	    }

	    mid(dsz_line,1,7);
	    dsz_size = atol(response);

	    if (!ul) {

	       if (where==-1)
		  success=OFF;
	       else if (  dsz_size>=file_q[file_index[where]].true_size)
		  success=ON;
	       else
		  success=OFF;
	    }
	    else {
	       complete_file_name(work);

	       status = _dos_findfirst(reserve,_A_NORMAL,&fileinfo);
	       if (!status) {
		  if (dsz_size >=fileinfo.size) {

		     if (!first_time) {
			 first_time=ON;


    diff = difftime (out_door, in_door);

    diff = (long int) diff/60L;


    if (diff>0)

      time_allowed=time_allowed+diff;





   }

		     success=ON;
  }

		  else
		     success=OFF;
	       }
	       else
		  success=OFF;
	    }


	    if (ul){
	       if (success) {
		  strcpy(file_temp,strupr(work));
		  mid(dsz_line,1,7);
		  calc=atol(user.ul_num);
		  calc++;
		  ltoa(calc,user.ul_num,10);
		  calc=atol(user.ul_kb);
		  calc=calc+atol(response);
		  ltoa(calc,user.ul_kb,10);

	       while (flag) {
		  p("[0;1;40;36m","\n\nPlease Describe ");
		  p("[31m",work);
		  p("[36m"," using a single line.\n\n");
		  response[0]='\0';
		  strncat(response,blnk,strlen(work)+2);
		  p("[31m",response);
		  p("","ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท\n");
		  p("[33m",work);
		  p("[32m",":  ");

		  if (user.ansi[0] !='1') {
		     p("[37;44m","                                ");
		     p("[32D","");
		  }

		  get_single_line(32,1);

		  if (strlen(response)<10) {
		     p("[0;1;40;32m","\n\nPlease make the");
		     p("[31m"," description ");
		     p("[32m","a bit longer");
		     p("[37m",".");
		  }
		  else
		     flag=OFF;
	       }

	       flag=ON;
	       strcpy(file_response,response);
	       strcpy(file_r_color,r_color);

	       p("[0;1;40;32m","\n\nWould you like to describe ");
	       p("[31m",work);
	       p("[32m"," in more detail ");
	       p("[35m","[");
	       p("[33m","Y");
	       p("[37m","/");
	       p("[33m","n");
	       p("[35m","]");
	       p("[32m",": ");

	       if (user.ansi[0] !='1') {
		  p("[37;44m"," ");
		  p("[1D","");
	       }

	       response[0]=get_yn("YN");
	       save_file_desc=ON;

	       if (response[0]=='N') {
		  p("[0;1;40;32m","\n\nSaving file information");
		  p("[37m","...");
		  save_file_desc=2;
		  strcpy(g_file_bbs.detailed,"N");
	       }
	       else {
		  strcpy(g_file_bbs.detailed,"Y");
		  intro_line_editor("Nil",'N');
	       }
	       line_editor(&idx,&who_tf);


	       save_file (OFF,OFF);

	       p("[32m","\n\nThanks for the ");
	       p("[31m","Upload");
	       p("[37m","!");
	       p("[36m","!");
	       p("[35m","!");
	       chk++;
	    }
	 }
	 else {

	    if (success) {
	       tot++;
	       calc=atol(user.dl_num);
	       calc++;
	       ltoa(calc,user.dl_num,10);
	       calc=atol(user.dl_kb);
	       calc=calc+file_q[file_index[where]].true_size;
	       ltoa(calc,user.dl_kb,10);

	       update_dl_counter(where);

	       delete_from_queue(where+1,where+1);
	    }

	 }
      }
   }

   if (!ul) {
      ltoa(tot,response,10);
      p("[0;1;33;40m","\n\n");
      p("",response);
      p("[32m"," file");
      p("[35m","(");
      p("[32m","s");
      p("[35m",")");
      p("[32m"," successfully downloaded");
      p("[37m",".");

   }
   else {
      ltoa(chk,response,10);
      p("[0;1;33;40m","\n\n");
      p("",response);
      p("[32m"," file");
      p("[35m","(");
      p("[32m","s");
      p("[35m",")");
      p("[32m"," successfully uploaded");
      p("[37m",".");
   }

   fclose(fp);
   save_file_desc=OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void complete_file_name(unsigned char *fname) {

   strcpy(reserve,file_path);
   strcat(reserve,menu[do_function].command);
   strcat(reserve,"\\");
   strcat(reserve,fname);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void upload_header() {

   send_clear_screen();

   display_f_area(0);
   p("[0;1;41;33m","ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ");
   p("[40m","\n");
   p("[41m","ณ");
   p("[44;33m","  Uploading Files  ");
   p("[41;33m","ณ");
   p("[40m","\n");
   p("[41;33m","ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");
   p("[40m","\n");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_f_area(unsigned char which_one) {

   long int area,m_bnd;
   unsigned char sz;

   if (which_one==0)
      sz=64;
   else if (which_one==1)
      sz=44;
   else
      sz=40;

   p("[0;1;33;44m","File Area ");
   p("[31m","#");

   format(menu[do_function].command,2);
   p("[36m",response);

   area=atol(menu[do_function].command);

   p("[37m",", ");

   format(file_names[area-1].name,sz);

   p("[32m",response);

   if (which_one==1) {
      p("[33m","[");
      p("[32m","New File Search");
      p("[33m","]   ");
   }
   else if (which_one==2) {
      p("[33m","[");
      p("[32m","Searching for files");
      p("[33m","]   ");
   }
   else if (which_one==3) {
      p("[33m","[");
      p("[32m","Searching for Text");
      p("[33m","]    ");
   }
   else if (which_one==4) {
      p("[33m","[");
      p("[32m","Search for New Files");
      p("[33m","]  ");
   }

   p("[40m","\n\n");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void queue_is_empty() {

   p("[33;40m","\n\nYour ");
   p("[31m","download queue ");
   p("[33m","is empty");
   p("[37m",".\n");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void calc_t_time(long int size) {


   long int factor,tm;
   double frac,i_part;

   if (bps==0 && erc==1)
      factor=70;
   else if (bps==0 && erc==0)
      factor=60;

   else if (bps==1 && erc==1)
      factor=130;
   else if (bps==1 && erc==0)
      factor=120;

   else if (bps==2 && erc==1)
      factor=230;
   else if (bps==2 && erc==0)
      factor=220;

   else if (bps==3 && erc==1)
      factor=460;
   else if (bps==3 && erc==0)
      factor=450;

   else if (bps==4 && erc==1)
      factor=920;
   else if (bps==4 && erc==0)
      factor=910;

   else if (bps==5 && erc==1)
      factor=1300;
   else if (bps==5 && erc==0)
      factor=1200;

   else if (bps==6 && erc==1)
      factor=1500;
   else if (bps==6 && erc==0)
      factor=1490;

   else if (bps==7 && erc==1)
      factor=1600;
   else if (bps==7 && erc==0)
      factor=1590;

   else if (bps==8 && erc==1)
      factor=1800;
   else if (bps==8 && erc==0)
      factor=1790;


   tm=(size/factor)/60;

   ltoa(tm,transfer_time,10);

   true_time= ((double) size/(double) factor)/(double) 60;

   frac=modf(true_time,&i_part);

   if (frac>=.60) {
      true_time++;
      true_time=true_time-.60;
   }



}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void delete_from_queue(long int end,long int start)  {


   long int temp,temp2,i,sum;

   sum=(end-start)+1;
   start--;
   if (sum==1) {
      temp=file_index[start];
      for (i= start;i<(items_in_queue-1);i++)
	 file_index[i]=file_index[i+1];
      file_index[i] = temp;
   }
   else {
      temp=file_index[end];
      for (i=0;i<(items_in_queue-1);i++) {
	 temp2=file_index[i+start];
	 file_index[i+start]=temp;
	 file_index[end]=temp2;
	 temp=file_index[++end];
      }
   }
   items_in_queue=items_in_queue-(unsigned char)sum;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void update_dl_counter(long int number) {

   FILE *fp;

   size_t rec_size_bbs = sizeof (struct file_bbs);

   struct file_bbs temp;

   list_batch=ON;

   strcpy(true_area,file_q[file_index[number]].area);

   file_name_files("bbs",0L);

   if ((fp=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }


   list_batch=OFF;


   fseek(fp,file_q[file_index[number]].record*rec_size_bbs,0);
   fread(&temp,rec_size_bbs,1,fp);

   temp.dl_number++;


   strcpy(response,first);
   strcat(response," ");
   strcat(response,last);

   strcpy(temp.last_person_dl,response);
   strcpy(temp.last_dl_date,s_date);

   fseek(fp,file_q[file_index[number]].record*rec_size_bbs,0);
   fwrite(&temp,rec_size_bbs,1,fp);

   fclose(fp);

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void tag_the_file() {

   long int answer,answer2,r,c;
   unsigned char cnt;

   tag_bypass=ON;

   if (!kind) {
      answer=atol(one);

      if (two[0]=='\0')
	 answer2=answer;
      else
	 answer2=atol(two);

      if (answer2<answer || answer2>mark_file);
      else {
	 for(cnt=answer;cnt<=answer2;cnt++) {
	    strcpy(response,tag_files[cnt].tag);
	    find_files();


	 }
      }
   }
   else {

      r=0;
      c=0;

      while (cut[r].level[c] != '\0') {

	 answer=atol(cut[r].level);

	 if (answer<=mark_file) {
	    strcpy(response,tag_files[answer].tag);
	    find_files();
	 }
	 r++;
      }
   }
   tag_bypass=OFF;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char check_file_date(unsigned char date1[11], unsigned char date2[11],unsigned char tim[9]) {

   long int area;


   int m1,d1,y1;
   int m2,d2,y2;


   int f_hrs,f_min;
   int s_hrs,s_min;


   mid(date1,6,4);
   y1=atoi(response);

   mid(date1,3,2);
   d1=atoi(response);

   mid(date1,0,2);
   m1=atoi(response);

   mid(date2,6,4);
   y2=atoi(response);

   mid(date2,3,2);
   d2=atoi(response);

   mid(date2,0,2);
   m2=atoi(response);

   area=atol(menu[do_function].command);

   area--;

   if (y2>y1)
      return 1;
   else if (y2<y1)
      return 0;
   else {
      if (m2>m1)
	 return 1;
      else if (m2<m1)
	 return 0;
      else {
	 if (d2>d1)
	    return 1;
	 else if (d2<d1)
	    return 0;
	 else {
	    mid(tim,0,2);
	    f_hrs = atoi(response);

	    mid(tim,3,2);
	    f_min = atoi(response);


	    if (f_hrs==12 && tim[6] == 'a')
	       f_hrs=0;
	    else if (tim[6] == 'p')
	       f_hrs=f_hrs+12;


	    mid(l_scan_date.last_time[area],0,2);
	    s_hrs = atoi(response);
	    mid(l_scan_date.last_time[area],3,2);

	    s_min = atoi(response);


	    if (s_hrs==12 && l_scan_date.last_time[area][6] == 'a' )
	       s_hrs=0;
	    else if (l_scan_date.last_time[area][6] == 'p')
	       s_hrs=s_hrs+12;


	    if (f_hrs>s_hrs)
		 return 1;
	    else if (f_hrs<s_hrs)
		 return 0;
	    else {
	       if (f_min>=s_min)
		  return 1;
	       else if (f_min<s_min)
		  return 0;
	    }
	 }
      }
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char find_text() {

   FILE *fp;
   FILE *id;

   unsigned char f_message[FSIZE],flag=OFF;

   long int count,r=0,c=0,cnt=0;

   size_t rec_size2 = sizeof (struct file_bbs);

   file_name_files("bbs",1L);

   if ((fp=fopen(reserve,"rb+")) == NULL) {
      printf("cannot open files.bbs\n");
      exit(1);
   }

   fseek(fp,file_rec_num*rec_size2,0);
   fread(&g_file_bbs,rec_size2,1,fp);

   fclose(fp);

   file_name_files("des",0L);

   if ((id=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   for (count=g_file_bbs.one_line_start;count<g_file_bbs.finish;count++) {

      fseek(id,count*sizeof(f_message),0);
      fread(f_message,sizeof(f_message),1,id);
      cnt=0;

      while (cnt<FSIZE-1) {
	 if (f_message[cnt] == '\n') {
	    text[r].line[c++]=f_message[cnt];
	    break;
	 }
	 else {
	    if (f_message[cnt]=='')
	       flag=ON;

	    if (flag==ON && f_message[cnt]=='m')
	       flag=OFF;

	    else if (!flag)
	       text[r].line[c++]=f_message[cnt];
	 }
	 cnt++;
      }
   }

   text[r].line[c]='\0';
   strcpy(response,text[r].line);
   num_text_lines=0;

   if (strstr(strupr(response),strupr(search_for))!=NULL) {
      fclose(id);
      return 1;
   }

   if (g_file_bbs.detailed[0]=='N') {
      fclose(id);
      return 0;
   }
   else {
      flag=OFF;
      cnt=0;
      c=0;
      r=0;
      for (count=g_file_bbs.start;count<g_file_bbs.one_line_start;count++) {
	 cnt=0;
	 fseek(id,count*sizeof(f_message),0);
	 fread(f_message,sizeof(f_message),1,id);

	 while (cnt<FSIZE-1) {
	    if (f_message[cnt] == '\0')
	       break;
	    else if (f_message[cnt] == '\n') {
	       text[r].line[c++]=f_message[cnt];
	       text[r].line[c++]='\0';
	       c=0;
	       r++;
	    }
	    else {
	       if (f_message[cnt]=='')
		  flag=ON;
	       if (flag==ON && f_message[cnt]=='m')
		  flag=OFF;
	       else if (!flag)
		  text[r].line[c++]=f_message[cnt];
	    }
	    strcpy(response,text[r].line);
	    cnt++;
	 }

	 strcpy(response,text[r].line);
      }
   }

   num_text_lines=r-1;
   fclose(id);

   for (count=0;count<=num_text_lines;count++) {
      strcpy(response,text[count].line);
      if (strstr(strupr(response),strupr(search_for))!=NULL) {
	 return 1;
      }
   }

   return 0;
}




