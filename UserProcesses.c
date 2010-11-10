#include "publicAPI.h"
#include <stdio.h>
#include <string.h>


//////// FUNCTION PROTOTYPES AND STRUCTS///////////////////

struct CQueue{
    struct messageEnvelope* ptrMessageInboxHead;
    struct messageEnvelope* ptrMessageInboxTail;
};

struct CQueue* LocalQueue; // TODO - FIND A PROPER WAY TO INITIALIZE. 

void CEnqueue(struct CQueue* tempQ, struct messageEnvelope* newEnv);
struct messageEnvelope* CDequeue(struct CQueue* tempQ);
int isEmpty(struct CQueue* tempQ);

///////////////// PROCESSES ////////////////////



void ProcessA(){
     struct messageEnvelope* start;
     start = receive_message();
     //Check for start message sent by CCI
     
     while(start->messageText[0] !='s')
        start = receive_message();
     printf("\nStart command received. Starting Process A\n");
     release_message_env(start);
     int count, status;
     count = 0;
     struct messageEnvelope* countEnv;
     //Start infinite loop
     do{
        //Request Envelope, change message type, place counter contents in message, and send to process B
        countEnv = request_message_env();
        countEnv->messageType = MSGTYPECOUNT;
        countEnv->messageText[0] = (char)count;
        status = send_message(PIDUserProcessB, countEnv);
        if (status != 1)
           printf("\n Send_Message failed from ProcessA to ProcessB\n");
        count ++;
        release_processor(); //TODO: Write function.
     }while(1); 
     printf("\nProcess A code reached a point where it shouldn't. ERROR.\n");
}

void ProcessB(){
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
     //TODO: Perform Initialization of local message queue
     struct messageEnvelope* CEnv;
     int status, count;
     //Start infinite loop.
     do{
        //Check local queue for waiting messages. Dequeue a waiting message, otherwise receive message.
        if(isEmpty(LocalQueue)==0) 
            CEnv = receive_message();
        else
            CEnv = CDequeue(LocalQueue); //TODO: Fill in Proper function name.
        //Check messagetype to make sure its from process B.
       
        if (CEnv->messageType == (int)MSGTYPECOUNT){
            //If message content is devisable by 20 then continue.
            count = atoi(CEnv->messageText);
            if ((count%20) == 0){ //TODO: Check syntax on how this works.
              
               CEnv->messageText[0] = 'P';
               CEnv->messageText[1] = 'r';
               CEnv->messageText[2] = 'o';
               CEnv->messageText[3] = 'c';
               CEnv->messageText[4] = 'e';
               CEnv->messageText[5] = 's';
               CEnv->messageText[6] = 's';
               CEnv->messageText[7] = ' ';
               CEnv->messageText[8] = 'C';
               CEnv->messageText[9] = '\0';
               
               status = send_console_chars(CEnv);
               if (status != 1)
                  printf("\nsend_console_chars failed for Process C. ERROR.\n");
               CEnv = receive_message();
               //Wait for output acknowledgement. Enqueue all non-acknowledge messages to local queue.
               while(CEnv->messageType != (int)MSGTYPEACK){
                   CEnqueue(LocalQueue, CEnv); //TODO: Fill in Proper Function Name.
                   CEnv = receive_message();
               }
               //Request Sleep.
               CEnv->messageType = (int)MSGTYPESLEEP;
               CEnv->messageText[0] = '1';
               CEnv->messageText[1] = '0';
               CEnv->messageText[2] = '\0';
               requestSleep(CEnv); //TODO: Fill in Proper function name.
               //Once done sleeping, enqueue all messages received during sleep to the local queue.
               do{
                  CEnv = receive_message();
                  if (CEnv->messageType == (int)MSGTYPEWAKEUP)
                     break;
                  LocalEnqueue(CEnv);
               }while(1);
            }
        }
        //Release message envelope. Will either be a msgtypewakeup, or a non-divisable by 20 message from B.
        status = release_message_env(CEnv);
        if (status != 1)
           printf("\nrelease_message_env failed for Process C. ERROR.\n");
        release_processor(); 
     }while(1);
}

void NullProcess(){
//Infinite Loop
     do{
        release_processor();
     }while(1);
}
void CCI()
{
	int hour, min, sec;
	int newPri, PID;
	char tempChar;
	struct messageEnvelope *temp;
	temp = request_message_env(); //should we request and release an envelope on every cycle?
	
	while(1)
	{
	strcpy(temp->messageText, "CCI:"); 
	send_message((int)(PIDiProcessCRT), temp);
	temp = receive_message();
	
	get_console_chars(temp);
	temp = receive_message(); //assuming KB iProcess sends evn back to process
	
	if(strcmp(temp->messageText, " ")==0)
	{
		get_console_chars(temp);
		temp = receive_message();
	}
	else if(strcmp(temp->messageText, "s")==0)
	{
		send_message((int)(PIDUserProcessA), temp);
		release_processor(); 
	}
	else if(strcmp(temp->messageText, "ps")==0)
	{
		request_process_status(temp);
		send_console_chars(temp); //we need to figure out where the status info get put into a table format. Here??
		temp = receive_message();
	}
	
	else if(strcmp(temp->messageText, "cd")==0)
	{
		//change flag of wall clock to send time to CRT every second
	}
	else if(strcmp(temp->messageText, "ct")==0)
	{
		//change flag of wall clock to stop sending time to CRT
	}
	else if(temp->messageText[0] == 'c') 
	{
		sscanf(temp->messageText, "%c %d:%d:%d", &tempChar, &hour, &min, &sec); //needs to check if correct number of items passed
		if(hour < 0 || hour >=24 || min < 0 || min >=60 || sec < 0 || sec >= 60)
		{
			strcpy(temp->messageText, "Illegal Time Entered");
			send_console_chars(temp);
			temp = receive_message();
		}
		else
		{
			wall_clock(hour,min,sec); //We should make wall clock accept these values 
		}
	}
	else if(strcmp(temp->messageText, "b")==0)
	{
		get_trace_buffers(temp);
		send_console_chars(temp);
		temp = receive_message();
	}
	else if(strcmp(temp->messageText, "t")==0)
	{
		terminate();
	}
	else 	if(temp->messageText[0] == 'n')
	{
		sscanf(temp->messageText, "%c &d &d", &tempChar, &newPri, &PID); //needs to check if correct number of items passed
		int check = change_priority(newPri, PID);
		if(check == -1)
		{
			strcpy(temp->messageText, "Priority Change Failed");
			send_console_chars(temp);
			temp = receive_message();
		}
	}
	else
	{
		strcpy(temp->messageText, "Illegal Command");
		send_console_chars(temp);
		temp = receive_message();
	}
	}
}

///////////////////// USER PROCESSES HELPER FUNCTIONS ////////////
int isEmpty(struct CQueue* tempQ){
    if(tempQ == NULL)
        return -1;
    else if(tempQ->ptrMessageInboxHead == NULL)
        return 1;
    else
        return 0;
}
struct messageEnvelope* CDequeue(struct CQueue* tempQ){
    struct messageEnvelope* returnEnv;
    //NULL Queue Case
    if (tempQ == NULL){
       printf("CDequeue: No Queue passed\n");
	   return returnEnv;
    }
    //Empty Queue Case
    else if(tempQ->ptrMessageInboxHead==NULL){
       printf("CDequeue: Dequeue requested on empty Queue.\n");
       return returnEnv;
    }
    //Standard Case
    else{
       returnEnv = tempQ->ptrMessageInboxHead;
       //Queue of length 1 case
       if(tempQ->ptrMessageInboxHead == tempQ->ptrMessageInboxTail)
          tempQ->ptrMessageInboxTail = NULL;
       //Move the head pointer down the line
       tempQ->ptrMessageInboxHead = returnEnv->ptrNextMessage;
       returnEnv->ptrNextMessage = NULL;
       return returnEnv;
    }
}


void CEnqueue(struct CQueue* tempQ, struct messageEnvelope* newEnv){
     //Null Q case.
     if (tempQ == NULL)
        printf("\nA NULL Queue was passed to CEnqueue. ERROR.\n");
             
     newEnv->ptrNextMessage = NULL;
     //Empty Q case
     if (tempQ->ptrMessageInboxHead == NULL){
        tempQ->ptrMessageInboxHead = newEnv;
        tempQ->ptrMessageInboxTail = newEnv;
     }
     //Standard Q Case
     else{
        tempQ->ptrMessageInboxTail->ptrNextMessage = newEnv;
        tempQ->ptrMessageInboxTail = newEnv;
     }
}




