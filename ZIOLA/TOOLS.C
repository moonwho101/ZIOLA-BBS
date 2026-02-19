#include <stdio.h>
#include <dos.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/

void goto_xy(int x, int y) {

	union REGS r;
	r.h.ah = 2;
	r.h.dl = (unsigned char)y;
	r.h.dh = (unsigned char)x;
	r.h.bh = 0;

	int86(0x10, &r, &r);
}

void cls(startx, starty, endx, endy)

    unsigned char startx,
    starty, endx, endy;

{

	union REGS r;

	r.h.ah = 6;
	r.h.al = 0;
	r.h.ch = startx;
	r.h.cl = starty;
	r.h.dh = endx;
	r.h.dl = endy;
	r.h.bh = 7;

	int86(0x10, &r, &r);
}
