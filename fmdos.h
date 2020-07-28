char fcb[36];

char get_user()
{
 return bdos(32, 0xff);
}

void set_user(u)
char u;
{
 if (u>=0 && u<16)
  return bdos(32, u);
}

char get_drive()
{
 return bdos(25);
}

void set_drive(d)
char d;
{
 bdos(14, d);
}

int dos_first()
{
 char c;
 bdos(26, 0x80);
 
 for(c=13;c<36;c++) fcb[c]=0;
 fcb[0]=1+get_drive();
 for(c=1;c<13;c++) fcb[c]='?';
 return bdos(0x11, &fcb);
}

int dos_next()
{
 return bdos(0x12, &fcb);
}
