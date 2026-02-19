#define MSIZE 80

unsigned char stop_all_read;

unsigned char options[25];

unsigned char show_it;

unsigned char mess_path[20];

struct mess {
	unsigned char line[82];
	unsigned char color[82];
};

unsigned char blue_bar, sysop_chat_color, user_chat_color;

struct who {
	unsigned char to[41];
	unsigned char from[41];
	unsigned char received[2];
};

unsigned char normal[6];
unsigned char reply_s[7];

struct new_scan {
	long int area;
	long int mes_num;
} new_mail[200];

unsigned char new_mail_read;

struct mess_area_names {
	unsigned char name[40];
} mess_name[6];

struct index {
	unsigned char delete[2];
	unsigned char private[2];
	unsigned char date[11];
	unsigned char time[9];
	unsigned char subject[71];
	long int parent;
	long int sib;
	long int prev_sib;
	long int child;
	long int start;
	long int end;
	long int reply;
	long int last_reply;
};

struct index g_idx;
struct who g_who_tf;

struct m_area {
	unsigned char name[30];
} mess_area[2];

unsigned char show_it;

void stat_bar(void);
void file_name(unsigned char ext[], long int mes_area);
long int m_bound(long int mes_num);
void intro_line_editor(unsigned char who[], unsigned char public);
void display_m_area(void);
void message_file(void);
void read_type(void);
void delete_message(long int mes_num);
void draw_border(unsigned char startx, unsigned char starty, unsigned char endx, unsigned char endy, unsigned char b_col, unsigned char fil, unsigned char f_col);
void age(unsigned char date1[], unsigned char date2[]);
void days(unsigned char date1[], unsigned char date2[]);
void clean_bar(unsigned char start);
void skip_area(void);