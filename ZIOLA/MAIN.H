
unsigned char one[4], two[4], big, kind;

unsigned char args[20][80];

FILE *system_log;

void log_out(void);
void rfile(unsigned char *filename, unsigned char fullscreen);
void delay_screen(unsigned int delay_loops);
unsigned char file_exist(unsigned char *filename);
void scan_new_mail(void);
void get_message_area(void);
void list_users(void);
void display_users(struct user_info *user_temp, unsigned char name[]);
void format(unsigned char p[], unsigned char length);
void page(void);
void beep(unsigned duration, unsigned frequency);
void delay_speaker(clock_t wait);
void shell_to_dos(void);
void chat(void);
void show_user(void);
void do_nothing(void);

void log_in_scan_files(unsigned char skip);
void new_file_search(void);
unsigned char cut_file(unsigned char test[80]);
unsigned char wild(unsigned char l[30]);
void search_files(void);

unsigned char hot_key(void);
unsigned char save_menu[81];
unsigned char show;
unsigned char connect_at[40];
unsigned char hot_key_number(void);

void get_text_to_search_for(void);
void search_which(void);
void file_stats(unsigned char p_return);
void get_date_scan(unsigned char put_it[]);
void create_dorinfo(void);