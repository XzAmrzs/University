#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/stat.h>
#include <string.h>
#include <error.h>
#include <fcntl.h>

#include <sys/types.h>
#define MAX_SIZE 1024

int main(void)
{
	int fd,len;
	char inbuf[MAX_SIZE] = "Hello Xzp";
	char outbuf[MAX_SIZE];
	
	fd = open("/dev/mydev", O_WRONLY);
	if(fd <= 0){
		printf("Error opening device for writing!\n");
		exit(0);
	}
	
	len = write(fd, inbuf, strlen(inbuf));
	if (len < 0)
	{
		printf("Error writing from device!\n");
		exit(1);
	}
	printf("\n There are %d bytes written to device!\n", len);
	close(fd);
	
	fd = open("/dev/mydev", O_RDONLY);
	if(len < 0){
	    printf("Error opening device for reading!\n");
		exit(1);
	}
	len = read(fd, outbuf, len);
	if(len < 0 ){
		printf("Error reading from device!\n");
		close(fd);
		exit(1);
	}
	printf("\n There are %d bytes read from device!\n", len);
	
}

