void draw_rect(x,y,w,h)
char x,y,w,h;
{
 char i, j;
 for (j=y;j<y+h;j++) {
  gotoxy(x, j);
  if (j==y || j==j+h-1)
   puts("+");
  else
   puts("|");
  
  for (i=1;i<w-1;i++)
   if (j==y || j==y+h-1)
    puts("-");
   else
    puts(" ");
    
  if (j==y || j==y+h-1)
   puts("+");
  else
   puts("|");
 }
}

void dialog(text)
char *text;
{
  char len;
  char x; 
  char y; 
  len=strlen(text);
  x=COLS/2-len/2;
  y=ROWS/2-1;
  inverse();
  draw_rect(x, y, len + 4, 3);
  gotoxy(x+2,y+1);
  puts(text);
  normal();
}

char confirm(text)
char *text;
{
 char c;
 dialog(text);
 c = 0;
 while (1) {
  c = bios(3);
  if (c=='y' || c=='Y') return 1;
  if (c=='n' || c=='N') return 0;
 }
}
