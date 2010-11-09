#include "Struct.h"
//#include "publicProcesses.h"

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
	struct messageEnvelope* env = NULL;
	env = k_receive_message();
	if (env != NULL){//if sleep request appears
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
	//&&displayWallClock ==1
	 if(absoluteTime%10 ==0) //only displays the wall clock if the CCI demanded it
	 { wallClock();
	 }

}

void iProcessCRT(){
	if((*CRTSharedMemPointer).completedFlag == 1){//Indicates CRT has completed copying. Means empty buffer.
		struct messageEnvelope* env = NULL;
		env = k_receive_message();//primitive name
	
		if (env != NULL){ //which it should always be the case
		    int i;
		    strcpy((*CRTSharedMemPointer).data, env->data);//Copies the contents of env->data to buffer
	            env->messageType = MSGTYPEACK;
	            (*CRTSharedMemPointer).bufferLength = i;
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
                    //strcpy(env->data, (*keyboardShareMemPointer).data);
		    int bufferLength = (*keyboardSharedMemPointer).bufferLength;
		    for(int i = 0; i < bufferLength; i++){//assume env->data is a char[MAXCHAR]?
		       env->data[i] = (*keyboardSharedMemPointer).data[i];
	    	    }
		    env->messageType = MSGCONSOLEINPUT;
		    (*keyboardSharedMemPointer).completedFlag = 0;
		    (*keyboardSharedMemPointer).bufferLength = 0;
		    k_send_message(env->PIDSender, env);
		    //printf("Found in Data Buffer: %s\n",dataBuffer);
		}
	}
else{
	k_release_message_env(env);//if mem not ready, ignore env; will never happen
}
}





