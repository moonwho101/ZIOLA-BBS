void display_message(long int mes_num, unsigned char where_am_i[]);
unsigned char message_prompt(unsigned char trace, unsigned char delete, unsigned char reply, unsigned char select);
void read_message(long int mes_num, unsigned char direction);
unsigned char reply_prompt(unsigned char trace, unsigned char delete, unsigned char reply, unsigned char plus, unsigned char minus, unsigned char select_r);
void list_replies(long int mes_num);
void save_reply(long int mes_num);
long int select_reply(long int mes_num);
long int jump(unsigned char msg[], unsigned char start[]);
void delete_message(long int mes_num);
void q_scan(long int mes_num);

unsigned char quick_scan;
unsigned char header_scan;
