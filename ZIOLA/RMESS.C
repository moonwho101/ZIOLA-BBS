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


void display_message(long int mes_num,unsigned char where_am_i[]) {


   unsigned char f_message[MSIZE],work[50];
   unsigned char target[81],sz,reply[30];


   size_t rec_size_bbs = sizeof (f_message);
   size_t rec_size_idx = sizeof (struct index);
   size_t rec_size_who = sizeof (struct who);



   FILE *fp,*id,*wh;

   long int num,i,m_bnd,area;


   show_it=ON;

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


   fseek(id,0L*rec_size_idx,0);
   fread(&g_idx,rec_size_idx,1,id);
   num=g_idx.parent;



   fseek(id,mes_num*rec_size_idx,0);
   fread(&g_idx,rec_size_idx,1,id);

   fseek(wh,mes_num*rec_size_who,0);
   fread(&g_who_tf,rec_size_who,1,wh);


     strcpy(work,first);
     strcat(work," ");
     strcat(work,last);



     if (g_idx.private[0] == 'Y') {


	if (atol(user.secur)==100L);



	else if (!strcmp(work,g_who_tf.to) || !strcmp(work,g_who_tf.from)) {
	}
	else
	   show_it=OFF;
     }
     if (g_idx.delete[0]=='Y')
	   show_it=OFF;



   if (quick_scan && show_it) {


     if(quick_scan==1) {
			more_select(ON);
			p("[0;1;33;44m","\n\nึฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤท");
			p("[40m","\n");
			p("[44m","บ");
			p("[41;33m",     " Msg#  To                 From               Rcvd Subject                  ");
			p("[33;44m","บ");
			p("[40m","\n");
			p("[0;1;33;44m","ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฝ");
			p("[40m","\n");




       quick_scan=2;
     }



     q_scan(mes_num);
   fclose(id);
   fclose(fp);
   fclose(wh);

     return;
   }
   else if (header_scan==1) {
      p("","\n\n");
      more_select(ON);
      header_scan=2;
   }


   if (show_it) {
   p("[0m","");



   if (!header_scan)
   send_clear_screen();
   /*display_m_area();*/


   if (user.ansi[0]=='3') {
   p("","  ");
   p("[0;1;30;47m","ีอต");






   p("[1;40;33m"," Message Area #");

   p("[36m",menu[do_function].command);

   area=atol(menu[do_function].command);



   p("[37m",", ");

   sz=18;

   sz=sz+strlen(mess_name[area-1].name);

   p("[32m",mess_name[area-1].name);


   p(""," ");



   p("[0;30;47m","ฦ");

   target[0]='\0';
   strncat(target,line,51-sz);

   p("",target);
   p("","ธ");

   p("[40m","\n  ");

   p("[47m","ณ");


   p("[44m","                                                       ");

   p("[30;47m","ณ");

   p("[40m","\n  ");



   p("[47m","ณ");



   p("[1;44;32m","  Msg # : ");




   ltoa(mes_num,reserve,10);

   sz=strlen(reserve);


   p("[37m",reserve);
   p("[36m"," of ");

   sz=sz+4;


   m_bnd=m_bound(0L);
   ltoa (m_bnd,reserve,10);

   sz=sz+strlen(reserve);


   p("[37m",reserve);

   reserve[0]='\0';


   strncat(reserve,blnk,25-sz);

   p("",reserve);




   p("[32m","Private : ");


   if (g_idx.private[0]=='Y')
      strcpy(reserve,"Yes");
   else
      strcpy(reserve,"No");

   format(reserve,10);
   p("[37m",response);


   p("[0;30;47m","ณ");

   p("[40m","\n  ");























   p("[47m","ณ");



   p("[1;44;32m","  To    : ");


   format(g_who_tf.to,25);



   p("[37m",response);


   p("[32m","Replies : ");

   itoa((int) g_idx.reply,reply,10);


   format(reply,10);
   p("[37m",response);


   p("[0;30;47m","ณ");

   p("[40m","\n  ");
   p("[47m","ณ");



   p("[1;44;32m","  From  : ");

   format(g_who_tf.from,25);
   p("[37m",response);


   p("[32m","Received: ");

   if (g_who_tf.received[0]=='Y')
      strcpy(reserve,"Yes");
   else
      strcpy(reserve,"No");


   format(reserve,10);
   p("[37m",response);

   p("[0;30;47m","ณ");

   p("[40m","\n  ");
   p("[47m","ณ");


   p("[1;44;32m","  Date  : ");

   format(g_idx.date,25);
   p("[37m",response);

   p("[32m","Time    : ");
   format(g_idx.time,10);

   p("[37m",response);

   p("[0;30;47m","ณ");

   p("[40m","\n  ");


   p("[47m","ณ");


   p("[1;44;32m","  Status: ");


   if (!strlen(where_am_i))
      format("None",45);
   else
     format(where_am_i,45);


     p("[33m",response);


   p("[0;30;47m","ณ");

   p("[40m","\n  ");



   p("[47m","ณ");


   p("[44m","                                                       ");


   p("[30;47m","ณ");


   p("[40m","\n  ");


   p("[47m","ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออพ");



   }

   else {

   p("[0;1;36m","Message # : ");

   itoa ((int) mes_num,target,10);

   m_bnd=m_bound(mes_num);
   itoa ((int) m_bnd,work,10);


   p("[33m",target);
   p("[35m"," of ");
   p("[33m",work);


   sz=(unsigned char)28 - ((unsigned char) strlen(target) + (unsigned char)strlen(work)+ (unsigned char) 4);

   strcpy(target,"");
   strncat(target,blnk,sz);

   p("",target);





   p("[36m","Area    : ");


   p("[31m","#");

   p("[36m",menu[do_function].command);

   area=atol(menu[do_function].command);

   p("[37m",", ");

   p("[32m",mess_name[area-1].name);

   p("","\n");

   p("[36m","To        : ");



   format(g_who_tf.to,28);
   p("[33m",response);

   p("[36m","Replies : ");

   itoa((int) g_idx.reply,reply,10);
   p("[33m",reply);

   p("","\n");
   p("[36m","From      : ");

   format(g_who_tf.from,28);
   p("[33m",response);


   p("[36m","Received: ");

   if (g_who_tf.received[0]=='Y')
       p("[33m","Yes");
   else
       p("[33m","No");

   p("","\n");
   p("[36m","Date      : ");

   format(g_idx.date,28);
   p("[33m",response);

   p("[36m","Time    : ");
   p("[33m",g_idx.time);
   p("[36m","\nStatus    : ");

   if (!strlen(where_am_i))
      strcpy(reserve,"None");
   else
      strcpy(reserve,where_am_i);



   format(reserve,28);
   p("[31m",response);


   p("[36m","Private : ");

   if (g_idx.private[0]=='Y')
       p("[33m","Yes");
   else
       p("[33m","No");






   }

   p("[0;1m","\n\n");

   p("[33m","  Subject: ");
   p("[36m",g_idx.subject);



   fseek(fp,g_idx.start*rec_size_bbs,0);


   p("[0;1;37;40m","\n\n");


  if (!header_scan) {
   more_select(ON);


   if (user.ansi[0]=='3')
   more=13;
   else
   more=9;




   for (i=0;i<g_idx.end;i++) {

     if (more_r>=ON) {
     fread(f_message,rec_size_bbs,1,fp);
     p("",f_message);

     }




   }



   more_select(OFF);
   }
     strcpy(work,first);
     strcat(work," ");
     strcat(work,last);


     if (g_who_tf.received[0]=='N' && strcmp(work,g_who_tf.to)==NULL) {
	 g_who_tf.received[0]='Y';

      fseek(wh,mes_num*rec_size_who,0);
      fwrite(&g_who_tf,rec_size_who,1,wh);


     }


   p("[0;1m","\n");
   }

   else if (!show_it && !quick_scan && ! header_scan) {

    p("[40;36m","\n\nSkipping ");
    p("[33m","PRIVATE ");
    p("[36m","or ");
    p("[33m","DELETED ");
    p("[36m","message.");
	      if (!new_mail_read)
    lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=mes_num+1;
   }





   fclose(id);
   fclose(fp);
   fclose(wh);

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/


unsigned char message_prompt(unsigned char trace,unsigned char delete,unsigned char reply,unsigned char select) {

   unsigned char c,length,num=0,target[20],i,flag=OFF;
   unsigned char result [20];

   for (i=0;i<20;i++)
      result[i]=0;

   if (trace) {
      flag=ON;
      p("[0;1;32m","[");
      p("[31m","T");
      p("[32m","]race   ");

      if (7<normal[num]) {
	 target[0]='\0';
	 strncat(target,blnk,normal[num]-7);
	 p("",target);
      }
      else
	result[num]=(unsigned char) 7-normal[num];
      num++;
   }

   if (delete) {
      flag=ON;
      p("[32m","[");
      p("[31m","D");
      p("[32m","]elete   ");

      if (8<normal[num]) {
	 strcpy(target,"");
	 strncat(target,blnk,normal[num]-8);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)8-normal[num];

      num++;
   }

   if (reply) {
      flag=ON;
      p("[32m","[");
      p("[31m","V");
      p("[32m","]iew   ");

      if (6<normal[num]) {
	 target[0]='\0';
	 strncat(target,blnk,normal[num]-6);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)6-normal[num];

      num++;


      if (select) {

      p("[32m","[");
      p("[31m","S");
      p("[32m","]elect   ");

      if (8<normal[num]) {
	 target[0]='\0';
	 strncat(target,blnk,normal[num]-8);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)8-normal[num];

      num++;
      }

      p("[32m","[");
      p("[31m","L");
      p("[32m","]ist   ");

      if (6<normal[num]) {
	 target[0]='\0';
	 strncat(target,blnk,normal[num]-6);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)6-normal[num];

      num++;
   }

   p("[0;1;36m","");

   if (flag)
   p("","\n");
   p("","[");

   p("[33m","H");
   p("[36m","]elp   ");

   if (result[0]>0) {
      target[0]='\0';
      strncat(target,blnk,result[0]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","R");
   p("[36m","]eply   ");

   if (result[1]>0) {
      target[0]='\0';
      strncat(target,blnk,result[1]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","N");
   p("[36m","]ext   ");

   if (result[2]>0) {
      target[0]='\0';
      strncat(target,blnk,result[2]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","A");
   p("[36m","]gain   ");

   if (result[3]>0) {
      target[0]='\0';
      strncat(target,blnk,result[3]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","J");
   p("[36m","]ump   ");

   if (result[4]>0) {
      target[0]='\0';
      strncat(target,blnk,result[4]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","Q");
   p("[36m","]uit: ");

   if (user.ansi[0] !='1') {
      p("[37;44m"," ");
      p("[1D","");
   }


   c=hot_key();
   return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void read_message(long int mes_num,unsigned char direction) {

   struct index idx;
   struct who who_tf;

   size_t rec_size_idx = sizeof (struct index);

   unsigned char flag=ON,c,r_flag,d,select=OFF,select_r,scratch;
   unsigned char target[81],trace,view,forward,back,trace_on=OFF;

   unsigned char where_am_i[40],dsply=ON,work[50],delete;

   long int reply_stack[100],trace_stack[100],tmp;
   long int main_trace_stack[100],main_trace_counter=0,reply_mes,t_counter,r_counter;
   long int result,save_last_reply,r_mes_num,t_mes_num,m_bnd;

   FILE *id;


   strcpy(where_am_i,"");



   while (flag) {




     if (!quick_scan && ! header_scan) {
     more_r=ON;
     more=0;
     }



      if (!more_r && more_of) {
        stop_all_read=ON;
	more_select(ON);
	break;
      }



     if (dsply)
       display_message(mes_num,where_am_i);
     else
       dsply=ON;


     if (quick_scan || header_scan)
       show_it=OFF;


     if (show_it) {

     forward=OFF;
     back=OFF;
     view=OFF;
     trace=OFF;
     select_r=OFF;
     delete=OFF;

     strcpy(options,"HRAJQ");

     if (g_idx.parent) {
	strcat(options,"T");
	trace=ON;
     }

     if (g_idx.reply && trace_on==OFF) {
	strcat(options,"VL");
	view=ON;

	if (g_idx.reply>1 ) {
	   strcat(options,"S");
	   select_r=ON;
	}
     }

     if (trace_on) {
	strcat(options,"O");
	strcat(options,"B");
	if (g_idx.reply) {
	   strcat(options,"L");
	   view=2;
	}

     }
     else {
	 strcat(options,"N");

	 strcpy(response,first);
	 strcat(response," ");
	 strcat(response,last);

	 if (strcmp(g_who_tf.to,response) == NULL || strcmp(g_who_tf.from,response) == NULL || strcmp(response,"Mark Longo")==NULL) {
	 delete=ON;
	 strcat(options,"D");
	 }
     }
     if (!select) {
	if (trace_on)
	   c=reply_prompt(trace,delete,view,OFF,OFF,select_r);
	else
	   c=message_prompt(trace,delete,view,select_r);
     }

     switch (c) {

	case 'D':
	    delete_message(mes_num);
	    if (new_mail_read)
	       return;

	    break;


	case 'B':
	   main_trace_counter--;
	   if (main_trace_counter>0)
	      mes_num = main_trace_stack[main_trace_counter];
	   else {
	      trace_on=OFF;
	      mes_num=t_mes_num;
	      where_am_i[0]='\0';
	   }
	   break;

	case 13:
	case 'N':

	   if (!trace_on) {

	   if (new_mail_read)
	      return;

	   if (direction=='F' || direction=='N') {
	      mes_num++;

	      if (!new_mail_read)
	      lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=mes_num+1;
	   }
	   else if (direction=='R')
	      mes_num--;



	   m_bnd=m_bound(mes_num);
	   if (!m_bnd || mes_num==0) {
	     more_select(OFF);
	     p("[0;1;31;40m","\n\n");
	     p("[32m","[");
	     p("[33m","End of file");
	     p("[32m","]");
	      lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=mes_num;
	     flag=OFF;
             stop_all_read=ON;
	   }

	   }

	   break;

	case 'Q':
	   if (new_mail_read)
	      new_mail_read=2;
	   flag=OFF;
	   stop_all_read=ON;
			      if (!new_mail_read)
		 lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=mes_num+1;
	   break;


	case 'J':

	   scratch = ON;
	   while (scratch) {
	      tmp=jump("Enter message to jump to:","");
	      if (!tmp){
		 dsply=OFF;
		 scratch=OFF;
		 p("[31;40m","\n\nJump has been aborted.\n\n");
	      }
	      else {
		 m_bnd=m_bound(tmp);
		 if (!m_bnd) {
		    p("[31;40m","\n\nThat is not a valid message number.");
		 }
		 else {
		    mes_num=tmp;
		    scratch=OFF;
	      if (!new_mail_read)
      lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=mes_num+1;
		 }
	      }
	   }
	   break;

	case 'O':
	   where_am_i[0]='\0';
	   trace_on=OFF;
	   mes_num=t_mes_num;
	   where_am_i[0]='\0';
	   break;

	case 'S':
	   tmp = select_reply(mes_num);

	   if (tmp) {
	      c='V';
	      select=ON;
	   }
	   else
	      p("[31;40m","\n\nThat is not a reply to this message.\n\n");
	   dsply=OFF;
	   break;

	case 'L':
	   list_replies(g_idx.child);
	   dsply=OFF;
	   break;

	case 'T':


	   if (!trace_on) {
	      t_mes_num = mes_num;
	      main_trace_counter=0;
	      strcpy(where_am_i,"Tracing Message #");
	      ltoa(mes_num,target,10);
	      strcat(where_am_i,target);
	   }
	   trace_on=ON;
	   main_trace_stack [main_trace_counter] = mes_num;
	   main_trace_counter++;
	   mes_num = g_idx.parent;
	   break;

	case 'R':
	   more_select(OFF);
	   intro_line_editor(g_who_tf.from,g_idx.private[0]);
	   save_reply(mes_num);
	   more_select(ON);
	   break;

	case 'V':
	   strcpy(where_am_i,"Replies to Message #");
	   ltoa(mes_num,target,10);
	   strcat(where_am_i,target);
	   reply_mes=mes_num;
	   t_counter=0;
	   r_counter=0;
	   r_flag=ON;
	   if (select) {
	      r_mes_num = tmp;
	      select=OFF;
	   }
	   else
	      r_mes_num = g_idx.child;

	   while (r_flag) {
	      more_r=ON;
	      more=0;
	      if (dsply)
		 display_message(r_mes_num,where_am_i);
	      else
		 dsply=ON;

	      strcpy(options,"HROAQBJ");
	      forward=OFF;
	      back=OFF;
	      view=OFF;
	      trace=OFF;
	      select_r=OFF;
	      delete=OFF;

	      if (g_idx.parent) {
		 strcat(options,"T");
		 trace=ON;
	      }

	      if (g_idx.reply && trace_on==OFF) {
		 strcat(options,"VL");
		 view=ON;
		 if (g_idx.reply>1) {
		    strcat(options,"S");
		    select_r=ON;
		 }
	      }

	      if (g_idx.reply && trace_on==ON) {
		 strcat(options,"L");
		 view=2;
	      }

	      if (g_idx.sib && trace_on==OFF) {
		 strcat(options,"+");
		 forward=ON;
	      }

	      if (g_idx.prev_sib && trace_on==OFF) {
		 strcat(options,"-");
		 back=ON;
	      }

	 strcpy(response,first);
	 strcat(response," ");
	 strcat(response,last);

	 if (strcmp(g_who_tf.to,response) == NULL || strcmp(g_who_tf.from,response) == NULL || strcmp(response,"Mark Longo")==NULL) {
	 delete=ON;
	 strcat(options,"D");
	 }

	      if (!select)
		 d=reply_prompt(trace,delete,view,forward,back,select_r);

	      switch (d) {


		  case 'D':

		     if (trace_on)
			delete_message(t_mes_num);
		     else
			delete_message(r_mes_num);

		     p("[40m","\n");
		     press_return();

		     r_flag=OFF;

		     break;

		  case 'R':
		     save_reply(r_mes_num);
		     break;

		  case 'O':
		     if (trace_on) {
			trace_on=OFF;
			r_mes_num=t_mes_num;
			strcpy(where_am_i,"Replies to Message #");
			ltoa(reply_mes,target,10);
			strcat(where_am_i,target);
		     }
		     else
			r_flag=OFF;
		     break;

		  case 'B':
		     if (trace_on) {
			t_counter--;
			if (t_counter>0)
			   r_mes_num = trace_stack[t_counter];
			else{
			   trace_on=OFF;
			   r_mes_num=t_mes_num;
			   strcpy(where_am_i,"Replies to Message #");
			   ltoa(reply_mes,target,10);
			   strcat(where_am_i,target);
			}
		     }
		     else {
			r_counter--;
			if (r_counter>=0) {
			   strcpy(where_am_i,"Replies to Message #");
			   if (r_counter) {
			      ltoa(reply_stack[r_counter-1],target,10);
			      strcat(where_am_i,target);
			   }
			   else {
			      ltoa(mes_num,target,10);
			      strcat(where_am_i,target);
			   }
			   r_mes_num = reply_stack[r_counter];
			}
			else
			   r_flag=OFF;

		     }
		     break;


		  case 'V':
		     strcpy(where_am_i,"Replies to Message #");
		     ltoa(r_mes_num,target,10);
		     strcat(where_am_i,target);
		     reply_mes=r_mes_num;
		     reply_stack[r_counter++] = r_mes_num;

		     if (select) {
			r_mes_num = tmp;
			select=OFF;
		     }
		     else
			r_mes_num = g_idx.child;
		     break;


		     case 'S':
			tmp = select_reply(r_mes_num);
			if (tmp) {
			   select=ON;
			   d='V';
			}
			else
			   p("[31;40m","\n\nThat is not a reply to this message.\n\n");

			dsply=OFF;
			break;

		     case '+':
			r_mes_num = g_idx.sib;
			break;

		     case '-':
			r_mes_num = g_idx.prev_sib;
			break;

		     case 'T':
			if (!trace_on) {
			   t_mes_num = r_mes_num;
			   strcpy(where_am_i,"Tracing Message #");
			   ltoa(r_mes_num,target,10);
			   strcat(where_am_i,target);
			}
			trace_on=ON;
			trace_stack [t_counter++] = r_mes_num;
			r_mes_num = g_idx.parent;
			break;

		     case 'Q':
			r_flag=OFF;
			flag=OFF;
			break;

		     case 'J':

	   scratch = ON;
	   while (scratch) {
	      tmp=jump("Enter message to jump to:","");
	      if (!tmp){
		 dsply=OFF;
		 scratch=OFF;
		 p("[31;40m","\n\nJump has been aborted.\n\n");
	      }
	      else {
		 m_bnd=m_bound(tmp);
		 if (!m_bnd) {
		    p("[31;40m","\n\nThat is not a valid message number.");
		 }
		 else {
		    mes_num=tmp;
		    r_flag=OFF;
		    scratch=OFF;
		 }
	      }
	   }
	   break;






		     case 'L':
			list_replies(g_idx.child);
			dsply=OFF;
			break;

	       }
	    }
	 where_am_i[0]='\0';
	 break;
      }

   }
   else {



     if (direction == 'F')
     mes_num++;
     else
     mes_num--;

       m_bnd=m_bound(mes_num);
       if (!m_bnd || mes_num==0) {

		     p("[0;1;31;40m","\n\n");
	     p("[32m","[");
	     p("[33m","End of file");
	     p("[32m","]");

	      lr_pnt.base_pnt[atol(menu[do_function].command)-1L]=mes_num;
	     stop_all_read=ON;
	     flag=OFF;
       }


   }

  }

}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

unsigned char reply_prompt(unsigned char trace,unsigned char delete,unsigned char reply,unsigned char plus, unsigned char minus,unsigned char select_r) {


   unsigned char c,length,num=0,target[20],i,flag=OFF;
   unsigned char result [20];

   for (i=0;i<20;i++)
      result[i]=0;

   if (trace) {
      flag=ON;
      p("[0;1;32m","[");
      p("[31m","T");
      p("[32m","]race   ");

      if (7<reply_s[num]) {
	 target[0]='\0';
	 strncat(target,blnk,reply_s[num]-7);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)7-reply_s[num];

      num++;
   }


   if (delete) {
      flag=ON;
      p("[32m","[");
      p("[31m","D");
      p("[32m","]elete   ");

      if (8<reply_s[num]) {
	 strcpy(target,"");
	 strncat(target,blnk,reply_s[num]-8);
	 p("",target);
      }

      else
	 result[num]=(unsigned char)8-reply_s[num];
      num++;
   }

   if (reply) {
      flag=ON;

      if (reply==ON) {
      p("[32m","[");
      p("[31m","V");
      p("[32m","]iew   ");

      if (6<reply_s[num]) {
	 target[0]='\0';
	 strncat(target,blnk,reply_s[num]-6);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)6-reply_s[num];
      num++;
   }

   if (reply==ON && select_r) {
      p("[32m","[");
      p("[31m","S");
      p("[32m","]elect   ");

      if (8<reply_s[num]) {
	 target[0]='\0';
	 strncat(target,blnk,reply_s[num]-8);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)8-reply_s[num];
      num++;
   }

   if (reply>=ON) {
      p("[32m","[");
      p("[31m","L");
      p("[32m","]ist   ");


      if (6<reply_s[num]) {
	 target[0]='\0';
	 strncat(target,blnk,reply_s[num]-6);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)6-reply_s[num];
      num++;
      }
   }

   if (plus) {
      flag=ON;
      p("[32m","[");
      p("[31m","+");
      p("[32m","] Next   ");


      if (8<reply_s[num]) {
	 strcpy(target,"");
	 strncat(target,blnk,reply_s[num]-8);
	 p("",target);
      }
      else
	result[num]=(unsigned char)8-reply_s[num];
      num++;
   }

   if (minus) {
      flag=ON;
      p("[32m","[");
      p("[31m","-");
      p("[32m","] Prev   ");

      if (8<reply_s[num]) {
	 strcpy(target,"");
	 strncat(target,blnk,reply_s[num]-8);
	 p("",target);
      }
      else
	 result[num]=(unsigned char)8-reply_s[num];
      num++;
   }


   p("[0;1;36m","");

   if (flag)
      p("","\n");

   p("","[");

   p("[33m","H");
   p("[36m","]elp   ");

   if (result[0]>0) {
      target[0]='\0';
      strncat(target,blnk,result[0]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","R");
   p("[36m","]eply   ");

   if (result[1]>0) {
      target[0]='\0';
      strncat(target,blnk,result[1]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","O");
   p("[36m","]rigin   ");

   if (result[2]>0) {
      target[0]='\0';
      strncat(target,blnk,result[2]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","B");
   p("[36m","]ack   ");

   if (result[3]>0) {
      target[0]='\0';
      strncat(target,blnk,result[3]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","A");
   p("[36m","]gain   ");

   if (result[4]>0) {
      target[0]='\0';
      strncat(target,blnk,result[4]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","J");
   p("[36m","]ump   ");

   if (result[5]>0) {
      target[0]='\0';
      strncat(target,blnk,result[5]);
      p("",target);
   }

   p("[36m","[");
   p("[33m","Q");
   p("[36m","]uit: ");

   if (user.ansi[0] !='1') {
      p("[37;44m"," ");
      p("[1D","");
   }



   c=hot_key();
   return c;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void list_replies(long int mes_num) {

   FILE *id,*wh;

   struct index idx;
   struct who who_tf;

   unsigned char target[40],flag=ON;

   size_t rec_size_idx = sizeof (struct index);
   size_t rec_size_who = sizeof (struct who);

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

   p("[0m","\n\n");

   p("[0;1;37;44m","ีอออออออัออออออออออออออออออออออออัออออออออออออออออออออออออัอออออออออออัอออออออธ");
   p("[40m","\n");
   p("[44m","ณ");

   p("[33;44m","Msg #  ");
   p("[37m","ณ");
   p("[33m","To                      ");
   p("[37m","ณ");
   p("[33m","From                    ");
   p("[37m","ณ");
   p("[33m","Date       ");
   p("[37m","ณ");
   p("[33m","Replies");
   p("[37m","ณ");
   p("[40m","\n");
   p("[37;44m","ิอออออออฯออออออออออออออออออออออออฯออออออออออออออออออออออออฯอออออออออออฯอออออออพ");
   p("[40m","\n\n");

   while (flag) {
      fseek(id,mes_num*rec_size_idx,0);
      fread(&idx,rec_size_idx,1,id);
      fseek(wh,mes_num*rec_size_who,0);
      fread(&who_tf,rec_size_who,1,wh);

      ltoa(mes_num,target,10);
      strncat(target,blnk,8-strlen(target));
      p(""," ");
      p("[37m",target);

      strcpy(target,who_tf.to);
      strncat(target,blnk,25-strlen(target));
      p("[32m",target);

      strcpy(target,who_tf.from);
      strncat(target,blnk,25-strlen(target));

      p("[33m",target);

      p("[36m",idx.date);
      p("","     ");

      ltoa(idx.reply,target,10);

      p("[31m",target);
      p("","\n");

      mes_num = idx.sib;

      if (!idx.sib)
	 flag=OFF;
   }

   p("","\n");

   fclose(id);
   fclose(wh);
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

void save_reply(long int mes_num) {

   struct who who_tf;
   struct index idx;

   size_t rec_size_idx = sizeof (struct index);

   unsigned char target[81];
   long int result,save_last_reply;

   FILE *id;

   g_date();
   strcpy(target,first);
   strcat(target," ");
   strcat(target,last);
   strcpy(who_tf.to,g_who_tf.from);
   strcpy(who_tf.from,target);

   strcpy(who_tf.received,"N");
   strcpy(idx.private,g_idx.private);
   strcpy(idx.date,s_date);
   strcpy(idx.time,s_time);
   strcpy(idx.subject,g_idx.subject);
   idx.parent=mes_num;

   if (g_idx.last_reply)
      idx.prev_sib = g_idx.last_reply;
   else
      idx.prev_sib = 0;

   result=line_editor(&idx,&who_tf);

   if (result) {

      file_name("idx",0L);

      if ((id=fopen(reserve,"rb+")) == NULL)  {
	 printf("can't open");
	 exit(1);
      }
      fseek(id,mes_num*rec_size_idx,0);
      fread(&idx,rec_size_idx,1,id);

      if (!idx.child)
	 idx.child=result;

      idx.reply++;
      save_last_reply=idx.last_reply;
      idx.last_reply=result;
      fseek(id,mes_num*rec_size_idx,0);
      fwrite (&idx, rec_size_idx,1,id);

      if (save_last_reply) {
	 fseek(id,save_last_reply*rec_size_idx,0);
	 fread(&idx,rec_size_idx,1,id);
	 idx.sib = result;
	 fseek(id,save_last_reply*rec_size_idx,0);
	 fwrite (&idx, rec_size_idx,1,id);
      }
      fclose(id);
   }

   file_name("loc",0L);
  file_unlock(reserve);


}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

long int select_reply(long int mes_num) {


   struct who who_tf;
   struct index idx;

   long int tmp;
   size_t rec_size_idx = sizeof (struct index);

   FILE *id;

   p("[33;40m","\n\nEnter reply to Jump to: ");

   if (user.ansi[0] !='1') {
      p("[37;44m","     ");
      p("[5D","");
   }

   get_number(5,"");
   tmp = atol(response);

   file_name("idx",0L);

   if ((id=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }

   fseek(id,tmp*rec_size_idx,0);
   fread(&idx,rec_size_idx,1,id);

   fclose(id);
   if (idx.parent == mes_num)
      return tmp;
   else
      return 0;
}

/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/

long int jump(unsigned char msg[],unsigned char start[]) {

   long int mes_num;
   unsigned char finish[20];

   mes_num=m_bound(1L);
   ltoa(mes_num,finish,10);

   p("[33;40m","\n\n");
   p("",msg);
   p("[31m"," [");
   p("[36m","1-");
   p("",finish);
   p("[31m","]");
   p("[33m",": ");

   if (user.ansi[0] !='1') {
      p("[37;44m","     ");
      p("[5D","");
   }

   get_number(5,start);

   if (! strlen(response))
    return 0;

   mes_num= atol(response);

   return mes_num;
}


void delete_message(long int mes_num) {

   size_t rec_size_idx = sizeof (struct index);

   struct index tmp;

   FILE *id;

   long int num;

   file_name("idx",0L);

   if ((id=fopen(reserve,"rb+")) == NULL)  {
      printf("can't open");
      exit(1);
   }


   fseek(id,mes_num*rec_size_idx,0);
   fread(&g_idx,rec_size_idx,1,id);

   g_idx.delete[0]='Y';

   fseek(id,mes_num*rec_size_idx,0);
   fwrite(&g_idx,rec_size_idx,1,id);



   if (g_idx.parent) {

	    fseek(id,g_idx.parent*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);


	    tmp.reply--;


	    fseek(id,g_idx.parent*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);



   }


   /*No child, has parent*/




   if (g_idx.child) {

       num=g_idx.child;

       while (num) {

	    fseek(id,num*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    fseek(id,num*rec_size_idx,0);

	    num=tmp.sib;

	    tmp.sib=0L;
	    tmp.prev_sib=0L;
	    tmp.parent=0L;

	    fwrite(&tmp,rec_size_idx,1,id);
       }
   }

   /* has parent, no previous */


   if (g_idx.parent && !g_idx.prev_sib && g_idx.sib) {


	    fseek(id,g_idx.sib*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    tmp.prev_sib=g_idx.prev_sib;

	    fseek(id,g_idx.sib*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);


	    fseek(id,g_idx.parent*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    tmp.child=g_idx.sib;

	    fseek(id,g_idx.parent*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);






   }


   /* has prev_sib, has sib */


   if (g_idx.prev_sib && g_idx.sib && g_idx.parent) {


	    fseek(id,g_idx.prev_sib*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    tmp.sib=g_idx.sib;

	    fseek(id,g_idx.prev_sib*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);

	    fseek(id,g_idx.sib*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    tmp.prev_sib=g_idx.prev_sib;


	    fseek(id,g_idx.sib*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);


   }

  /* no sib, has prev_sib */


  if (!g_idx.sib && g_idx.prev_sib && g_idx.parent) {

	    fseek(id,g_idx.prev_sib*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    tmp.sib=g_idx.sib;

	    fseek(id,g_idx.prev_sib*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);



  }


  /* has parent, no sib prev_sib child*/


  if ( g_idx.parent && !g_idx.sib && !g_idx.prev_sib && !g_idx.child) {


	    fseek(id,g_idx.parent*rec_size_idx,0);
	    fread(&tmp,rec_size_idx,1,id);

	    tmp.child=0L;

	    fseek(id,g_idx.parent*rec_size_idx,0);
	    fwrite(&tmp,rec_size_idx,1,id);


  }


   fclose(id);
}


void q_scan(long int mes_num) {

   ltoa(mes_num,reserve,10);

   format(reserve,6);
   p("[33m","  ");

   p("",response);




   format(g_who_tf.to,19);

   p("[36m",response);

   format(g_who_tf.from,19);

   p("[35m",response);

         if (g_who_tf.received[0]=='Y')
      strcpy(reserve,"Yes");
   else
      strcpy(reserve,"No");


   format(reserve,5);
   p("[32m",response);


   strcpy(reserve,g_idx.subject);

   reserve[strlen(reserve)-1] = '\0';

   format(reserve,25);

   p("[33m",response);


   p("[32m","\n");

}

