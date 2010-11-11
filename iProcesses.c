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
  struct PCB * temp = ptrCurrentExecuting; 
	ptrCurrentExecuting = getPCB(PIDiProcessCRT);
	
	
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
	 
	ptrCurrentExecuting = temp;
	
*/
}

void iProcessCRT(){ //THIS FUNCTION IS IN A NON WORKING STATE TODO.

	struct PCB * temp = ptrCurrentExecuting; 
	ptrCurrentExecuting = getPCB(PIDiProcessCRT);
	
	if((*CRTSharedMemPointer).completedFlag == 0){//Indicates CRT has completed copying. Means empty buffer.
		struct messageEnvelope* env = NULL;
	
		env = k_receive_message();//primitive name
	
		printf("iProcessCRT: got Here %s", env->messageText);
	
		if (env != NULL){ //which it should always be the case
		    
		    strcpy((*CRTSharedMemPointer).data, env->messageText);//Copies the contents of env->data to buffer
	      env->messageType = MSGTYPEACK;
	      (*CRTSharedMemPointer).completedFlag = 1; //Indicate to UNIXCRT that buffer is loaded
	      k_send_message(env->PIDSender,env);
	  }
  }
  
  ptrCurrentExecuting = temp;
}

void iProcessKeyboard(){

	struct PCB * temp = ptrCurrentExecuting; 
	ptrCurrentExecuting = getPCB(PIDiProcessKeyboard);
	
	struct messageEnvelope* env = NULL;
	env = k_receive_message();

	if((*keyboardSharedMemPointer).completedFlag == 1)
	{		
			if (env != NULL){
	        
	        strcpy(env->messageText, (*keyboardSharedMemPointer).data);
			    env->messageType = MSGCONSOLEINPUT;
			    (*keyboardSharedMemPointer).completedFlag = 0;
			    k_send_message(env->PIDSender, env);
			}
	}	
	else{
			
			if(env!=NULL)
			k_release_message_env(env);//if mem not ready, ignore env; will never happen (SINCE COMPUTERS ARE FAAAAST)
	}
	  ptrCurrentExecuting = temp;
}

/*int TimingListEnqueue(struct messageEnvelope* env){

	if (env != NULL){
	
		if(ptrTimingList == NULL){//case 1: TimingQueue is empty
			ptrTimingList = env;
			env->ptrNextMessage = NULL;
			}
		
		struct messageEnvelope* temporary = NULL;
		temporary = ptrTimingList; //TEMPORARY POINTS TO THE FRONT OF THE LIST. 
	
	
		if (env->sleepTicks < temporary->sleepTicks){//case 2:insertion to front of list
			ptrTimingList = env;
			env->ptrNextMessage = temporary;
			temporary->sleepTicks = temporary->sleepTicks - env->sleepTicks;
			return 1;
		}
	
		env->sleepTicks = env->sleepTicks - temporary->sleepTicks;
	
		while (env->sleepTicks >= temporary->ptrNextMessage->sleepTicks){//case 3: general insertion
			env->sleepTicks = env->sleepTicks - temporary->ptrNextMessage->sleepTicks;
			temporary = temporary->ptrNextMessage;
			}
			env->ptrNextMessage = temporary->ptrNextMessage;
			temporary->ptrNextMessage = env;
			if (env->ptrNextMessage)//case 4: insertion as last Queue element
				env->ptrNextMessage->sleepTicks = env->ptrNextMessage->sleepTicks - env->sleepTicks;
			return 1;
		}
		return 0;//case 5: if env is NULL
	}

}
*/



