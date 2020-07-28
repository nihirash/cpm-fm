char files[11 * 128];
char files_count;

char *extract_name(dmapos)
char dmapos; 
{
 return 0x81 + dmapos * 32;
}

void load_dir() 
{
 char dmapos; 
 int i;
 files_count=0;
 for (i=0;i<128*11;i++) files[i] = 0;
 
 dmapos = dos_first();
 if (dmapos==0xff) return;

 while (dmapos != 0xff) {
  movmem(extract_name(dmapos), &files[11 * files_count++], 11);
  dmapos = dos_next();
  if (files_count>127) return;
 }
}
