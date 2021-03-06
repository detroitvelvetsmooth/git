#include "publicAPI.h"
#include <stdio.h>
#include <string.h>


//////// FUNCTION PROTOTYPES AND STRUCTS///////////////////

struct messageEnvelope* ProcessCQHead;
struct messageEnvelope* ProcessCQTail;
void CEnqueue(struct messageEnvelope* newEnv);
struct messageEnvelope* CDequeue();
     struct messageEnvelope* ProcessCQHead = NULL;
     struct messageEnvelope* ProcessCQTail = NULL;

///////////////// PROCESSES ////////////////////



void ProcessA(){


/*	 printf("Entered Process A\n");*/
     struct messageEnvelope* start;
    
     start = receive_message();
     //Check for start message sent by CCI
     
	 while(strcmp(start->messageText, "s\0")!=0){
	 	release_message_env(start);
        start = receive_message();
	 }
/*     printf("\nStart command received. Starting Process A\n");*/
     release_message_env(start);
     int count, status;
     count = 0;
     struct messageEnvelope* countEnv;
     //Start infinite loop
     do{
        //Request Envelope, change message type, place counter contents in message, and send to process B
        countEnv = request_message_env();
        countEnv->messageType = MSGTYPECOUNT;
        sprintf(countEnv->messageText,"%d",count);
        status = send_message(PIDUserProcessB, countEnv);
        if (status != 1)
           printf("\n Send_Message failed from ProcessA to ProcessB\n");
        count ++;
        release_processor();
     }while(1); 
     printf("\nProcess A code reached a point where it shouldn't. ERROR.\n");
}

void ProcessB(){



/*	printf("Entered Process B\n");*/

    struct messageEnvelope* BTemp;
     int status;
     //Start infinte loop
     do{
        //Receive message and send to process C, then release processor.
        BTemp = receive_message();               
        status = send_message(PIDUserProcessC, BTemp);
        if (status!=1)
           printf("\n Send_Message failed from ProcessB to ProcessC\n");
        release_processor();
     }while(1);
     printf("\nProcess B code reached a point where it shouldn't. ERROR. \n");
}

void ProcessC(){
     
/*     printf("Entered Process C\n");*/
     struct messageEnvelope* CEnv;
     int status, count;
     
     //Start infinite loop.
     do{
        //Check local queue for waiting messages. Dequeue a waiting message, otherwise receive message.
		 if(ProcessCQHead==NULL){  //CHECKS TO SEE IF IT IS EMPTY
            CEnv = receive_message();
		 }
        else
            CEnv = CDequeue();       //RETURNS MESAGE FROM LOCAL QUEUE. 
            //Check messagetype to make sure its from process B.
       
        if (CEnv->messageType == MSGTYPECOUNT){
            //If message content is divisable by 20 then continue.
            sscanf(CEnv->messageText, "%d", &count); //TURNS FROM A CHARACTER ARRAY TO AN INTEGER.
            
            //we don't want to display the very first one
            
            if (count%20 == 0 && count!= 0){ // means it is divisible.
				strcpy(CEnv->messageText, "Process C\0");
               
               status = send_console_chars(CEnv);
               if (status != 1)
                  printf("\nSend_console_chars failed for Process C. ERROR.\n");
                  
               CEnv = receive_message();
               //Wait for output acknowledgement. Enqueue all non-acknowledge messages to local queue.
               while(CEnv->messageType != MSGTYPEACK){
                   CEnqueue(CEnv); 
                   CEnv = receive_message();
               }
               
			   //Request Sleep. 
               request_delay(100,CEnv);
             
               //Once done sleeping, enqueue all messages received during sleep to the local queue.
               do{
                  CEnv = receive_message();
                  if (CEnv->messageType == MSGTYPEWAKEUP)
                      break;
                  CEnqueue(CEnv);
               }while(1);
            }
        }
        //Release message envelope. Will either be a msgtypewakeup, or a non-divisable by 20 message from B.
        status = release_message_env(CEnv);
        if (status != 1)
           printf("\nRelease_message_env failed for Process C. ERROR.\n");
        release_processor(); 
     }while(1);
}

void NullProcess(){
//Infinite Loop    
     int k;
     do{
		   k++;
		   k--;
		   k++;
		   k--;
           release_processor();
     }while(1);
}
void CCI()
{

   	int sentS=0;
	int hour, min, sec;
	int newPri, PID;
	int check_result;
	char tempChar;
	struct messageEnvelope *temp;
	//printf("Requesting Env.\n");
	temp = request_message_env(); //ONLY REQUESTS ONE ENVELOPE TOTAL. 
	//printf("Got env.\n");
	temp ->PIDSender = PIDcci;
	temp ->messageType = (int)MSGTYPEDATA;
	
	while(1)
	{
	
		strcpy(temp->messageText, "CCI:\0"); 
		send_console_chars(temp);
		//printf("Receiving ACK.\n");
		temp = receive_message(); //GETS THE MESSAGE BACK FROM THE CRT. 
		//printf("Received ACK.\n");
		get_console_chars(temp);
		temp = receive_message(); //assuming KB iProcess sends env    back to process //GETS THE MESSAGE BACK FROM THE KEYBOARD IPROCESS.
		
		while(strcmp(temp->messageText, "\0")==0){
			get_console_chars(temp);
			temp = receive_message();
		}

		if(temp->messageType != MSGCONSOLEINPUT){
			strcpy(temp->messageText, "Message sent to CCI that was not from keyboard. Message ignored.\0");
			send_console_chars(temp);
			temp = receive_message();
		}
		else if(strcmp(temp->messageText, "s\0")==0)
		{ 
			if(sentS==0){
			
           		struct messageEnvelope * messageForProcess = NULL;
				messageForProcess = request_message_env();
				strcpy(messageForProcess->messageText,temp->messageText);
				send_message((int)(PIDUserProcessA), messageForProcess);
				sentS = 1;
				release_processor(); 
			}
			else{
				strcpy(temp->messageText, "Process A has already been started.\0");
				send_console_chars(temp);
				temp = receive_message();
			}
		}
		else if(strcmp(temp->messageText, "ps\0")==0)
		{
			request_process_status(temp);
			send_console_chars(temp); 
			temp = receive_message();
		}
	
		else if(strcmp(temp->messageText, "cd\0")==0)
		{    
			if(displayWallClock)
			{		
					strcpy(temp->messageText, "Wall Clock is already on.\0");
					send_console_chars(temp);
					temp = receive_message();
			}
			displayWallClock=1;//change flag of wall clock to send time to CRT every second
		}
		else if(strcmp(temp->messageText, "ct\0")==0)
		{
			if(!displayWallClock)
			{		
					strcpy(temp->messageText, "Wall Clock is already OFF.\0");
					send_console_chars(temp);
					temp = receive_message();
			}
		
			displayWallClock=0;//change flag of wall clock to stop sending time to CRT
		}
		else if(temp->messageText[0] == 'c' && temp->messageText[1] == ' ')
		{
			if(sscanf(temp->messageText, "%c %d:%d:%d", &tempChar, &hour, &min, &sec) == 4){ //needs to check if correct number of items passed
				if(hour < 0 || hour >=24 || min < 0 || min >=60 || sec < 0 || sec >= 60)
				{			
					strcpy(temp->messageText, "Illegal Time Entered.\0");
					send_console_chars(temp);
					temp = receive_message();
				}
				else
				{
	  				relativeTime = (hour*3600+min*60+sec)*10; //Sets the relative time. 	
				}
			}
			else{
					strcpy(temp->messageText, "Illegal Command Entered.\0");
					send_console_chars(temp);
					temp = receive_message();
			}
		}
		else if(strcmp(temp->messageText, "b\0")==0)
		{
			get_trace_buffers(temp);
			send_console_chars(temp);
			temp = receive_message();
		}
		else if(strcmp(temp->messageText, "t\0")==0||strcmp(temp->messageText, "exit\0")==0)
		{
			terminate();
		}
		else if(temp->messageText[0] == 'n' && temp->messageText[1] == ' ') //WORKS?
		{
			if(sscanf(temp->messageText, "%c %d %d", &tempChar, &newPri, &PID)==3){ //needs to check if correct number of items passed
				check_result = change_priority(newPri, PID); 
				if(check_result == -1) //Priority or PID didn't match values in system
				{
					strcpy(temp->messageText, "Priority Change Failed.\0");
					send_console_chars(temp);
					temp = receive_message();
				}
				else if(check_result == 0) //Tried to change priority of process that cannot be changed.
				{
					strcpy(temp->messageText, "Illegal PID entered. Unable to change process ID of NULL process.\0");
					send_console_chars(temp);
					temp = receive_message();
				}
			}
			else{ //Scan format failed
					strcpy(temp->messageText, "Illegal Priority Command Format Entered.\0");
					send_console_chars(temp);
					temp = receive_message();
			}

		}
		else //THIS WORKS
		{ 
			strcpy(temp->messageText, "Illegal Command.\0");
			send_console_chars(temp);
			temp = receive_message();
		}
	 
	}
}

void WallClock(){

     struct messageEnvelope* temp;//temp is out temporary time env
     temp = request_message_env();
     int math, hour, min, sec;
     char time[10];
     do{  
    	if(displayWallClock == 1){
			temp->messageType = MSGTYPEDATA;                    
			math = relativeTime/10;
			hour = (math/3600)%24;
			math %= 3600;
			min = math / 60;
			sec = math % 60;
			sprintf (time,"%02d:%02d:%02d",hour,min,sec); 
			strcpy(temp->messageText,time);
			send_console_chars(temp);
			temp = receive_message();
			//temp->messageType = MSGTYPEWAKEUP;  
			}
		request_delay(10, temp);
		temp = receive_message();
		release_processor(); //is this release processor even necessary ?it will be blocked on receive anyways... 
     }while(1);
}
///////////////////// USER PROCESSES HELPER FUNCTIONS ////////////

struct messageEnvelope* CDequeue(){
    struct messageEnvelope* returnEnv;
    //Empty Queue Case
    if(ProcessCQHead==NULL){
       return NULL;
    }
    //Standard Case
    else{
       returnEnv = ProcessCQHead;
       //Queue of length 1 case
       if(ProcessCQHead == ProcessCQTail)
          ProcessCQTail = NULL;
       //Move the head pointer down the line
       ProcessCQHead = returnEnv->ptrNextMessage;
       returnEnv->ptrNextMessage = NULL;
       return returnEnv;
    }
}


void CEnqueue(struct messageEnvelope* newEnv){    
     newEnv->ptrNextMessage = NULL;
     //Empty Q case
     if (ProcessCQHead == NULL){
        ProcessCQHead = newEnv;
        ProcessCQTail = newEnv;
     }
     //Standard Q Case
     else{
        ProcessCQTail->ptrNextMessage = newEnv;
        ProcessCQTail = newEnv;
     }
}





