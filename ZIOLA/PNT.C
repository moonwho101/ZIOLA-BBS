
#include <stdio.h>
#include <string.h>


main() {

   struct user_idx {
      unsigned char name[41];
      long int left;
      long int right;
   } user_tree;

   struct last_scan_date {
   unsigned char last_date[10][11];
   unsigned char last_time[10][9];
   unsigned char tag[10];
} l_scan_date;


   unsigned char reserve[81];

   char result;
   char side;
   char flag=1;

   long int count=1;
   long int record,ass;
   size_t rec_size = sizeof (struct user_idx);

   long int number;

   FILE *fp;

struct last_read {
   long int mes_pnt[50];
   long int base_pnt[50];
   unsigned char scan[50];
} lr_pnt;



   if ((fp=fopen("c:\\bz\\users\\users.idx","rb+")) == NULL) {
      printf("Cannot open users.idx\n");
      exit(1);
   }

   fseek(fp,0L,0);
   fread(&user_tree,rec_size,1,fp);
   number=user_tree.left;

   fclose(fp);

	    if ((fp=fopen("users.fil","wb+")) == NULL) {
		 printf("cannot open users.pnt\n");
		  exit(1);
	    }



   for (ass=0;ass<number;ass++) {


    for (count=0;count<10;count++) {

      if (count<4) {
      strcpy(l_scan_date.last_date[count],"10-01-1993");
      strcpy(l_scan_date.last_time[count],"12:00 pm");
      }
      else {
      strcpy(l_scan_date.last_date[count],"          ");
      strcpy(l_scan_date.last_time[count],"        ");
      }

      if (count<4)
      l_scan_date.tag[count]='Y';
      else
      l_scan_date.tag[count]='N';
     }





   fseek(fp,ass*sizeof(struct last_scan_date),0);
   fwrite (&l_scan_date, sizeof(struct last_scan_date),1,fp);



   }




   fclose(fp);

}






