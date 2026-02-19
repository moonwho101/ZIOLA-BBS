#include <setjmp.h>

#define ON 1
#define OFF 0

long int number_of_mess_areas;
long int number_of_file_areas;


char dummy_local ;
unsigned int delay_time;

unsigned char abort_file,carrier_detect;

unsigned char no_more;

jmp_buf state;


unsigned char save_ansi_param[81];

char user_input;
char direct_write;
char far *vid_mem;
int atrib;
int xloc,yloc;
int savex,savey;

unsigned char upx,upy,lowx,lowy,bot;

unsigned char hot_of,key_pressed;

int index[31];
unsigned char bottom;

struct scrn {
    char chrtr[82];
    int atrib[82];
} screen[31];

char obey_ansi_input;
char grab_ansi;
unsigned char local;
unsigned char file_scroll,under_bar;

unsigned char options[25];
unsigned char more_of,more_r;

long int more;


void ansi_param( char setting[], char type);
void scrn_ctrl(unsigned int test[],unsigned char sz,char type);
void goto_xy(int x,int y);
void write_char(int x,int y,char ch,int attrib,char save);
video_mode(void);
void get_video(void);
void cls(unsigned char startx,unsigned char starty,unsigned char endx, unsigned char endy);
void scroll_window(unsigned char startx,unsigned char starty,unsigned char endx,unsigned char endy,unsigned char lines);
void save_scrn(void);
void p(char *ansi,char *str);
void system_log_break(void);
void write_system_log(void);