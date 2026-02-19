#include "dos.h"


main()
{

   union scan {
      int c;
      char ch[2];
   } sc;

   printf("Press `q' to quit\n\n");

   do {
      sc.c = get_key();
      if(sc.ch[0]==0)
	 printf("Special Key %d\n",sc.ch[1]);
      else
	 printf("Ordinary Key %d\n",sc.ch[0]);
   } while(sc.ch[0] !='q');
}


get_key()
{
   union REGS r;
   r.h.ah = 0;
   return int86(0x16,&r,&r);
}

