#define COLS 80
#define ROWS 25

void gotoxy(x, y)
char x, y;
{
 printf("%c[%d;%dH", 27, y, x);
}

void cls() {
 printf("%c[2J", 27);
 gotoxy(1,1);
}

void inverse() {
 printf("%c[47m%c[30m", 27, 27);
}

void normal() {
 printf("%c[44m%c[37m", 27, 27);
}

void rst_crt()
{
 printf("%c[0m%c[2J", 27, 27);
}
