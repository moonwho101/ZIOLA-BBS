struct menu_struct {
   unsigned char key;
   unsigned int security;
   unsigned char display[81];
   unsigned char password[11];
   unsigned char command[81];
   unsigned int function;
} menu[25];


unsigned int do_function;
unsigned char mess_options[80];



void men(unsigned char fl[100]);
unsigned int get_funct(void);
void get_menu(unsigned char *filename,unsigned char type);
void check_time(void);
