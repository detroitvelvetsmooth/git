#include "Struct.h"
#include <stdio.h>
#include <string.h>


///////////// AUXILLARY FUNCTIONS PROTOTYPES //////////////////

// THESE ARE DEFINED AT THE BOTTOM OF THE FILE. 

void context_switch(struct PCB* next_PCB);
void atomic(int on);
struct PCB * getPCB(int findPID);
int Enqueue(struct PCB* ptr,struct nodePCB* Q);
struct PCB* Dequeue(struct nodePCB* Q);
struct PCB* SearchPCBDequeue(int searchPID, struct nodePCB* Q);
struct PCB* ReadyProcessDequeue();
void add_to_traceBuffer(int PIDsender, int PIDreceiver, int msgType, int traceBuffer);//traceBuffer of 0 will be send, 1 will be receive

///////////// KPRIMITIVES ///////////////////

void k_process_switch(){

 struct	PCB* next_PCB = ReadyProcessDequeue(); //finds the next pcb to execute with the highest priority.
	next_PCB->PCBState = EXECUTING;
	//printf("k_process_switch: Switching To: %s\n", debugProcessName(next_PCB->PID));
	//ptrCurrentExecuting = next_PCB; // CONTEXT SWITCH ONLY WORKS WHEN WE PASS IN PTRCURRENTEXECUTING BECAUSE THATS HOW THE CONTEXT WAS INITIALIZED.
																	
	context_switch(next_PCB);// SINCE PTRCURRENTEXECUTING IS A GLOBAL PARAMETER WE DON'T NEED TO PASS IT IN. HOWEVER WE MUST MAKE SURE THAT IT IS POINTING TO WHAT WE WANT TO EXECUTE.
}
	
struct messageEnvelope* k_request_message_env( ){

    struct messageEnvelope* temp;

    while(ptrMessage == NULL){ // No envelopes available
    
	  ptrCurrentExecuting->PCBState = BLOCKED_MSG_ALLOCATE;

      int result = Enqueue(ptrCurrentExecuting, ptrPCBBlockedAllocate);
      if(result !=1)
      printf("In k_request_message_env: Enqueue function failed to return correctly\n");
      
      
      k_process_switch(); 
    }
   
    temp = ptrMessage;

    if(temp->ptrNextMessage==NULL)
          ptrMessageTail= NULL;

    ptrMessage = ptrMessage->ptrNextMessage;
    return temp;
}


int  k_release_message_env ( struct messageEnvelope* temp ){

    if(temp == NULL) //No envelope
         return -1;
    if(ptrMessage == NULL) //Empty Message linked list
    {
         ptrMessage = temp;
         ptrMessageTail = temp;
         temp->ptrNextMessage = NULL;
    }
    else //Normal Case
    {
         ptrMessageTail->ptrNextMessage = temp;
         ptrMessageTail = temp;
         temp->ptrNextMessage = NULL;
    }
    if(ptrPCBBlockedAllocate->queueHead != NULL) //Processes are blocked on allocate
    {
         struct PCB* movingPCB;
         movingPCB = Dequeue(ptrPCBBlockedAllocate); //Grab PCB from Q.
         if(movingPCB == NULL){
             printf("Inside k_release_message_env: The PCB that was intended to be sent to ready was not properly dequeued from the blocked on allocate Q. ERROR.\n");
         }
         movingPCB->PCBState = READY;  //Change PCB's state to ready
         int result;
         if(movingPCB->processPriority == HIGH_PRIORITY)  //Enqueue the PCB to its proper ready Q.
             result = Enqueue(movingPCB, ptrPCBReadyHigh);
         else if(movingPCB->processPriority == MED_PRIORITY)
             result = Enqueue(movingPCB, ptrPCBReadyMed);
         else if(movingPCB->processPriority == LOW_PRIORITY)
             result = Enqueue(movingPCB, ptrPCBReadyLow);
         else
             result = Enqueue(movingPCB, ptrPCBReadyNull);
         if(result != 1)
         {
             printf("Inside k_release_message_env: The PCB that was intended to be sent to ready was not properly enqueued. ERROR.\n");
         }
    }
    return 1;
}

int k_send_message( int dest_process_id, struct messageEnvelope* temp ){

	if(temp==NULL) //No Envelope Passed.
	{ printf("k_send_message: No envelope passed\n");
		return -1;
	}

	temp->PIDSender = ptrCurrentExecuting->PID;
	temp->PIDReceiver = dest_process_id;


	struct PCB *receiver;
        receiver = getPCB(dest_process_id);
        /*
        printf("\nk_send_message: Receiver PID : %s\n", debugProcessName(receiver->PID));
				printf("k_send_message: Sender: %s\n", debugProcessName(temp->PIDSender));
				printf("k_send_message: Receiver: %s\n", debugProcessName(temp->PIDReceiver));
				printf("k_send_message: Contents: %s\n", temp->messageText);
				printf("k_send_message: MessageType: %s\n\n", debugMessageType(temp->messageType));
				*/

        if(receiver->ptrMessageInboxHead == NULL) //Empty Inbox
        {
            receiver->ptrMessageInboxHead = temp;
            receiver->ptrMessageInboxTail = temp;
		    temp->ptrNextMessage = NULL;
        }
        else
        {
            receiver->ptrMessageInboxTail->ptrNextMessage = temp;
            receiver->ptrMessageInboxTail = temp;
            temp->ptrNextMessage = NULL;
        }
        struct PCB* tempPCB;
        if(receiver->PCBState == BLOCKED_MSG_RECEIVE || (receiver->PCBState == BLOCKED_SLEEPING && temp->messageType == MSGTYPEWAKEUP)){
        //Note: BLOCKED_SLEEPING and BLOCKED_MSG_RECEIVE both reside in the ptrPCBBlockedReceive Q)
            tempPCB = SearchPCBDequeue(receiver->PID, ptrPCBBlockedReceive);
            if (tempPCB == NULL){
                printf("Inside k_send_message: The receiving process is in BLOCKED_MSG_RECEIVE state but not found on Q. ERROR.\n");
            }
            else if(tempPCB != receiver){
                printf("Inside k_send_message: The dequeued PCB from the Blocked on Receive Q is not = to the receiving PCB. ERROR. \n");
            }
            else{
            
            tempPCB->PCBState = READY;
            
                int result;
                if(tempPCB->processPriority == HIGH_PRIORITY)
                    result = Enqueue(tempPCB, ptrPCBReadyHigh);
                else if(tempPCB->processPriority == MED_PRIORITY)
                    result = Enqueue(tempPCB, ptrPCBReadyMed);
                else if(tempPCB->processPriority == LOW_PRIORITY)
                    result = Enqueue(tempPCB, ptrPCBReadyLow);
                else
                    result = Enqueue(tempPCB, ptrPCBReadyNull);
                if(result != 1){
                    printf("Inside k_send_message: The Enqueue function on the receiving PCB to its ready Q did not return 1. ERROR. \n");
                }
            }
        }

	//Add sender, receiver, message type information to trace buffer. 
	add_to_traceBuffer(temp->PIDSender, temp->PIDReceiver, temp->messageType, (int) 0); //Last parameter (0) is to add to send buffer
	return 0;
}

struct messageEnvelope* k_receive_message( )
{/*
       struct PCB* debug =NULL;
        printf("\n**********Current********\n");
       printf("Curent Exeucting : %s\n",debugProcessName(ptrCurrentExecuting->PID));
       
        printf("**********Queues********\n"); 
    	 debug =	ptrPCBReadyHigh->queueHead;
      	 while(debug !=NULL)
       	{
   		printf("PID HIGH: %s\n",debugProcessName( debug->PID)); 
   		debug = debug->ptrNextPCBQueue;
     	}
     	 debug =	ptrPCBReadyMed->queueHead;
      	 while(debug !=NULL)
       	{
   		printf("PID MED: %s\n",debugProcessName( debug->PID)); 
   		debug = debug->ptrNextPCBQueue;
     	}
     	 debug =	ptrPCBReadyLow->queueHead;
      	 while(debug !=NULL)
       	{
   		printf("PID LOW: %s\n",debugProcessName( debug->PID)); 
   		debug = debug->ptrNextPCBQueue;
     	}
     	 debug =	ptrPCBReadyNull->queueHead;
      	 while(debug !=NULL)
       	{
   		printf("PID NULL: %s\n",debugProcessName( debug->PID)); 
   		debug = debug->ptrNextPCBQueue;
     	}
     	
     	
     	printf("***********BLOCKED****************\n");
         debug =	ptrPCBBlockedReceive->queueHead;
      	 while(debug !=NULL)
       	{
   		printf("PID BLOCKED: %s\n",debugProcessName( debug->PID)); 
   		debug = debug->ptrNextPCBQueue;
     	}
     	if(ptrPCBBlockedReceive->queueTail!=NULL)
     	printf("Tail Blocked: %s\n", debugProcessName(ptrPCBBlockedReceive->queueTail->PID));
     	printf("***************************\n");
   	
   	
	*///printf("k_receive_Message: Receiving Process : %s\n", debugProcessName(ptrCurrentExecuting->PID));
   struct messageEnvelope *temp; 
   while(ptrCurrentExecuting->ptrMessageInboxHead == NULL)
   {
       if(ptrCurrentExecuting->PCBState == IPROCESS) //Iprocesses don't block
           { 
               //printf("k_receive_message: Following process was not Blocked: %s\n",debugProcessName(ptrCurrentExecuting->PID));
               return NULL;
           }
     //  printf("k_receive_message: Following process to be placed on Blocked Queue: %s\n",debugProcessName(ptrCurrentExecuting->PID));
       if(ptrCurrentExecuting->PCBState != BLOCKED_SLEEPING) //IF IT HAS BEEN SLEEPING YOU DON'T WANT TO OVERWRITE THE STATE. 
       ptrCurrentExecuting->PCBState = BLOCKED_MSG_RECEIVE; //Change state
       
       int result = Enqueue(ptrCurrentExecuting, ptrPCBBlockedReceive);
       
       if (result != 1)
           printf("Inside k_receive_message_env: The invoking PCB is meant to be blocked. The Enqueue function was unable to enqueue the PCB to the Blocked on Receive Q.\n");
       
       k_process_switch();//Call Process_switch();
   }   
   
    if(ptrCurrentExecuting->ptrMessageInboxHead->ptrNextMessage==NULL) //Inbox is size 1
     	ptrCurrentExecuting->ptrMessageInboxTail = NULL;
     
    temp = ptrCurrentExecuting->ptrMessageInboxHead;
    ptrCurrentExecuting->ptrMessageInboxHead = ptrCurrentExecuting->ptrMessageInboxHead->ptrNextMessage;
	temp->ptrNextMessage = NULL;
	//Store the sender, receiver, msgType to trace buffer;
	add_to_traceBuffer(temp->PIDSender, temp->PIDReceiver, temp->messageType, (int) 1); // Last paramater (1) is to add it to receive buffer
    return temp;

}

int  k_get_console_chars( struct messageEnvelope * temp )
{
        if(temp == NULL)
            return -1;
           
		k_send_message((int)PIDiProcessKeyboard,temp);
		return 0;
}

int  k_send_console_chars(struct messageEnvelope * temp )
{
		if(temp == NULL)
			return -1;
  	    
    
		k_send_message((int)PIDiProcessCRT, temp);
	//	kill(getpid(), SIGUSR1);
		iProcessCRT();
		return 0;
}

int  k_release_processor( )
{
	ptrCurrentExecuting->PCBState = READY; //Ready
	
	if(ptrCurrentExecuting->processPriority == HIGH_PRIORITY)	//enqueue current process to ready queue
	Enqueue(ptrCurrentExecuting,ptrPCBReadyHigh);
	
	else if(ptrCurrentExecuting->processPriority == MED_PRIORITY)
	Enqueue(ptrCurrentExecuting,ptrPCBReadyMed);
	
	else if(ptrCurrentExecuting->processPriority == LOW_PRIORITY)
	Enqueue(ptrCurrentExecuting,ptrPCBReadyLow);
	
	else if(ptrCurrentExecuting->processPriority == NULL_PRIORITY)
	Enqueue(ptrCurrentExecuting,ptrPCBReadyNull);
	

	k_process_switch();
	return 1;
}

struct messageEnvelope*  k_request_process_status(struct messageEnvelope * temp ) //THIS FUNCTION WILL GATHER THE LIST OF PCBS AND PRINT THEIR PRIORITY, THEIR STATUS AND THEIR PID. 
{
    if(temp == NULL)
        return NULL;

	int PID, Priority, State;
	char ps[80]; //the string to be populated

    strcpy(ps, "PID\tPri\tState\n"); //MAKES THE HEADERS FOR THE COLUMNS.
    
    struct PCB *process; //temporary PCB that loops through
    process = ptrPCBList; //initializes loop pointing to the first PCB

    while(process != NULL)
    {
		char tempPS[6]; //I DON'T KNOW IF THIS SIZE IS RIGHT. EITHER WAY IT WILL BE RESIZED BY SPRINTF. 
		PID = process->PID;
		Priority = process->processPriority;
		State = process->PCBState;
		sprintf(tempPS, "%d\t%d\t%d\n", PID, Priority, State);
		strcat(ps, tempPS);
		
      	  process = process->ptrNextPCBList;
    }
	strcpy(temp->messageText, ps); //TODO - WARNING. strcpy may resize our messageText if ps is too long.

   return temp;		//RETURNS THE MESSAGE WITH THE POPULATED DATA.
}

int  k_change_priority(int new_priority, int targetID){
	
    if (!(targetID == PIDUserProcessA || targetID == PIDUserProcessB || targetID == PIDUserProcessC
		|| targetID == PIDcci || targetID == PIDNullProcess || targetID == PIDiProcessKeyboard || targetID == PIDWallClock
		|| targetID == PIDiProcessCRT || targetID == PIDiProcessTimer || new_priority == HIGH_PRIORITY
		||new_priority == MED_PRIORITY || new_priority == LOW_PRIORITY || new_priority == NULL_PRIORITY))
		return -1;
	
    struct PCB *temp;
    temp = getPCB(targetID);
    //Since PCB is in a ready Q it must be changed immediately.
    if(temp->PCBState == READY){
	  struct PCB* movingPCB;
          if(temp->processPriority == HIGH_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyHigh);
          else if(temp->processPriority == MED_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyMed);
          else if(temp->processPriority == LOW_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyLow);
          else if(temp->processPriority == NULL_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyNull);
          if(movingPCB != temp){
              printf("Inside k_change_priority: The proper PCB was not returned by the SearchPCBDequeue function. ERROR.\n");
          }
          int result;
          if(new_priority == HIGH_PRIORITY)
              result = Enqueue(movingPCB, ptrPCBReadyHigh);
          else if(new_priority == MED_PRIORITY)
              result = Enqueue(movingPCB, ptrPCBReadyMed);
          else if(new_priority == LOW_PRIORITY)
              result = Enqueue(movingPCB, ptrPCBReadyLow);
          else
              result = Enqueue(movingPCB, ptrPCBReadyNull);
          if(result != 1){
              printf("Inside k_change_priority: The PCB was not properly enqueued to its ready Q. ERROR./n");
          }
    }
    temp->processPriority = new_priority;
    return 1;
}

int  k_request_delay( int delay, struct messageEnvelope * temp )// TODO THIS FUNCTION IS INCOMPLETE.
{
     if(temp == NULL)
        return -1;
        
    temp->messageType = MSGREQUESTDELAY;
    temp->sleepTicks = delay;
    ptrCurrentExecuting->PCBState = BLOCKED_SLEEPING; //changes the state to sleeping
	
	k_send_message((int)PIDiProcessTimer,temp); 
	return 1;
}

int k_terminate()
{
    cleanup();
    return 1;
}

int k_get_trace_buffers( struct messageEnvelope * temp){
	/*if(temp == NULL)
		return -1;
	char bufferData[32];
	int i, j, k;
	k = 0;
	for(i=0; i<16; i++)
	{
		for(j=0; j<3; j++)
		{
			bufferData[k] = sendTraceBuffer[i][j];
			bufferData[k+16] = receiveTraceBuffer[i][j];
			k++;
		}
	}
	strcpy(temp->messageText, bufferData); //we have to decide where this gets put into a table format. CRT iprocess?
	*/
	return 1;
}
			
/////////////PRIMITIVE HELPER FUNCTIONS //////////////////////

void context_switch(struct PCB* next_PCB){
	int return_code = setjmp(ptrCurrentExecuting->contextBuffer);
	
	if(return_code == 0){
	ptrCurrentExecuting = next_PCB;
	longjmp(ptrCurrentExecuting->contextBuffer,1);//will it work on next_PCB's 1st execution? IT SHOULD NOW. 
	}
}

void atomic(int on) {
     /*
    static sigset_t oldmask;
    sigset_t newmask;
    if (on) {
       atomic_count++;
       if (atomic_count == 1) { //Check to see if atomic isn't already on
          sigemptyset(&newmask); //Initialize Newmask. Add appropriate signals to mask.
          sigaddset(&newmask, 14); //the alarm signal
          sigaddset(&newmask, 2); // the CNTRL-C
          sigaddset(&newmask, SIGUSR1);
          sigaddset(&newmask, SIGUSR2);
          sigprocmask(SIG_BLOCK, &newmask, &oldmask); //Oldmask saves the signals being blocked
       }
    }
    else {
         atomic_count--;
         if (atomic_count == 0) { //Check to see if atomic can be turned off
            sigprocmask(SIG_SETMASK, &oldmask, NULL);
         }
    }
    */
}

struct PCB * getPCB(int findPID)
{
    if(ptrPCBList->PID == findPID)
            return ptrPCBList;

    struct PCB *temp;

    temp = ptrPCBList;
    int done =0;
    while(done ==0)
    {
        temp = temp->ptrNextPCBList;
        if(temp->PID == findPID)
            done = 1;
    }
    return temp;
}

int Enqueue(struct PCB* ptr,struct nodePCB* Q){//general enqueue function
    ptr->ptrNextPCBQueue = NULL;
   	if(Q->queueHead == NULL){//Queue empty case
		Q->queueHead = ptr;
		Q->queueTail = ptr;
}
	else{
	    
	     Q->queueTail->ptrNextPCBQueue = ptr;
       Q->queueTail = ptr;
    }
	return 1;
}

struct PCB* Dequeue(struct nodePCB* Q){

	if(Q->queueHead == NULL)//Queue empty case
		return NULL;


	struct PCB* returnPCB;
	returnPCB = Q->queueHead;

	if(Q->queueHead == Q->queueTail)
        Q->queueTail = NULL;

    Q->queueHead = returnPCB->ptrNextPCBQueue;
    returnPCB->ptrNextPCBQueue = NULL;

	return returnPCB;
}

struct PCB* SearchPCBDequeue(int searchPID, struct nodePCB* Q){
    struct PCB* returnPCB;
    struct PCB* temp;
    returnPCB = Q->queueHead;
		if(Q->queueHead == NULL){//special case: Queue empty case
             return NULL;
		}
		if(Q->queueHead->PID == searchPID){//special case: first element
			Q->queueHead = Q->queueHead->ptrNextPCBQueue;
			
			if(Q->queueHead == NULL)
                Q->queueTail = NULL; //LIST IS NOW EMPTY
            return returnPCB;
		}
		
		if(Q->queueHead == Q->queueTail)
            return NULL; //Q of length 1. AND RESULT WAS NOT FOUND
            
        temp = Q->queueHead;
		while (!(temp->ptrNextPCBQueue==NULL)){//general case iteration
		
			if(temp->ptrNextPCBQueue->PID == searchPID){ //Found the node
			    returnPCB=temp->ptrNextPCBQueue; //Node to return
			    
   				if(temp->ptrNextPCBQueue->ptrNextPCBQueue == NULL){//special case: last element
					Q->queueTail = temp;
				}
				temp->ptrNextPCBQueue = temp->ptrNextPCBQueue->ptrNextPCBQueue; //Jump node
				return returnPCB;
			}
			temp = temp->ptrNextPCBQueue;
			}
	return NULL;//if element is not in Queue.
}

struct PCB* ReadyProcessDequeue(){//Chuy, is there a better way to 'if' this? BRIAN: No.

	struct PCB* ptr = NULL;

	ptr = Dequeue(ptrPCBReadyHigh);
	if (ptr != NULL){
		ptr->ptrNextPCBQueue = NULL;
		return ptr;
	}
	ptr = Dequeue(ptrPCBReadyMed);
	if (ptr != NULL){
		ptr->ptrNextPCBQueue = NULL;
		return ptr;
	}
	ptr = Dequeue(ptrPCBReadyLow);
	if (ptr != NULL){
		ptr->ptrNextPCBQueue = NULL;
		return ptr;
	}
	ptr = Dequeue(ptrPCBReadyNull);
	ptr->ptrNextPCBQueue = NULL;
	return ptr;
}

void add_to_traceBuffer(int PIDSender, int PIDReceiver, int msgType, int traceBufferNumber){
	if(traceBufferNumber == 0){//Add to send traceBuffer
		sendTraceBuffer->data[sendTraceBuffer->tail][0] = PIDSender;
		sendTraceBuffer->data[sendTraceBuffer->tail][1] = PIDReceiver;
		sendTraceBuffer->data[sendTraceBuffer->tail][2] = msgType;
		sendTraceBuffer->data[sendTraceBuffer->tail][3] = traceBufferNumber;

		sendTraceBuffer->tail ++; //Shift tail down now
		sendTraceBuffer->tail = sendTraceBuffer->tail % 16; //If tail is outside trace buffer limits, set back to 0.
		if(sendTraceBuffer->head == sendTraceBuffer->tail){ //This will always be the case on a full trace buffer
			sendTraceBuffer->head ++; //Move head along, since old head was replaced by new entry
			sendTraceBuffer->head = sendTraceBuffer->head % 16; //If head is outside trace buffer limits, set back to 0.
		}
	}
	else{

		receiveTraceBuffer->data[receiveTraceBuffer->tail][0] = PIDSender;
		receiveTraceBuffer->data[receiveTraceBuffer->tail][1] = PIDReceiver;
		receiveTraceBuffer->data[receiveTraceBuffer->tail][2] = msgType;
		receiveTraceBuffer->data[receiveTraceBuffer->tail][3] = traceBufferNumber;
		receiveTraceBuffer->tail ++; //Shift tail down now
		receiveTraceBuffer->tail = receiveTraceBuffer->tail % 16; //If tail is outside trace buffer limits, set back to 0.
		if(receiveTraceBuffer->head == receiveTraceBuffer->tail){ //This will always be the case on a full trace buffer
			receiveTraceBuffer->head ++; //Move head along, since old head was replaced by new entry
			receiveTraceBuffer->head = receiveTraceBuffer->head % 16; //If head is outside trace buffer limits, set back to 0.
		}
	}
}
