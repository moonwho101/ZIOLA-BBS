#include <stdio.h>
#include <string.h>

#include "file.h"


void srt(struct files *item,int count);

main() {

struct files file_q[60];

strcpy(file_q[0].name,"MARK");
strcpy(file_q[1].name,"KELLY");


srt(file_q,2);



}


void srt(struct files *item,int count) {

  int a,b;
  unsigned char t;

  for (a=1;a<count;++a)
     for (b=count-1;b>=a;--b) {
	if ( strcmp(item[b-1].name , item[b].name)) {
	   t=file_index[b-1];
	   file_index[b-1]=file_index[b];
	   file_index[b]=t;
	}
     }
}

