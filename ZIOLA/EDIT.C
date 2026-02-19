#include <stdio.h>
#include <ctype.h>
#include <bios.h>  /*keyboard*/
#include <string.h> /*string functions*/
#include <stdlib.h> /*convert atoi etc */
#include <time.h>
#include <io.h>
#include <setjmp.h>


#define OFF 0
#define ON 1

int inkey();
void display();

struct user_info {
   unsigned char city[30];
   unsigned char voice[11];
   unsigned char data[11];
   unsigned char pword[11];
   unsigned char secur[6];
   unsigned char sex[2];
   unsigned char total_calls[6];
   unsigned char total_calls_today[6];
   unsigned char last_date_called[11];
   unsigned char log_in_time[9];
   unsigned char log_out_time[9];
   unsigned char time_used[6];
   unsigned char ansi[2];
   unsigned char screen_length[4];
   unsigned char user_comment[81];
   unsigned char doors_opened[6];
   unsigned char messages_posted[6];
   unsigned char bdate[7];
   unsigned char bps_rate[21];
   unsigned char new_files[2];
   unsigned char more[7];
   unsigned char dl_num[11];
   unsigned char dl_kb[11];
   unsigned char ul_num[11];
   unsigned char ul_kb[11];
   unsigned char dl_date[9];
   unsigned char dl_num_today[11];
   unsigned char dl_kb_today[11];
   unsigned char ul_date[9];
   unsigned char ul_num_today[11];
   unsigned char ul_kb_today[11];
   unsigned char screen_clear[2];

} user;

   struct user_idx {
      unsigned char name[41];
      long int left;
      long int right;
   } user_tree;



main() {

   unsigned char answer[80],check=OFF;
   long int number,user_rec_num;
   int c,flag=1;


   size_t rec_size = sizeof (struct user_idx);
   size_t rec_size2 = sizeof (struct user_info);

   FILE *fp,*id;




   if ((fp=fopen("c:\\bz\\users\\users.idx","rb+")) == NULL) {
      printf("Cannot open users.idx\n");
      exit(1);
   }


   fseek(fp,0L,0);
   fread(&user_tree,rec_size,1,fp);
   number=user_tree.left;





	    if ((id=fopen("c:\\bz\\users\\users.bbs","rb+")) == NULL) {
	       printf("cannot open users.bbs\n");
	       exit(1);
	    }

	    fseek(id,1L*rec_size2,0);
	    fread(&user,rec_size2,1,id);
   fseek(fp,1L*rec_size,0);
   fread(&user_tree,rec_size,1,fp);
   user_rec_num=1;
	    display();

   while  (flag) {

    c=0;
    while (c==0)
    c=(unsigned char) inkey();


   switch (c) {

 /*   strupr(c);*/

    case '+':

	user_rec_num++;
	break;

    case '-':

	user_rec_num--;
	break;


    case 'b':

	printf("\n\nEnter city: ");
	gets(answer);

	strcpy(user.city,answer);
	check=ON;


	break;

    case 'e':

	printf("\n\nEnter pword: ");
	gets(answer);

	strcpy(user.pword,answer);
	check=ON;


	break;




    case 'f':

	printf("\n\nEnter security level: ");
	gets(answer);

	strcpy(user.secur,answer);
	check=ON;


	break;
    case 'x':

	printf("\n\nEnter download bytes: ");
	gets(answer);

	strcpy(user.dl_kb,answer);

	check=ON;

	break;

    case 'z':

	printf("\n\nEnter upload bytes: ");
	gets(answer);

	strcpy(user.ul_kb,answer);

	check=ON;

	break;
    case 'w':

	printf("\n\nEnter dl # : ");
	gets(answer);

	strcpy(user.dl_num,answer);
	check=ON;

	break;


    case 'q':
       flag=0;

fclose(fp);
fclose(id);
       exit(1);
       break;

   }
    if (check) {
	    fseek(id,user_rec_num*rec_size2,0);
	    fwrite(&user,rec_size2,1,id);
    }
    check=OFF;
     if (user_rec_num<=0)
       user_rec_num=number;
     else if (user_rec_num>number)
       user_rec_num=1;

   fseek(fp,user_rec_num*rec_size,0);
   fread(&user_tree,rec_size,1,fp);


	    fseek(id,user_rec_num*rec_size2,0);
	    fread(&user,rec_size2,1,id);


	    display();




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


   printf("\n\nA] First name          :%s\n",user_tree.name);
   printf("B] City                :%s\n",user.city             );
   printf("C] Voice               :%s\n",user.voice            );
   printf("D] Date                :%s\n",user.data             );
   printf("E] Pword               :%s\n",user.pword            );
   printf("F] Secur               :%s\n",user.secur            );
/*
   printf("G] Sex                 :%s\n",user.sex              );
   printf("H] T Calls             :%s\n",user.total_calls      );
   printf("I] T Calls Today       :%s\n",user.total_calls_today);
   printf("J] Last Date called    :%s\n",user.last_date_called);
   printf("K] Log in time         :%s\n",user.log_in_time    );
   printf("L] Log out time        :%s\n",user.log_out_time   );
   printf("M] Time Used           :%s\n",user.time_used      );
   printf("N] Ansi                :%s\n",user.ansi           );
   printf("O] S length            :%s\n",user.screen_length  );
   printf("P] S comment           :%s\n",user.user_comment   );
   printf("Q] Doors               :%s\n",user.doors_opened   );
   printf("R] Messages Posted     :%s\n",user.messages_posted);


   printf("S] Bdate               :%s\n",user.bdate       );

   printf("T] Bps Rate            :%s\n",user.bps_rate    );
   printf("U] New files           :%s\n",user.new_files   );
   printf("V] More                :%s\n",user.more        );

*/
   printf("W] Dl Num              :%s\n",user.dl_num      );
   printf("X] Dl kb               :%s\n",user.dl_kb       );
   printf("Y] Ul num              :%s\n",user.ul_num      );
   printf("Z] Ul Kb               :%s\n",user.ul_kb       );
   printf("1] Dl date             :%s\n",user.dl_date     );
   printf("2] Dl Num Today        :%s\n",user.dl_num_today);

   /*
   printf("3] Dl Kb today         :%s\n",user.dl_kb_today );
   printf("4] Ul Date             :%s\n",user.ul_date     );
   printf("5] Ul Num Today        :%s\n",user.ul_num_today);
   printf("6] Ul Kb today         :%s\n",user.ul_kb_today);
   */
   printf("7] Screen Clear        :%s\n",user.screen_clear);






}
