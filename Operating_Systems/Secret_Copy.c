#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096


int main(int argc, char *argv[]){
	
	if (argc!= 2){
		return 1;
	}
	
	char *inputFile = argv[1];
	
	char outputFile[256];

	snprintf (outputFile, sizeof(outputFile), ".%s", inputFile);

	int inFD = open(inputFile,O_RDONLY);
	if (inFD < 0){
		perror("ERROR OPENING INPUT FILE");
		return 1;
	}
	
	int outFD = open(outputFile, O_WRONLY | O_CREAT| O_TRUNC, 0400);
	if (outFD < 0)
	{
		perror("ERROR CREATING OUTPUT FILE");
		close(inFD);
		return 1;
	}

	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;
	
	while((bytesRead = read(inFD, buffer, BUFFER_SIZE)) >  0)
	{
		ssize_t bytesWritten = 0;
		while (bytesWritten < bytesRead){
			
			ssize_t result = write(outFD, buffer + bytesWritten, bytesRead - bytesWritten);
			
			if (result < 0){
			
				perror ("ERROR WRITING TO OUTPUT FILE");
			
				close(inFD);
			
				close(outFD);
			
				return 1;
			}
			bytesWritten += result;
		}
	}

	if (bytesRead < 0){

		perror("ERROR READING INPUT FILE");
	
		close(inFD);
		close(outFD);

		return 1;
	}
	
	if (chmod(outputFile, S_IRUSR) < 0){
		perror("ERROR SETTING FILE PERMISSIONS");	
	}

	if(close(inFD) < 0){
		perror ("ERROR CLOSING INPUT FILE");
	}

	if(close(outFD) < 0){
		perror("ERROR CLOSING OUTPUT FILE");
	}

	printf("Secret Copy Created: %s\n", outputFile);
	
	return 0;
}
