void speed(void);
void init_modem(void);
unsigned char ok(void);
void wait_caller(void);
void detect_ansi_graphics(void);
void put_str_serial(unsigned char *s);
void get_str_serial(void);

unsigned char connect_save[60];