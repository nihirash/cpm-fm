#include <stdio.h>
#include <fmterm.h>
#include <fmdos.h>
#include <fmfiles.h>
#include <fmui.h>

#define LIMIT ROWS - 4
char cursor, offset;
char fnme[13];

void print_name(name)
char *name;
{
 char i;
 for (i=0;i<8;i++)
  printf("%c", name[i] & 0x7f);
  printf(".%c%c%c", name[8], name[9], name[10]);
}

char *find_name(pos)
char pos;
{
 return &files[pos * 11];
}

void draw_file(pos)
char pos;
{
 gotoxy(2, pos + 3);
 if (offset + pos < files_count)
  print_name(find_name(offset + pos));
 else
  puts("            ");
}

void update_files()
{
 char i;
 normal();
 draw_rect(1,2, 14, ROWS - 2);
 for (i=0;i<LIMIT;i++) draw_file(i);
 inverse();
 gotoxy(6, 2);
 printf("%c%d:", 'A' + get_drive(), get_user());
 normal();
}

void draw_cursor(i)
char i;
{
 if (i==1) inverse(); else normal();
 draw_file(cursor);
 if (i==1) printf("<"); else printf("|");
 normal();
}
void pr_rwin(y, text)
char y, *text;
{
 char x; 
 x = 15 + (COLS - 15) / 2 - strlen(text) / 2;
 gotoxy(x, y);
 
 puts(text); 
}

void draw_ui() 
{
 char c;
 cls();
 inverse();
 gotoxy(1,1);
 for(c=0;c<COLS;c++) printf(" ");
 gotoxy(1,1);
 printf(" File Manager 0.1 By Nihirash");
 normal();
 draw_rect(14, 2, COLS - 14, ROWS - 2);
 pr_rwin(3, "Quick help");
 pr_rwin(5, "^E ^X - Move cursor");
 pr_rwin(6, "E - Erase file");
 pr_rwin(7, "D - Change drive");
 pr_rwin(8, "U - Change User");
/* pr_rwin(9, "C - Copy file"); */
 pr_rwin(10, "Enter - Execute file");
 pr_rwin(12, "Q - Quit");
 update_files();
 draw_cursor(1);
 normal();
}

void cur_dn()
{
 if (cursor+offset>=files_count-1) 
  return;

 draw_cursor(0);
 cursor++;
 if (cursor>=LIMIT) {
  cursor = 0;
  offset += LIMIT;
  update_files();
 }
 draw_cursor(1);
}

void cur_up()
{
 draw_cursor(0);
 cursor--;
 if (cursor > LIMIT) {
  cursor = 0;
  if (offset>=LIMIT) {
   offset -= LIMIT;
   cursor = LIMIT - 1;
   update_files();
  }
 }
 draw_cursor(1);
}

void ext_fnme()
{
 char *nptr;
 char c, x;
 x=0;
 nptr = find_name(offset + cursor);
 for (c=0;c<8;c++)
  if (nptr[c] != ' ' && nptr[c] != 0) fnme[x++]=nptr[c];
 if (x==0) {
  fnme[0] = 0;
  return;
 }
 fnme[x++]='.';
  
 for (c=8;c<11;c++)
  if (nptr[c] != ' ') fnme[x++]=nptr[c];
 
 fnme[x] = 0;
}

void exec_com()
{
 char *f;
 f = find_name(offset + cursor);
 if (f[8] != 'C' || f[9] != 'O' || f[10] != 'M') return;
 ext_fnme();
 rst_crt();
 cls();
 exec(fnme);
}

void reload(c)
char c;
{
 if (c) {
  cursor=0;offset=0;
 }
 
 load_dir();
 draw_ui();
}

void era()
 {
 if (cursor + offset > files_count) return;
 if (confirm("Are you sure?") == 1) {
  ext_fnme();
  unlink(fnme);
 }
 reload(0);
}

void ch_dr()
{
 char c;
 dialog("Enter drive letter:");
 while(1) {
  c = toupper(bios(3));
  if (c>='A' && c<='P') {
   set_drive(c-'A');
   reload(1);
   return;
  }
 }
}

void ch_us()
{
 char c;
 dialog("Select user area(1-F)");
 while(1) {
  c = toupper(bios(3));
  if (c>='0' && c<='9') {
   set_user(c - '0');
   reload(1);
   return;
  } 
  if (c>='A' && c<='F') {
   set_user(c - 55);
   reload(1);
   return;
  }
 }
}

void main() {
 char c;
 
 c=0;
 reload(1);
 
 while(1) {
  c = bios(3);
  switch(c) {
   case 24:
    cur_dn();
    break;
   case 5:
    cur_up();
    break;
   case 'q':
   case 'Q':
    if (confirm("Do you want exit?")==0) {
     reload(1);
     continue;  
    }
    rst_crt();
    return;
   case 'e':
   case 'E':
    era();
    break;
   case 'd':
   case 'D':
    ch_dr();
    break;
   case 'u':
   case 'U':
    ch_us();
    break;
   case 13:
    exec_com();
    break;
   default:
    continue;
  }
 }
}
