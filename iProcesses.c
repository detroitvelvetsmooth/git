#include "Struct.h"
#include "publicProcesses.h"

void wallClock(){
	int math = relativeTime/10;
	int hour = (math/3600)%24;
	math %= 3600;
	int min = math / 60;
	int sec = math % 60;
	printf("%02d:%02d:%02d\n",hour,min,sec);  //THIS WILL BE MODIFIED, INSTEAD OF PRINTF, IT WILL COMMUNICATE WITH THE CRT PROCESS (SOMEHOW) 
}

void iProcessAlarm(){ //THIS FUNCTION IS IN A NON WORKING STATE . TODO
/*
	absoluteTime++;
	relativeTime++;
	
	struct messageEnvelope* env = NULL;
	env = k_receive_message();

	if (env != NULL){//if sleep request appears
		
		//	there is a new request. Insertion Sort the messages into the message envelope queue. 
		
		sqEnqueue(env->PIDSender,env);//still gotta make this funtion.. does it iterate through global PCB ptr
		//What does it do with the env?
		}

	
	if (ptrPCBTiming != NULL){
		ptrTimingList->data --;//does int ticks var exist in PCBs? How do env keep track??TODO

		if (ptrPCBTiming->ticks <= 0){//what if two are 0? do-while?
			env = sqDequeue();//returns env ptr
			env->messageType = MSGTYPEWAKEUP;//enumerated?
			k_send_message(env->PIDSender,env);
			}
		}	
	 if(relativeTime%10 ==0) //only displays the wall clock if the CCI demanded it
	 { wallClock();
	 }
*/
}

void iProcessCRT(){ //THIS FUNCTION IS IN A NON WORKING STATE TODO.

	if((*CRTSharedMemPointer).completedFlag == 1){//Indicates CRT has completed copying. Means empty buffer.
		struct messageEnvelope* env = NULL;
		env = k_receive_message();//primitive name
	
		if (env != NULL){ //which it should always be the case
		    int i;
		    strcpy((*CRTSharedMemPointer).data, env->data);//Copies the contents of env->data to buffer
	      env->messageType = MSGTYPEACK;
	      (*CRTSharedMemPointer).completedFlag = 0; //Indicate to UNIXCRT that buffer is loaded
	      k_send_message(env->PIDSender,env);
	  }
  }
}

void iProcessKeyboard(){

	if((*keyboardSharedMemPointer).completedFlag == 1)
	{
		messageEnvelope* env = NULL;
		env = k_receive_message();
			if (env != NULL){
	        
	        strcpy(env->data, (*keyboardShareMemPointer).data);
			    env->messageType = MSGCONSOLEINPUT;
			    (*keyboardSharedMemPointer).completedFlag = 0;
			    k_send_message(env->PIDSender, env);
			}
		}
	else{
		k_release_message_env(env);//if mem not ready, ignore env; will never happen (SINCE COMPUTERS ARE FAAAAST)
	}
}

/*int TimingListEnqueue(struct messageEnvelope* env){

	if (env != NULL){
	
  struct messageEnvelope* temporary = NULL; // 
	temporary = ptrTimingList; //TEMPORARY POINTS TO THE FRONT OF THE LIST. 
	
	if (env->sleepTicks < temporary->sleepTicks){//case 1: front of list insertion
		ptrTimingList = env;
		env->ptrNextMessage = temporary;
		temporary->sleepTicks = temporary->sleepTicks - env->sleepTicks;
		return 1;
	}
	
	env->sleepTicks = env->sleepTicks - temporary->sleepTicks;
	
	while (ptr->ptrNextMessage){//this while condition sucks
		if (env->ticks < ptr->ptrNextMessage->ticks){//will tail insertion work?
			env->ptrNextMessage = ptr->ptrNextMessage;
			ptr->ptrNextMessage = env;
			}
		ptr = ptr->ptrNextMessage;
		env->ticks -= ptr->ticks;
	}
	ptr = env->ptrNextMessage;
	do{
	ptr->ticks -= env->ticks;
	ptr = ptr->ptrNextMessage;	
	}while (ptr);//decrements all following envs
		
	}
}
*/



