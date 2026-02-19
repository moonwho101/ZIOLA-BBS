#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h> /* exit(1) */
#include <ctype.h>
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


void who(unsigned char sysop) {

   struct index idx;
   struct who who_tf;

   unsigned char target[81],flag=ON,result;
   unsigned char to_all='N';

   g_date();

   send_clear_screen();

   display_m_area();

   p("[0;1;33;44m","ึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
   p("[40m","\n");
   p("[44m","บ");
   p("[41;33m",    "  Posting a Message  ");
   p("[33;44m","บ");
   p("[40m","\n");
   p("[0;1;33;44m","ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");

   p("[40m","\n");

   if (!sysop) {

   while (flag) {

   p("[32m","\nEnter");
   p("[33m"," ALL ");
   p("[32m","if this message is addressed to every user.");


   p("[1;33;40m","\n\nWho is this message to: ");
   if (user.ansi[0] !='1') {
      p("[34;44m","                                        ");
      p("[40D","");
   }
   p("[1;37;44m","");
   get_string(40,0);

   strcpy(target,cut[0].level);

   if (target[0] == '\0') {
      p("[40;31m","\n\nWrite message has been aborted.");
      return;
   }

   if (strcmp(cut[0].level,"All")==OFF) {
      strcpy(target,"All");
      to_all='Y';
      break;
   }

   strcat(target," ");
   strcat(target,cut[1].level);

   strcpy(temp,target);

   result=save_user(2,OFF);

   if (result)
       flag=OFF;
   else {
    p("[40;31m","\n\n");
    p("",target);
    p("[36m"," is not in the user file, check your spelling.");


   }
   }
   }
   else {
     strcpy(target,"Mark Longo");
     p("[1;36;40m","\nPosting a comment to ");
     p("[33m",target);

   }


   flag=ON;

   while (flag) {



   p("[0;1;32;40m","\n\nEnter the message subject: ");

   if (user.ansi[0] !='1') {
      p("[34;44m","                                        ");
      p("[40D","");
   }
   p("[0;1;37;44m","");

   get_single_line(40,0);

   if (response[0] == '\n') {
      p("[40;31m","\n\nWrite message has been aborted.");
      return;
   }
   else if (strlen(response) < 5)
      p("[40;31m","\n\nMessage Description must be great than 5 characters.");
   else
      flag=OFF;

   }


   strcpy(idx.subject,response);
   strcpy(idx.private,"N");

   if (to_all=='N' && !sysop) {

   p("[1;31;40m","\n\nIs this a private message ");

   p("[34m","[");
   p("[35m","y/N");
   p("[34m","]");
   p("[32m",": ");

   if (user.ansi[0] !='1') {
      p("[34;44m"," ");
      p("[1D","");
   }
   p("[37m","");
   response[0]=get_yn("NY");

   if (response[0]=='Y')
     strcpy(idx.private,"Y");



   }

   if (sysop)
       strcpy(idx.private,"Y");



   strcpy(who_tf.to,target);
   strcpy(who_tf.received,"N");


   strcpy(who_tf.from,first);
   strcat(who_tf.from," ");
   strcat(who_tf.from,last);

   strcpy(idx.date,s_date);
   strcpy(idx.time,s_time);

   strcpy(idx.delete,"N");

   idx.parent=0;
   idx.prev_sib=0;


   intro_line_editor(who_tf.to,idx.private[0]);

   line_editor(&idx,&who_tf);

   file_name("loc",0L);
   file_unlock(reserve);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


long int save_message (struct mess message[],unsigned char l_number,unsigned char loc[],struct index *idx,struct who *who_tf){

   struct index message_data;

   unsigned char i,j,colr=6,counter,trc,z;
   unsigned char build[20];
   unsigned char f_message[MSIZE];

   long int m_num,size=0;
   long int n_spot;


   size_t rec_size_bbs = sizeof (f_message);
   size_t rec_size_idx = sizeof (struct index);
   size_t rec_size_who = sizeof (struct who);


   FILE *fp,*id,*wh;


   file_name("loc",0L);

   while (file_lock(reserve)==0) {
      printf("file is locked\n");
   }



   file_name("bbs",0L);

   if ((fp=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   file_name("idx",0L);

   if ((id=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   file_name("who",0L);

   if ((wh=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }



   fseek(id,0L,0);
   fread(&message_data,rec_size_idx,1,id);

   m_num=message_data.parent;
   n_spot=message_data.sib;

   fseek(fp,n_spot*rec_size_bbs,0);

   counter=0;

   for (j=0;j<l_number;j++) {

      i=0;

      while (message[loc[j]].line[i] !='\n') {



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
	       if (counter>=MSIZE-1) {
		  f_message[counter]='\0';
		  fwrite(f_message,sizeof(unsigned char),MSIZE,fp);


		  size++;
		  counter=0;
	       }
	       f_message[counter] = build[z];
	       counter++;
	    }
	 }

	 if (counter>=MSIZE-1) {
	    f_message[counter]='\0';
	    fwrite(f_message,sizeof(unsigned char),MSIZE,fp);
	    size++;
	    counter=0;
	 }
	 f_message[counter] = message[loc[j]].line[i];
	 counter++;
	 i++;
      }



      if (counter>=MSIZE-1) {
	 f_message[counter]='\0';
	 size++;
	 fwrite(f_message,sizeof(unsigned char),MSIZE,fp);
	 counter=0;
      }
      f_message[counter] = '\n';
      counter++;
   }




   if (counter>0){
      f_message[counter++]='\0';
      for (i=counter;i<MSIZE-1;i++)
	 f_message[i]=32;
      size++;
      fwrite(f_message,sizeof(unsigned char),MSIZE,fp);
   }



   message_data.parent =++m_num;
   message_data.sib=n_spot+size;
   message_data.child=0;
   message_data.start=0;
   message_data.end=0;
   message_data.reply=0;
   message_data.last_reply=0;

   fseek(id,0L,0);
   fwrite (&message_data, rec_size_idx,1,id);

   idx->sib=0;
   idx->child=0;
   idx->start=n_spot;
   idx->end=size;
   idx->reply=0;
   idx->last_reply=0;

   fseek(id,(long) m_num*rec_size_idx,0);
   fwrite (idx, rec_size_idx,1,id);


   fseek(wh,(long) m_num*rec_size_who,0);
   fwrite (who_tf, rec_size_who,1,wh);

   fseek(id,(long) m_num*rec_size_idx,0);
   fread(&message_data,rec_size_idx,1,id);

   fclose(id);
   fclose(fp);
   fclose(wh);


   return m_num;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


long int line_editor(struct index *idx,struct who *who_to_from) {

   static struct mess message[100];
   struct mess *t;

   unsigned char *r,*u,cont=OFF,flag2=ON;
   unsigned char d=6;
   unsigned char counter=0,back=0;
   unsigned char flag=ON;
   unsigned char c,temp_c;
   register unsigned char l_number=1;
   unsigned char build[41];
   unsigned char colr=OFF,i,j;
   unsigned char loc[99],tab,empty;
   unsigned char across,temp_flag;
   char temp_line;

   unsigned char hor;
   unsigned char mid;
   unsigned char the_bottom;
   unsigned char how_far_back;

   long int mess_saved;

   if (save_file_desc) {
      hor=31;
      mid=15;
      the_bottom=20;
      how_far_back=16;

   }

   else {
      hor=73;
      mid=40;
      the_bottom=99;
      how_far_back=33;
   }



   for (i=0;i<99;i++)
      loc[i]=i;

   u=&loc[0];
   t=&message[0];

   for (i=0;i<81;i++)
      message[0].color[i]=6;

   r=&d;

   if (save_file_desc==2) {


	 save_file_des(t,0,u);
	 return 1;
    }



   display_m_bar();
   d_line(l_number,1,0);

   while (flag) {
      c=0;
      while (c==0 && colr==OFF)
	 c= (unsigned char) inkey();
      if (colr==ON) {
	 c=temp_c;
	 colr=OFF;
      }


      if (c==13 && l_number==the_bottom) {
	 p("[40;31m","\n\nYou have reached the end of the message.");
	 c=27;
      }


      if (!is_valid(c)) {
      }
      else if (c==13) {
	 if (cont==OFF) {
	    message[loc[l_number-1]].color[counter]='\0';
	    response[counter++] = '\n';
	    response[counter] = '\0';
	    strcpy(message[loc[l_number-1]].line,response);
	 }
	 else
	    cont=OFF;


	 l_number++;

	 d_line(l_number,1,1);
	 counter=0;
	 message_color((int) d);
	 for (i=0;i<81;i++)
	    message[loc[l_number]].color[i]=6;


      }

      else if (c==10) {
      }

      else if (c == 9) {


	 if (yloc<8)
	    tab=8;
	 else if (yloc<16)
	    tab=16;
	 else if (yloc<24)
	    tab=24;
	 else if (yloc<32)
	    tab=32;
	 else if (yloc<40)
	    tab=40;
	 else if (yloc<48)
	    tab=48;
	 else if (yloc<56)
	    tab=56;
	 else if (yloc<64)
	    tab=64;
	 else if (yloc<72)
	    tab=72;
	 else
	    tab=78;

	for(i=yloc;i<tab;i++) {
	  message[loc[l_number-1]].color[counter]=d;
	  response[counter++]=32;
	  send[0]=32;
	  p("",send);
	}

      }



      else if (c==1 && user.ansi[0] !='1') {
	 temp_c=get_colr(r);
	 message[loc[l_number-1]].color[counter] = d;
	 colr=ON;
      }
      else if (c==8 && counter>0){
	 delete(1);
	 counter--;
	 response[counter]=32;
	 d=message[loc[l_number-1]].color[counter];
	 message[loc[l_number-1]].color[counter]=6;
	 message_color((int) d);
      }
      else if(c==8 && counter==0 && l_number !=1) {

	 display_m_bar();
	 l_number--;
	 strcpy(response,message[loc[l_number-1]].line);
	 j=6;
	 d_line(l_number,1,0);
	 counter= (unsigned char)    strlen(response);
	 counter--;

	 if (counter==hor+1)  /*74!!!*/
	    response[counter--]=32;

	 for (i=0;i<counter;i++) {
	    if (message[loc[l_number-1]].color[i] !=j) {
	       message_color((int) message[loc[l_number-1]].color[i]);
	       j=message[loc[l_number-1]].color[i];
	       d=j;
	    }

	    send[0]=response[i];
	    p("",send);
	 }
      }


      else if (c==20) {
	g_date();

	    message[loc[l_number-1]].color[counter]='\0';
	    response[counter++] = '\n';
	    response[counter] = '\0';
	    strcpy(message[loc[l_number-1]].line,response);

	p("[0;1;32;40m","\n\nCurrent Time: ");
      p("[36m",s_time);
      p("[32m","\nCurrent Date: ");
      p("[36m",s_date);
		  if (l_number==0) {
		     colr=ON;
		     cont=ON;
		     temp_c=13;

		     display_m_bar();
		  }
		  else {
		     colr=ON;

		     temp_c=8;
		     counter=0;
		     l_number++;
		  }


      }
      else if (c==3 && user.ansi[0] !='1') {


	    message[loc[l_number-1]].color[counter]='\0';
	    response[counter++] = '\n';
	    response[counter] = '\0';
	    strcpy(message[loc[l_number-1]].line,response);

       color_chart();
		  if (l_number==0) {
		     colr=ON;
		     cont=ON;
		     temp_c=13;

		     display_m_bar();
		  }
		  else {
		     colr=ON;

		     temp_c=8;
		     counter=0;
		     l_number++;
		  }



      }

      else if (c==27) {

	 if (counter>0) {
	    message[loc[l_number-1]].color[counter]='\0';
	    response[counter++] = '\n';
	    response[counter] = '\0';
	    strcpy(message[loc[l_number-1]].line,response);
	 }
	 else
	    l_number--;

	 while (flag2) {
	    strcpy(options,"LDRIECAS");
	    p("[0;40m","\n\n");



	    if (user.ansi[0]=='3') {
	    p("[1;33;41m","ีอต");
	    p("[36m"," Line Editor Options ");
	    p("[33m","ฦออออออออออออธ");
	    p("[40m","\n");
	    p("[41m","ณ");
	    p("[44m","                                    ");
	    p("[41m","ณ");
	    p("[40m","\n");
	    p("[41m","ณ");
	    p("[31;44m"," [");
	    p("[37m","L");
	    p("[31m","]");
	    p("[32m","ist");
	    p("[31m"," [");
	    p("[37m","I");
	    p("[31m","]");
	    p("[32m","nsert");
	    p("[31m"," [");
	    p("[37m","C");
	    p("[31m","]");
	    p("[32m","ontinue");
	    p("[31m"," [");
	    p("[37m","A");
	    p("[31m","]");
	    p("[32m","bort ");
	    p("[33;41m","ณ");
	    p("[40m","\n");
	    p("[41m","ณ");
	    p("[44m","                                    ");
	    p("[41m","ณ");
	    p("[40m","\n");
	    p("[41m","ณ");
	    p("[31;44m"," [");
	    p("[37m","E");
	    p("[31m","]");
	    p("[32m","dit");
	    p("[31m"," [");
	    p("[37m","D");
	    p("[31m","]");
	    p("[32m","elete");
	    p("[31m"," [");
	    p("[37m","R");
	    p("[31m","]");
	    p("[32m","eplace");
	    p("[31m","  [");
	    p("[37m","S");
	    p("[31m","]");
	    p("[32m","ave  ");
	    p("[33;41m","ณ");
	    p("[40m","\n");
	    p("[40m","");
	    p("[41m","ณ");
	    p("[44m","                                    ");
	    p("[41m","ณ");
	    p("[40m","\n");
	    p("[41m","ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");


	    p("[36;40m","\n\n Selection: ");

	    }
	    else {


	    p("[1;36;40m","Line Editor Options\n\n");
	    p("[31m","[");
	    p("[37m","L");
	    p("[31m","]");
	    p("[32m","ist");
	    p("[31m","  [");
	    p("[37m","I");
	    p("[31m","]");
	    p("[32m","nsert");
	    p("[31m","  [");
	    p("[37m","C");
	    p("[31m","]");
	    p("[32m","ontinue");
	    p("[31m","  [");
	    p("[37m","S");
	    p("[31m","]");
	    p("[32m","ave\n");
	    p("[31m","[");
	    p("[37m","E");
	    p("[31m","]");
	    p("[32m","dit");
	    p("[31m","  [");
	    p("[37m","D");
	    p("[31m","]");
	    p("[32m","elete");
	    p("[31m","  [");
	    p("[37m","R");
	    p("[31m","]");
	    p("[32m","eplace ");
	    p("[31m","  [");
	    p("[37m","A");
	    p("[31m","]");
	    p("[32m","bort: ");



	    }




	    if (user.ansi[0] !='1') {
	       p("[37;44m"," ");
	       p("[1D","");
	    }
	    c=hot_key();

	    switch (c) {

	       case 'L':

		  more_select(ON);

		  if (l_number==0) {
		     flag=OFF;
		     p("[32;40m","\n\nThere are no lines in this message.");
		     break;
		  }


		  intro_line_editor(who_to_from->to,idx->private[0]);
		  list_message(t,l_number,u);

		  more_select(OFF);
		  break;

	       case 'D':
		  back=del_message(l_number,u);
		  if (back !=100)
		     l_number=back;
		  break;


	       case 'R':
		  back= (unsigned char) replace_line(t,l_number,u);
		  if (back) {
		     strcpy(message[loc[back-1]].line,response);
		     strcpy(message[loc[back-1]].color,r_color);
		  }
		  break;

	       case 'I':

		  if (l_number==the_bottom)
		     p("[40;31m","\n\nYou have reached the end of the message, no lines can be inserted.");
		  else {
		     back=insert_line(t,l_number,u);
		     if (back)
			l_number=back;
		  }
		  break;

	       case 'E':
		  edit_line(t,l_number,u);
		  break;

	       case 'C':
		  if (l_number==0) {
		     colr=ON;
		     cont=ON;
		     temp_c=13;
		     flag2=OFF;
		     display_m_bar();
		  }
		  else {
		     colr=ON;
		     flag2=OFF;
		     temp_c=8;
		     counter=0;
		     l_number++;
		  }
		  break;
	       case 'A':


		  back=quit_m();
		  if (back=='Y') {
		     flag2=OFF;
		     flag=OFF;
		     back=0;

		     if (save_file_desc) {
			strcpy(g_file_bbs.detailed,"N");
			save_file_desc=2;
			save_file_des(t,0,u);
			return 1;


		     }
		  }
		  break;

	       case 'S':



		  temp_line=l_number-1;
		  temp_flag=ON;

		  across=0;

		  while (temp_flag) {

		   if (message[loc[temp_line]].line[across]==' ')
		     across++;
		   else if (message[loc[temp_line]].line[across]=='\n') {
		     temp_line--;
		   }
		   else
		     temp_flag=OFF;

		  }


		     if (temp_line<0) {
			p("[40;36m","\n\nThere are no ");
			p("[33m","lines with text");
			p("[36m"," in this message.");
			break;
		     }
		     else
		       l_number=temp_line+1;


		  p("[40;33m","\n\n Saving ");

		  ltoa(l_number,response,10);
		  p("[36m",response);
		  p("[33m"," line");
		  p("[34m","(");
		  p("[33m","s");
		  p("[34m",")");
		  p("[32m","...");

		  if (save_file_desc) {
		     save_file_des(t,l_number,u);
		     return 1;
		  }


		  else {


		  mess_saved=atol(user.messages_posted);
		  mess_saved++;
		  ltoa(mess_saved,user.messages_posted,10);


		  return save_message(t,l_number,u,idx,who_to_from);
		  }

		  break;


	    }
	 }
      flag2=ON;
      }
      else if (c !=8) {
	 message[loc[l_number-1]].color[counter]=d;
	 response[counter++]=c;
	 send[0]=c;
	 p("",send);
      }

      if (counter>hor && l_number==the_bottom) {
	 p("[40;31m","\n\nYou have reached the end of the message.");
	 colr=ON;
	 temp_c=27;
      }
      else if (counter>hor  )   {
	 if (response[counter-1] !=32) {
	    back=0;
	    i=counter;
	    while (response[--counter] !=32 && counter>mid){
	       message[loc[l_number]].color[back]=d;
	       build[back++]=response[counter];

	    }
	    build[back]='\0';
	 }
	 else
	    back=how_far_back;

	    if (counter==mid)  /*40*/
	       counter=i;
	    message[loc[l_number-1]].color[counter]='\0';
	    response[counter++] = '\n';
	    response[counter] = '\0';
	    strcpy(message[loc[l_number-1]].line,response);

	    if (back != how_far_back ) {
	       for (i=(unsigned char) 73-back;i<75;i++)
		  message[loc[l_number-1]].color[i]=6;
	       counter=back;
	       delete(back);
	       d_line(++l_number,1,1);
	       back=0;
	       strrev(build);
	       strcpy(response,build);
	       message_color((int) d);
	       p("",build);
	    }
	 else if (flag) {
	    d_line(++l_number,1,1);
	    message_color((int) d);
	    counter=0;
	 }
      }
   }
   return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void d_line(unsigned char l_num,unsigned char b_bar,unsigned char warn) {

   unsigned char d_num[4],i;

     if (warn) {
	 if (l_num==97)
	    p("[40;31m","\n\nWarning: 3 lines remaining\n");
	 else if (l_num==98)
	    p("[40;31m","\n\nWarning: 2 lines remaining\n");
	 else if (l_num==99)
	    p("[40;31m","\n\nWarning: This is your last line\n");

     }


   p("[0;1;32;40m","\n");
   itoa(l_num,d_num,10);
   if (l_num<10)
      p("","0");
   p("",d_num);
   p("[31m",": ");

   if (blue_bar==ON && b_bar==ON) {

      p("[44;37m","");


      if (save_file_desc) {
      p("","                                ");
      p("[32D","");
      }

      else {


      p("","                                                                          ");
      p("[74D","");
      }



   }
   else
      p("[40;37m","");

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char get_colr(unsigned char *r) {


   unsigned char value[3];
   unsigned char flag=ON;
   unsigned char c;
   unsigned char counter=0;

   unsigned char val;

   value[0]='\0';

   while (flag) {
      c=0;
      while (c==0)
	 c=(unsigned char) inkey();

      if (isdigit(c) && counter<2)
         value[counter++] =c;

      else {
         value[counter]='\0';
         flag=OFF;
      }
   }
   val=(unsigned char) atoi(value);
   if (val>0 && val <25)
      *r=val;
   message_color(val);
   return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void message_color(int val) {

  if (blue_bar ) {
     if ( val>0 && val <7)
	p("[0;1;44m","");
     else if (val>6 && val <13) {
	p("[0;1;5;44m","");
	val=val-6;
     }
     else if (val>12 && val <19) {
	p("[0;44m","");
	val=val-12;
     }
     else if (val>18 && val <25) {
	p("[0;5;44m","");
	val=val-18;
     }
  }
  else {
     if ( val>0 && val <7)
	p("[0;1m","");
     else if (val>6 && val <13) {
	p("[0;1;5m","");
	val=val-6;
     }
     else if (val>12 && val <19) {
	p("[0m","");
	val=val-12;
     }
     else if (val>18 && val <25) {
	p("[0;5m","");
	val=val-18;
     }
  }
  switch (val) {

     case 1:
	p("[31m","");
	break;
     case 2:
	p("[32m","");
	break;
     case 3:
	p("[33m","");
	break;
     case 4:
	p("[35m","");
	break;
     case 5:
	p("[36m","");
	break;
     case 6:
	p("[37m","");
	break;
  }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void display_m_bar() {
   /*p("[0;1;36;40m","\n\n   [ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ]");*/

   if (save_file_desc)
      p("[0;1;36;40m","\n\n   [ออออþอออออออþอออออออþอออออออþอออ]");
   else
      p("[0;1;36;40m","\n\n   [ออออþอออออออþอออออออþอออออออþอออออออþอออออออþอออออออþอออออออþอออออออþอออออ]");



}


void list_message(struct mess message[],unsigned char l_number,unsigned char loc[]) {

   unsigned char i,j,counter,d,flag=ON,lngth;

   if (save_file_desc)
     lngth=32;
   else
     lngth=74;



   if (flag) {

      display_m_bar();
      for (i=0;i<(unsigned char) l_number;i++) {
	 d=6;
	 d_line(i+1,0,0);
	 message_color((int) d);
	 counter=(unsigned char) strlen(message[loc[i]].line);
	 for (j=0;j<counter;j++) {
	    if (message[loc[i]].color[j] !=d) {
	       message_color((int) message[loc[i]].color[j]);
	       d=message[loc[i]].color[j];
	    }
	    if (message[loc[i]].line[j] !='\n') {
	       send[0]=message[loc[i]].line[j];
	       p("",send);
	    }
	 }
	 if (counter <lngth && blue_bar==ON) {
	    for(j=0;j<(unsigned char) ((lngth+1)-counter);j++)
	    p(""," ");
	 }
      }


   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char del_message(unsigned char l_number,unsigned char loc[]) {

   unsigned char start,end,i;
   unsigned char sum,temp,temp2,flag=ON,flag2=OFF,flag3=ON;
   unsigned char sz[3],sz2[3];

   if (l_number==0) {
      p("[32;40m","\n\nThere are no lines in this message.");
      return 0;
   }
   itoa((int) l_number,sz,10);

   while (flag) {

      p("[33;40m","\n\nEnter staring line number to delete: ");
      if (user.ansi[0] !='1') {
	 p("[37;44m","  ");
	 p("[2D","");
      }
      get_number(2,"");
      start=(unsigned char) atoi(response);
      itoa((int) start,sz2,10);

      if (start==0 && strlen(response) !=0 || start>l_number && strlen(response) !=0) {
	 p("[32;40m","\n\nLine number must be between 1 and ");
	 p("",sz);
      }
      else if (strlen(response) !=0)
	 flag2=ON;
      else  {
	 flag3=OFF;
	 flag=OFF;
      }
      while (flag2) {
	 p("[33;40m","\n\nEnter ending line number to delete: ");
	 if (user.ansi[0] !='1') {
	    p("[37;44m","  ");
	    p("[2D","");
	 }
	 get_number(2,"");
	 end=(unsigned char) atoi(response);
	 if (end<start && strlen(response) !=0 || end>l_number && strlen(response) !=0){
	    p("[32;40m","\n\nLine number must be between ");
	    p("",sz2);
	    p(""," and ");
	    p("",sz);
	 }
	 else if (strlen(response) !=0) {
	    flag2=OFF;
	    flag=OFF;
	    flag3=ON;
	 }
	 else {
	    flag2=OFF;
	    flag=OFF;
	    flag3=OFF;
	 }
      }
   }
   if (flag3) {
      sum=(end-start)+1;
      start--;
      if (sum==1) {
	 temp=loc[start];
	 for (i= start;i<(l_number-1);i++)
	    loc[i]=loc[i+1];
	 loc[i] = temp;
      }
      else {
	 temp=loc[end];
	 for (i=0;i<(l_number-1);i++) {
	    temp2=loc[i+start];
	 loc[i+start]=temp;
	 loc[end]=temp2;
	 temp=loc[++end];
	 }
      }
      p("[31;40m","\n\nLines have been deleted.");
      return l_number-sum;
   }
   p("[32;40m","\n\nDelete line has been aborted.");

   return 100;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void get_single_line(unsigned char length,unsigned char ansi_in) {

   unsigned char flag=ON,c,counter=0;
   unsigned char *r,d=6,e;
   unsigned char colr=OFF;

   r=&d;

   while(flag) {
      c=0;
      if (colr==ON) {
	 c=e;
	 colr=OFF;
      }
      else {
	 while (c==0)
	 c=(unsigned char) inkey();
      }
      if (!is_valid(c)) {
      }
      else if (c==27) {
      }
      else if (c==9) {
      }
      else if (c==8 && counter>0)  {
	 delete(1);
	 counter--;
	 if (ansi_in) {
	    d=r_color[counter];
	    message_color(d);
	 }
      }
      else if (c==1 && ansi_in) {
	 e=get_colr(r);
	 r_color[counter]=d;
	 colr=ON;
      }
      else if (c==13) {
	 flag=OFF;
	 r_color[counter]='\0';
	 response[counter++]='\n';
	 response[counter]='\0';
      }
      else if (counter<length && c !=8) {
	 send[0]=c;
	 p("",send);
	 if (ansi_in)
	    r_color[counter]=d;
	 response[counter++]=c;
      }
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char replace_line(struct mess message[],unsigned char l_number,unsigned char loc[]) {


   unsigned char line;
   unsigned char flag=ON,flag2=ON;
   unsigned char sz[3];


   if (l_number==0) {
      p("[32;40m","\n\nThere are no lines in this message.");
      return 0;
   }

   itoa((int) l_number,sz,10);

   while (flag) {
      p("[33;40m","\n\nEnter the line number to replace: ");
      if (user.ansi[0] !='1') {
      p("[37;44m","  ");
      p("[2D","");
      }
      get_number(2,"");
      line=(unsigned char) atoi(response);


      if (line==0 && strlen(response ) !=0 || line>l_number && strlen(response) !=0) {
	 p("[32;40m","\n\nLine number must be between 1 and ");
	 p("",sz);
      }
      else if (strlen(response)==0) {
	 flag=OFF;
	 flag2=OFF;
      }
      else flag=OFF;
   }

   if (flag2) {
      p("[31;40m","\n\nOld Line:");
      list_line(message[loc[line-1]].line,message[loc[line-1]].color,line);
      p("[31;40m","\n\nNew Line:");
      display_m_bar();
      d_line(line,1,0);
      get_single_line(74,1);
      if (strlen (response) !=1) {
	 p("[31;40m","\n\nLine has been replaced.");
	 return line;
      }
      else
	 flag2=OFF;
   }
   p("[32;40m","\n\nReplace line aborted.");
   return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char insert_line(struct mess message[],unsigned char l_number,unsigned char loc[]) {


   unsigned char line;
   unsigned char sz[3];

   unsigned char j,temp,temp2;
   unsigned flag=ON;

   if (l_number==0) {
      p("[32;40m","\n\nThere are no lines in this message.");
      return 0;
   }

   itoa((int) l_number,sz,10);
   while (flag) {

      p("[33;40m","\n\nEnter the line number to insert before: ");

      if (user.ansi[0] !='1') {
	 p("[37;44m","  ");
	 p("[2D","");
      }
      get_number(2,"");

      line= (unsigned char) atoi(response);

      if (line > l_number && strlen(response) != 0 || line ==0 && strlen(response) != 0){
	 p("[32;40m","\n\nLine number must be between 1 and ");
	 p("",sz);
      }
      else
	 flag=OFF;
   }
   if (strlen(response) != 0) {


      p("[31;40m","\n\nEnter text to be inserted below ");
      p("[35m","[");
      p("[37m","Return Aborts");
      p("[35m","]");
      p("[31m",":");

      display_m_bar();
      d_line(line,1,0);
      get_single_line(73,1);
      if (strlen(response) !=1) {
	 temp=loc[l_number];
	 for(j=(unsigned char) line-1;j<(unsigned char) l_number+1;j++) {
	    temp2 = loc[j];
	    loc[j]=temp;
	    temp=temp2;
	 }
	 strcpy(message[loc[line-1]].line,response);
	 strcpy(message[loc[line-1]].color,r_color);
	 p("[31;40m","\n\nLine has been inserted.");
	 return l_number+1;
      }
   }
   p("[32;40m","\n\nInsert line aborted.");
   return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void edit_line(struct mess message[],unsigned char l_number,unsigned char loc[]) {


   unsigned char line,flag=ON,flag2=OFF,flag3=OFF,flag4=OFF,abort=OFF,ok;
   unsigned char sz[3];


   struct mess work;

   unsigned char test[82],test_c[82],a,b,c,d,flag5=ON;
   unsigned char *point,i,j,k;
   int pos;

   if (l_number==0) {
      flag5=OFF;
      p("[32;40m","\n\nThere are no lines in this message.");
   }

   itoa((int) l_number,sz,10);

   while (flag5) {
      while (flag) {
	 p("[33;40m","\n\nEnter the line number to edit: ");

	 if (user.ansi[0] !='1') {
	 p("[37;44m","  ");
	 p("[2D","");
	 }
	 get_number(2,"");
	 line=(unsigned char) atoi(response);

	 if (line <=0 && strlen(response) !=0 || line >l_number && strlen(response) !=0) {
	    p("[32;40m","\n\nLine number must be between 1 and ");
	    p("",sz);
	 }
	 else if (strlen (response) ==0){
	    flag=OFF;
	    flag2=OFF;
	    abort=ON;
	 }
	 else {
	    flag=OFF;
	    flag2=ON;
	 }
      }
      flag=ON;
      if (flag2) {
	 strcpy(test,message[loc[line-1]].line);
	 strcpy(test_c,message[loc[line-1]].color);
	 a=(unsigned char)strlen(test)-1;
	 p("[31;40m","\n\nLine to edit:");
	 list_line(test,test_c,line);
	 while (flag) {
	    p("[33;40m","\n\nEnter text to edit: ");

	    if (user.ansi[0] !='1') {
	    p("[37;44m","                                        ");
	    p("[40D","");
	    }
	    get_single_line(40,1);
	    response[strlen(response)-1] = '\0';
	    b=(unsigned char)strlen(response);
	    if (strlen(response) != 0) {
	       k=(unsigned char) strlen(response);
	       point=strstr(test,response);
	       if (point !=NULL) {
		  pos=  point-test;
		  flag=OFF;
		  flag3=ON;
	       }
	       else
		  p("[31;40m","\n\nText was not found.");
	    }
	    else {
	       flag=OFF;
	       flag3=OFF;
	       abort=ON;
	    }
	 }
      }

      flag2=ON;

      if (flag3) {
	 while (flag2) {
	    p("[33;40m","\n\nEnter replacement text: ");
	    if (user.ansi[0] !='1') {
	    p("[37;44m","                                                  ");
	    p("[50D","");
	    }
	    get_single_line(50,1);
	    response[strlen(response)-1] = '\0';
	    c=(unsigned char)strlen(response);
	    if (strlen(response) != 0) {


	       d=(a-b)+c;


	       if (d<75) {
		  for (i=0;i<(unsigned char) pos;i++) {
		     work.line[i]=test[i];
		     work.color[i]=test_c[i];
		  }
		  for (j=0;j<(unsigned char) strlen(response);j++) {
		     work.line[i]=response[j];
		     work.color[i]=r_color[j];
		     i++;
		  }
		  for (j=k+pos;j<=(unsigned char) strlen(test);j++) {
		     work.line[i]=test[j];
		     work.color[i]=test_c[j];
		     i++;
		  }
		  p("[31;40m","\n\nNew line looks like:");
		  list_line(work.line,work.color,line);
		  p("[33;40m","\n\nIs this ok [Y/n]: ");
		  p("[44m"," ");
		  p("[1D","");
		  ok=get_yn("YN");
		  if (ok=='Y') {
		     strcpy(message[loc[line-1]].line,work.line);
		     strcpy(message[loc[line-1]].color,work.color);
		     flag5=OFF;
		     flag3=OFF;
		     flag2=OFF;
		     p("[31;40m","\n\nLine has been editted.");
		  }
	       }
	       else
		  p("[31;40m","\n\nText is too long to fit on that line, shorten.");
	    }
	    else {
	       abort=ON;
	       flag3=OFF;
	       flag2=OFF;
	    }
	 }
      }

      if (abort) {
	 p("[32;40m","\n\nEdit line aborted.");
	 flag5=OFF;
      }
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char quit_m() {

   p("[32;40m","\n\nAre you sure you wish to quit [Y/n]: ");
   return get_yn("YN");
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void list_line(unsigned char test[],unsigned char test_c[],unsigned char i) {

   register unsigned char j,counter,d;

   display_m_bar();

   d=6;
   d_line(i,0,0);
   message_color((int) d);
   counter=(unsigned char) strlen(test)-1;
   for (j=0;j<counter;j++) {
      if (test_c[j] !=d) {
	 message_color((int) test_c[j]);
	 d=test_c[j];
      }
      if (test[j] !='\n') {
	 send[0]=test[j];
	 p("",send);
      }
   }

   if (counter <74 && blue_bar==ON) {
      for(j=0;j<(unsigned char) (74-counter);j++)
	 p(""," ");
   }
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void color_chart() {

   p("[40m","\n\n");
   p("[0;1;33;41m","ึฤฤด");
   p("[45m","  Colour Chart  ");
   p("[41m","รฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
   p("[40m","\n");
   p("[41m","บ");
   p("[44m","                                                                           ");
   p("[41m","บ");
   p("[40m","\n");
   p("[41m","บ");

   p("[31;44m","   Û");
   p("[36;44m"," 1   ");
   p("[32m","Û");
   p("[36m"," 2   ");
   p("[33m","Û");
   p("[36m"," 3   ");
   p("[35m","Û");
   p("[36m"," 4   ");

   p("[36m","Û");
   p("[36m"," 5   ");

   p("[37m","Û");
   p("[36m"," 6   ");

   p("[5;31m","Û");
   p("[0;1;44;36m"," 7   ");

   p("[5;32m","Û");
   p("[0;1;44;36m"," 8   ");

   p("[5;33m","Û");
   p("[0;1;44;36m"," 9   ");

   p("[5;35m","Û");
   p("[0;1;44;36m"," 10  ");

   p("[5;36m","Û");
   p("[0;1;44;36m"," 11  ");

   p("[5;37m","Û");
   p("[0;1;44;36m"," 12  ");

   p("[0;1;33;41m","บ");
   p("[40m","\n");
   p("[41m","บ");
   p("[0;44;31m","   Û");
   p("[1;36m"," 13  ");
   p("[0;32m","Û");
   p("[1;44;36m"," 14  ");
   p("[0;33m","Û");
   p("[1;44;36m"," 15  ");
   p("[0;35m","Û");
   p("[1;44;36m"," 16  ");
   p("[0;36m","Û");
   p("[1;44;36m"," 17  ");
   p("[0;37m","Û");
   p("[1;44;36m"," 18  ");
   p("[0;5;44;31m","Û");
   p("[0;1;44;36m"," 19  ");
   p("[0;5;44;32m","Û");
   p("[0;1;44;36m"," 20  ");
   p("[0;5;44;33m","Û");
   p("[0;1;44;36m"," 21  ");
   p("[0;5;44;35m","Û");
   p("[0;1;44;36m"," 22  ");
   p("[0;5;44;36m","Û");
   p("[0;1;44;36m"," 23  ");
   p("[0;5;44;37m","Û");
   p("[0;1;44;36m"," 24  ");

   p("[0;1;33;41m","บ");
   p("[40m","\n");
   p("[41m","บ");
   p("[44m","                                                                           ");
   p("[41m","บ");
   p("[40m","\n");
   p("[0;1;33;41m","ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
   p("[40m","");
   p("[33;40m","\n\nPress ");
   p("[36m","CTRL");
   p("[37m","-");
   p("[36m","A");
   p("[33m"," and then enter a ");
   p("[32m","colour ");
   p("[33m","number");
   p("[37m",".");
}


