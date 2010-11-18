#include "Struct.h"
#include "publicProcesses.h"

/*void wallClock(){
	int math = absoluteTime/10;
	int hour = (math/3600)%24;
	math %= 3600;
	int min = math / 60;
	int sec = math % 60;
	
	struct messageEnvelope * msg =NULL;
    msg = k_receive_message();
    if(msg!=NULL)
	{
    	
        char * time[10];
        sprintf (time,"%02d:%02d:%02d\0",hour,min,sec);
    
         strcpy(msg->messageText,time);
         k_send_message(msg);
         iProcessCRT();
    }
	//printf("%02d:%02d:%02d\n",hour,min,sec);  //THIS WILL BE MODIFIED, INSTEAD OF PRINTF, IT WILL COMMUNICATE WITH THE CRT PROCESS (SOMEHOW) 
	
}
*/
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
   
        while (env->sleepTicks >= temporary->ptrNextMessage->sleepTicks && temporary->ptrNextMessage != NULL){//case 3: general insertion
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
   

}

void iProcessCRT(){ //THIS FUNCTION IS IN A NON WORKING STATE TODO.

	struct PCB * temp = ptrCurrentExecuting; 
	ptrCurrentExecuting = getPCB(PIDiProcessCRT);

	

	if((*CRTSharedMemPointer).completedFlag == 0){//Indicates CRT has completed copying. Means empty buffer.
		struct messageEnvelope* env = NULL;
	
		env = k_receive_message();//primitive name
	
		//printf("iProcessCRT forwarding msg to UNIXcrt: %s\n", env->messageText);
	
		if (env != NULL){ //which it should always be the case
		    
		    strcpy((*CRTSharedMemPointer).data, env->messageText);//Copies the contents of env->data to buffer
	      env->messageType = MSGTYPEACK;
	      (*CRTSharedMemPointer).completedFlag = 1; //Indicate to UNIXCRT that buffer is loaded
	      while((*CRTSharedMemPointer).completedFlag == 1);
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
			    printf("iProcess Keyboard forwarding msg CCI: %s\n", env->messageText);
			}
	}	
	else{
			
			if(env!=NULL)
			k_release_message_env(env);//if mem not ready, ignore env; will never happen (SINCE COMPUTERS ARE FAAAAST)
	}
	  ptrCurrentExecuting = temp;
}




