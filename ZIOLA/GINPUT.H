#define ON  1
#define OFF 0

unsigned char erc,bps;

char s_date[12];
char s_time[9];

unsigned char save_atrib;

time_t start_time,new_time,time_out_start,time_out_check;
long int time_allowed,time_out;


unsigned char display_time[60];

unsigned char user_path[41];

unsigned char upper();
unsigned char get_yn();
unsigned char send[2];

unsigned char response[82];
unsigned char r_color[82];

struct disect {
 char level[80];
} cut[20];

char temp[81];

unsigned char valid_user;

unsigned char user_ansi,mistake;

unsigned char chat_flag;

unsigned char s_bar;

unsigned char line[81];
unsigned char line2[81];
unsigned char blnk[81];


unsigned char reserve[81];


unsigned char first[81];
unsigned char last[81];

struct last_scan_date {
   unsigned char last_date[10][11];
   unsigned char last_time[10][9];
   unsigned char tag[10];
} l_scan_date;

struct last_scan_date2 {
   unsigned char last_date[10][11];
   unsigned char last_time[10][9];
   unsigned char touched[1];
} l_scan_date2;



struct last_read {
   long int mes_pnt[50];
   long int base_pnt[50];
   unsigned char scan[50];
} lr_pnt;

long int user_rec_num;

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

void cut_string(void);
void strip_blanks(void);
void get_string(unsigned char length,char out) ;
int inkey(void);
void clear_input(void);
void write_str (char * ch,int x,int y,int atrib);
unsigned char upper(unsigned char c);
unsigned char lower(unsigned char c);
unsigned char get_yn(char * in);
void get_phone(void);
void delete(unsigned char number);
void get_date(void);
void get_number(unsigned char length,unsigned char in[80]);
void m_or_f(void);
void voice(void);
void data(void);
void bdate(void);
void screen_length(void);
void screen_clear(void);
void password(void);
void get_name(void);
void get_city(void);
void status_line(void);
void g_date(void);
char save_user(unsigned char search,char loc);
void read_file(void);
char file_lock(char *filename);
void file_unlock(char *filename);
void create_file(void);
void login(void);
int check_keys(int c);
void ansi_level(void);
void user_file_name(unsigned char ext[]);
void correct(unsigned char save);
void mid(unsigned char str[],unsigned char start,unsigned char num);
void display_phone(unsigned char phone[]);
void display_bdate(unsigned char bdate[]);
void display_sex(void);
void display_yn(unsigned char str[]);
void more_select(unsigned char toggle);
void more_prompt(void);
void send_clear_screen(void);
void get_password(void);
void press_return(void);
void save_video(int startx,int endx,int starty,int endy,unsigned char *buf_ptr);
void restore_video(int startx,int endx,int starty,int endy,unsigned char *buf_ptr);
void select_more_prompt(void);
unsigned char is_valid(unsigned char c);
