#include "Struct.h"
#include "publicProcesses.h"

void preemptive(){

	struct PCB* ptr = NULL;
	ptr = ptrPCBReadyHigh->queueHead;

	if(ptr == NULL){//Assign ptr to the next available Ready Process
		ptr = ptrPCBReadyMed->queueHead;
		if(ptr == NULL){
			ptr = ptrPCBReadyLow->queueHead;
			if(ptr == NULL){
				ptr = ptrPCBReadyNull->queueHead;
			}
		}
	}
	if(ptr != NULL){//If process being interupted is NOT NULL process
		if(ptrCurrentExecuting->processPriority > ptr->processPriority){//If a Ready Process exists of higher priority
			k_release_processor();
			printf("Preempted\n");
			return;
		}
	}
}
		



struct messageEnvelope* TimingListDequeue(){
       if(ptrTimingList == NULL)
           return NULL;
       struct messageEnvelope* temp;
       temp = ptrTimingList;
       ptrTimingList = ptrTimingList->ptrNextMessage;
       return temp;
} 

int TimingListEnqueue(struct messageEnvelope* env){
	
    if (env != NULL){
		if(ptrTimingList == NULL){//case 1: TimingQueue is empty
			ptrTimingList = env;
			env->ptrNextMessage = NULL;
			return 1;
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

    while (temporary->ptrNextMessage != NULL){//Case 3: general insertion. Loop through till empty.
        if(env->sleepTicks >= temporary->ptrNextMessage->sleepTicks) //Or exit if position found
			break;
		env->sleepTicks = env->sleepTicks - temporary->ptrNextMessage->sleepTicks;
        temporary = temporary->ptrNextMessage;
    }
        env->ptrNextMessage = temporary->ptrNextMessage;
        temporary->ptrNextMessage = env;
        if (env->ptrNextMessage)
            env->ptrNextMessage->sleepTicks = env->ptrNextMessage->sleepTicks - env->sleepTicks;
        return 1;
    }
    return 0;//case 5: if env is NULL
    }
    
void iProcessAlarm(){
    atomic(1);
    struct PCB* temp = ptrCurrentExecuting;
    ptrCurrentExecuting = getPCB(PIDiProcessTimer);
    absoluteTime++;
    relativeTime++;
   
    struct messageEnvelope* env = NULL;
       
	do{
		env = k_receive_message();
		if(env != NULL){//if sleep request appears
			TimingListEnqueue(env);
		} 
	}while(env!= NULL);
       
    if (ptrTimingList != NULL){
        ptrTimingList->sleepTicks --;
				//printf("WallClock sleepTicks: %d\n", ptrTimingList->sleepTicks);
        while (ptrTimingList!=NULL && ptrTimingList->sleepTicks == 0){//what if two are 0? do-while?
            env = TimingListDequeue();//returns env ptr
            env->messageType = MSGTYPEWAKEUP;
            k_send_message(env->PIDSender,env);
        }
    }   
    ptrCurrentExecuting = temp;
   //preemptive();
   atomic(0);
}

	void iProcessCRT(){ 
	
	atomic(1);
	//Change current executing pcb to iProcessCRT's PCB
	struct PCB * temp = ptrCurrentExecuting; 
	ptrCurrentExecuting = getPCB(PIDiProcessCRT);

	static struct messageEnvelope* outputMsg = NULL; //outputMsg will need to be stored for future use

	if(outputMsg == NULL){ //The iProcessCRT is not fulfilling another message output at this moment.
		outputMsg = k_receive_message();
		if(outputMsg == NULL){
		}
		else{
			strcpy((*CRTSharedMemPointer).data, outputMsg->messageText);
			(*CRTSharedMemPointer).completedFlag = 1;
		}
	}
	else if((*CRTSharedMemPointer).completedFlag == 1){//The iProcessCRT is fulfilling another message output at this moment.
	}
	else{//The UNIXCRT has signalled the RTX to let it know it has emptied and printed the buffer.
		outputMsg->messageType = MSGTYPEACK;
		k_send_message(outputMsg->PIDSender, outputMsg); //Send aknowledgement message to the process with the fulfilled request
		outputMsg = k_receive_message(); //Check if there are more messages to be outputted.
		if (outputMsg != NULL){ //If there are, copy to buffer.
			strcpy((*CRTSharedMemPointer).data, outputMsg->messageText);
			(*CRTSharedMemPointer).completedFlag = 1;
		}
	}
	ptrCurrentExecuting = temp; //Reset to the current executing process.
	//preemptive();
atomic(0);
}



void iProcessKeyboard(){
	atomic(1);
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
/*			    printf("iProcess Keyboard forwarding msg CCI: %s\n", env->messageText);*/
			}
	}	
	else{
			
			if(env!=NULL)
			k_release_message_env(env);//if mem not ready, ignore env; will never happen (SINCE COMPUTERS ARE FAAAAST)
	}
	  ptrCurrentExecuting = temp;
	//preemptive();
	atomic(0);
}




