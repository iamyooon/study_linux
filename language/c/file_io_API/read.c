#include <stdio.h>         // puts()
#include <string.h>        // strlen()
#include <fcntl.h>         // O_WRONLY
#include <unistd.h>        // write(), close()

#define  BUFF_SIZE   1024

int main()
{
   char   buff[BUFF_SIZE];
   int    fd;

   if ( 0 < (fd = open( "./test.txt", O_RDONLY)))
   {
      read(fd, buff, BUFF_SIZE);
      puts(buff);
      close(fd);
   }
   else
   {
      printf( "file open failed.\n");
   }
   return 0;
}
