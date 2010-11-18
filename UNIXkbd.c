#include "Struct.h"

void in_die()
{
	printf("\nHousekeeping...Will Exit UNIXkbd NOW");
	exit(0);
}

int main (int argc, char * argv[]){

	sigset(SIGINT,in_die); //When we quit the program, the RTX parent process will send a kill signal to this process. 
												 //Memory cleanup will be done by the parent, this process just needs to exit
	
	int parent_id, file_id;
	int bufferSize = BUFFERSIZE;
	caddr_t mmap_ptr;
	

	struct Buffer * input_mem_p;
	char c;
	sscanf(argv[1], "%d", &parent_id);
	sscanf(argv[2], "%d", &file_id);

	mmap_ptr = mmap((caddr_t) 0,
		bufferSize,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		file_id,
		(off_t) 0);

	if (mmap_ptr == NULL)	
		printf("The memory mapping for the child failed.");
	input_mem_p = (struct Buffer *)mmap_ptr;
	input_mem_p->completedFlag = 0;
	
	int i = 0;
	
	do{
		   c = getchar();
		if (c == '\n'){ 
		
			input_mem_p->data[i] ='\0';
			i = 0;
			input_mem_p->completedFlag = 1; //LETS THE IPROCESS KNOW ITS READY. 
/*			printf("UNIXkbd says: Input received. Notifying keyboardIprocess\n");*/
/*			fflush(stdout); */
			kill(parent_id, SIGUSR2); //sends signal to the mainRTX.
			while (input_mem_p->completedFlag == 1)
			sleep(1);
		}
		else{
			if(strlen(input_mem_p->data) < MAXCHAR){ //checks that there is still some space in the buffer. 
				input_mem_p->data[i] = c;
				i++;
			}
		}
	}while(1);
	printf("THERE IS AN ERROR IN THE UNIX KEYBOARD PROCESS");
}
