#include "Struct.h"


void wallClock(){

	int math = absoluteTime/10;
	int hour = (math/3600)%24;
	math %= 3600;
	int min = math / 60;
	int sec = math % 60;
	printf("%i:%i:%i\n",hour,min,sec);  //THIS WILL BE MODIFIED, INSTEAD OF PRINTF, IT WILL COMMUNICATE WITH THE CRT PROCESS (SOMEHOW) 
}

void iProcessAlarm(){

absoluteTime++;

	//check inbox for sleep requests
	//enqeue sleep request
	//minus sleep time
	//dequeue first msg of sleep time queue if zeroKeyboard I_proc must:
	//send msg to sleeping process
	
//&&displayWallClock ==1
 if(absoluteTime%10 ==0) //only displays the wall clock if the CCI demanded it. 
 { wallClock();
 }

}



void iProcessCRT(){}



/*
//Global Stuffs
char A[5];//to test printf of a string
int modInt = 0;
*/

void iProcessKeyboard(){

//create temp *env pointer
//reads its inbox (loop til empty) msg_receive
//check if memory is ready to be read
//if yes, copies data from buffer into env
	//sends it off to requesting process (sender of env) type CONESOLE_INPUT
//if no, ignores request (but this shouldn't happen)

if((*keyboardSharedMemPointer).completedFlag == 1){

	char dataBuffer[MAXCHAR];
	int i;

	for(i = 0; i < MAXCHAR; i++){
	dataBuffer[i] = (*keyboardSharedMemPointer).data[i];
	}	

	(*keyboardSharedMemPointer).completedFlag = 0;
	(*keyboardSharedMemPointer).bufferLength = 0;
		printf("Found in Data Buffer: %s\n",dataBuffer);
  }
  
}
				


/*
int main() {    //the Main just tests random crap; don't worry about it
printf ("Howdy, World.\n");
printf ("This is your first LINUX .c compile.\n");

gcc while(true){
		for (int i=0; i<25000000; i++){ //this is approx delay of 100ms
		}
	iProcessAlarm();
}
A[0] = 'a';
A[1] = 'b';
A[2] = 'c';
A[3] = 'd';
A[4] = 'e';

char B[5];
for(int i=0; i<5; i++){
 B[i] = A[i]; }

printf("%s\n",B);
return 0;
}

*/






