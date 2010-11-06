#include "Struct.h"


void in_die()
{
	printf("\nHousekeeping...Will Exit UNIXkbd NOW");
	exit(0);
}


int main (int argc, char * argv[]){

	sigset(SIGINT,in_die);

	int parent_id, file_id;
  int bufferSize = BUFFERSIZE;

	caddr_t mmap_ptr;
	struct Buffer * output_mem_p;
	char output_text[MAXCHAR];

	sscanf(argv[1], "%d", &parent_id);
	sscanf(argv[2], "%d", &file_id);


	mmap_ptr = mmap((caddr_t) 0,bufferSize,PROT_READ | PROT_WRITE,MAP_SHARED,file_id, (off_t) 0);

	if (mmap_ptr == NULL){
		printf("ISSUES");
		exit(0);
	}

	output_mem_p =(struct Buffer*) mmap_ptr;
	output_mem_p->bufferLength = 0;
	int i;
	do{
		while(output_mem_p->completedFlag == 0)
			sleep(1);
		for (i = 0; i < output_mem_p->bufferLength; i++){
			output_text[i] = output_mem_p->data[i];
			output_mem_p->data[i] = '\0';
		}
		output_mem_p->data[output_mem_p->bufferLength] = '\0';
		printf("UNIXcrt says: %s", output_text);
		output_mem_p->completedFlag = 1;
		output_mem_p->bufferLength = 0;
		kill(parent_id, SIGUSR1);
	}while(1);
	printf("AN ISSUE HAS OCCURED WITHIN THE UNIX CRT PROCESS");
}
