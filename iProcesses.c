#include "Struct.h"


void wallClock(){

	int math = absoluteTime/10;
	int hour = (math/3600)%24;
	math %= 3600;
	int min = math / 60;
	int sec = math % 60;
	printf("%02d:%02d:%02d\n",hour,min,sec);  //THIS WILL BE MODIFIED, INSTEAD OF PRINTF, IT WILL COMMUNICATE WITH THE CRT PROCESS (SOMEHOW) 
}

void iProcessAlarm(){

	absoluteTime++;
	/*messageEnvelope* env = NULL;
	env = messageReceive();
	if (env != NULL){//if sleep request appears
		sqEnqueue(env->PIDsender,env->msgType);//requires iteration through main PCB list, m
		}
	blockSleep.head->ticks --;
	if (blockSleep.head->ticks <= 0)
	
	
		//check inbox for sleep requests
		//enqeue sleep request
		//minus sleep time
		//dequeue first msg of sleep time queue if zeroKeyboard I_proc must:
		//send msg to sleeping process
	*/
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

if((*keyboardSharedMemPointer).completedFlag == 1){
	messageEnvelope* env = NULL;
	env = k_messageReceive();
		
		if (env != NULL){//should we loop til inbox is empty? CCI can only ever send 1 request,then it gets blocked
		int bufferLength = (*keyboardSharedMemPointer).bufferLength;
		for(int i = 0; i < bufferLength; i++){//assume env->data is a char[MAXCHAR]?
		env->data[i] = (*keyboardSharedMemPointer).data[i];
		}
		env->type = CONSOLE_INPUT //enumerated?
		k_messageSend(env, env->sender_pid);
		(*keyboardSharedMemPointer).completedFlag = 0;
		(*keyboardSharedMemPointer).bufferLength = 0;
		//printf("Found in Data Buffer: %s\n",dataBuffer);
		}
	}else{
		k_messageDeallocate(env);//if mem not ready, ignore env; will never happen
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






