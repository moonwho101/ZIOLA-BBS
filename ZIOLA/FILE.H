#define FSIZE 80



unsigned char free_dl[20];
unsigned char ul_ratio[20];
unsigned char dl_ratio[20];

double ul_dl_ratio;
double system_ul_dl_ratio;


unsigned char st;
unsigned char text_search;

unsigned char other_areas;

unsigned char detail;

unsigned char search_f;
unsigned char search_name[30];

double total_t_time;

double true_time;

unsigned char found_new_files;
time_t in_door,out_door;
struct file_bbs {



   unsigned char who[41];
   unsigned char bps_rate[41];
   long int dl_number;
   unsigned char last_dl_date[9];
   unsigned char last_dl_time[9];
   unsigned char last_person_dl[41];
   unsigned char sysop_comment[81];
   long int attach_msg;
   long int private;
   long int start;
   long int finish;
   long int one_line_start;
   unsigned char detailed[2];
} g_file_bbs;

struct file_idx {
   unsigned char delete;
   unsigned char name[13];
   unsigned char date[11];
   unsigned char time[9];
   long int right;
   long int left;
} g_file_idx;


struct file_area_names {
  unsigned char name[40];
} file_names[6];


unsigned char file_path[81];
long int file_rec_num;


unsigned char file_temp[20];
unsigned char scan_new_files;


void get_file_name(void);
void create_file_areas(void);

struct files {
   unsigned char name[13];
   unsigned char size[21];
   unsigned char des[41];
   unsigned char date[14];
   long int true_size;
   long int record;
   unsigned char area[10];
} file_q[50];

unsigned char file_index[50];

unsigned char items_in_queue;

unsigned char file_response[82];
unsigned char file_r_color[82];

unsigned int mark_file;

unsigned char proto;
unsigned char batch_ul;

unsigned char xname[80],add_to_queue;
unsigned char tag;

unsigned char transfer_time[10];
unsigned char list_batch;
unsigned char true_area[10];


struct tag_it {

 unsigned char tag[13];


} tag_files[500];


struct txt_srch {

 unsigned char line[60];
} text[20];


unsigned char search_for[41];

unsigned char num_text_lines;

unsigned char tag_bypass;
unsigned char more_file;


char save_file (unsigned char search,char loc);
void file_name_files(unsigned char ext[],long int file_area);
void create_file_areas(void);
void get_file_name(void);
void display_file(unsigned char fname[]);
void batch(void);
void get_files(unsigned char length);
void srt(void);
void edit_queue(void);
void find_files(void);
void display_file_info(struct files *fdata,unsigned char detailed);
char binary(void);
void file_date(struct find_t *find);
void upload(void);
void file_exist_upload(void);
long int save_file_des (struct mess message[],unsigned char l_number,unsigned char loc[]);
void file_header_bar(void);
void list_files(void);
void list_protocols(void);
void dsz_upload(unsigned char ul);
void dsz_log(unsigned char ul);
void complete_file_name(unsigned char *fname);
void upload_header(void);
void display_f_area(unsigned char sz);
void queue_is_empty(void);
void calc_t_time(long int size);
void delete_from_queue(long int end,long int start);
void update_dl_counter(long int number);
void tag_the_file(void);
unsigned char check_file_date(unsigned char date1[11], unsigned char date2[11],unsigned char tim[9]);
unsigned char find_text(void);