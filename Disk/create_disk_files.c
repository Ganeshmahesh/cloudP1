#include<stdio.h>
#include<stdint.h>

int main()
{
  FILE *src_fp;
  FILE *trg_fp;
  uint64_t i;
  char a[2]="a";
  char b[2]="b";

  src_fp = fopen("disk_file.txt","w+");
  trg_fp = fopen("read_write_test_src.txt","w+");
 
  for(i=0;i<10737418240;i++)
  {
    fwrite(a, sizeof(char), 1, src_fp);
    fwrite(b, sizeof(char), 1, trg_fp);
  }
  fclose(src_fp);
  fclose(trg_fp);
  return 0;
}
