void kbd_raw();
void kbd_restore();

extern int COLS, ROWS;
void tty_get_size();
void tty_setup_winch();
