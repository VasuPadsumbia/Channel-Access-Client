#include <cadef.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alarm.h>

dbr_string_t data;
dbr_string_t data_1;
struct dbr_ctrl_double ctrl;
int num;
FILE *fptr;
int main(int argc, char **argv) 
{

	fptr = fopen("/home/vasu/client/cacApp/O.linux-x86_64/test.txt","w");
	if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   printf("Enter num: ");
   scanf("%d",&num);

   fprintf(fptr,"%d",num);
   fclose(fptr);

   return 0;
}