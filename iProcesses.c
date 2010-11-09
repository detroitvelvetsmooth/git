#include "Struct.h"
#include "publicProcesses.h"

#define WAKEUP 0 ////This enumeration is preliminary if made up on the spot
#define DISPLAY_ACK 1//just so it would compile-ish
#define CONSOLE_INPUT 2



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

	if((*CRTSharedMemPointer).completedFlag == 1){//== 1 or 0?? Ask Andy
	
		struct messageEnvelope* env = NULL;
		env = k_receive_message();//primitive name
	
		if (env != NULL){ //which it should always be the case

		int i;
		while(env->data[i] != NULL){//how can I extract data length? Print MAXCHAR everytime?TODO while loop logic
			(*CRTSharedMemPointer).data[i] = env->data[i];
			i++;
		}
	
	env->messageType = DISPLAY_ACK; //enumerated?
	k_send_message(env->PIDSender,env);//or do we dealloacate msg?
	(*CRTSharedMemPointer).bufferLength = i;
	(*CRTSharedMemPointer).completedFlag = 0;//or 1??
	 }
   }
}

void iProcessKeyboard(){

if((*keyboardSharedMemPointer).completedFlag == 1){
	messageEnvelope* env = NULL;
	env = k_receive_message();
		
		if (env != NULL){//should we loop til inbox is empty? CCI can only ever send 1 request,then it gets blocked
		int bufferLength = (*keyboardSharedMemPointer).bufferLength;
		for(int i = 0; i < bufferLength; i++){//assume env->data is a char[MAXCHAR]?
		env->data[i] = (*keyboardSharedMemPointer).data[i];
		}
		env->messageType = CONSOLE_INPUT; //enumerated?
		k_send_message(env->PIDSender, env);
		(*keyboardSharedMemPointer).completedFlag = 0;
		(*keyboardSharedMemPointer).bufferLength = 0;
		//printf("Found in Data Buffer: %s\n",dataBuffer);
		}
	}else{
		k_release_message_env(env);//if mem not ready, ignore env; will never happen
		}
}






