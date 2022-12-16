#include <stdio.h>
 
#define  MAX_LEN  100
 
int main(void)
{
   FILE *stream;
   char line[MAX_LEN], *result;
 
   stream = fopen("mylib/myfile","rb");
 
   if ((result = fgets(line,MAX_LEN,stream)) != NULL)
       printf("The string is %s\n", result);
 
   if (fclose(stream))
       perror("fclose error");
}