#include "Struct.h"

struct messageEnvelope* k_request_message_env( )
{
    struct messageEnvelope* temp;
    
    if(ptrMessage == NULL) // No envelopes available
    {
	  ptrCurrentExecuting->PCBState = BLOCKED_MSG_ALLOCATE;
	  struct PCB* movingPCB;
          if(ptrCurrentExecuting->processPriority == HIGH_PRIORITY)
              movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyHigh);
          elseif(ptrCurrentExecuting->processPriority == MED_PRIORITY)
              movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyMed);
          elseif(ptrCurrentExecuting->processPriority == LOW_PRIORITY)
              movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyLow);
          else(ptrCurrentExecuting->processPriority == NULL_PRIORITY)
              movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyNull);
          if(movingPCB != ptrCurrentExecuting){
              printf("Inside k_request_message_env: The proper PCB was not returned by the SearchPCBDequeue function. ERROR.\n");
          }
          int result = Enqueue(movingPCB, ptrPCBBlockedAllocate);
          return NULL; //TODO: Change this once k_process_switch() works.
	  //k_process_switch();
    }
    //temp = (struct messageEnvelope *)malloc(sizeof(struct messageEnvelope ));
    
    temp = ptrMessage;
    
    if(temp->ptrNextMessage==NULL)
          ptrMessageTail= NULL;
    
    ptrMessage = ptrMessage->ptrNextMessage;
    return temp;
}


int  k_release_message_env ( struct messageEnvelope* temp )
{
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
         elseif(movingPCB->processPriority == MED_PRIORITY)
             result = Enqueue(movingPCB, ptrPCBReadyMed);
         elseif(movingPCB->processPriority == LOW_PRIORITY)
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

int k_send_message( int dest_process_id, struct messageEnvelope* temp )
{	

	if(temp==NULL) //No Envelope Passed.
	{ printf("k_send_message: No envelope passed\n");
		return -1; 
	}
	
	temp->PIDSender = ptrCurrentExecuting->PID;
	temp->PIDReceiver = dest_process_id;
		
	struct PCB *receiver;
        receiver = getPCB(dest_process_id);
        
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
        if(receiver->PCBState == BLOCKED_MSG_RECEIVE || (receiver->PCBState == BLOCKED_SLEEPING && temp->messageType == MSGTYPEWAKEUP){
        //Note: BLOCKED_SLEEPING and BLOCKED_MSG_RECEIVE both reside in the ptrPCBBlockedReceive Q)
            tempPCB = SearchPCBDequeue(receiver->PID, ptrPCBBlockedReceive);
            if (tempPCB == NULL){
                printf("Inside k_send_message: The receiving process is in BLOCKED_MSG_RECEIVE state but not found on Q. ERROR.\n");
            }
            elseif(tempPCB != receiver){
                printf("Inside k_send_message: The dequeued PCB from the Blocked on Receive Q is not = to the receiving PCB. ERROR. \n");
            }
            else{
                int result;
                if(tempPCB->processPriority == HIGH_PRIORITY)
                    result = Enqueue(tempPCB, ptrPCBReadyHigh);
                elseif(temp->processPriority == MED_PRIORITY)
                    result = Enqueue(tempPCB, ptrPCBReadyMed);
                elseif(temp->processPriority == LOW_PRIORITY)
                    result = Enqueue(tempPCB, ptrPCBReadyLow);
                else
                    result = Enqueue(tempPCB, ptrPCBReadyNULL);
                if(result != 1){
                    printf("Inside k_send_message: The Enqueue function on the receiving PCB to its ready Q did not return 1. ERROR. \n");
                }
            }
        }       
            
        /*
		store the sender, receiver, and time information of env into trace struct
	*/
	return 0;
}

struct messageEnvelope* k_receive_message( )
{
   struct messageEnvelope *temp;
   if(ptrCurrentExecuting->ptrInboxMessageHead == NULL)
   {
       if(ptrCurrentExecuting->PCBState == IPROCESS) //Iprocesses don't block
           return NULL;
       ptrCurrentExecuting->PCBState = BLOCKED_MSG_RECEIVE; //Change state
/* I DON'T BELIEVE THIS IS NECESSARY AS THE INVOKING PROCESS WON'T BE ON A READY Q
       struct PCB* movingPCB;
       if(ptrCurrentExecuting->processPriority == HIGH_PRIORITY)
           movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyHigh);
       elseif(ptrCurrentExecuting->processPriority == MED_PRIORITY)
           movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyMed);
       elseif(ptrCurrentExecuting->processPriority == LOW_PRIORITY)
           movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyLow);
       else(ptrCurrentExecuting->processPriority == NULL_PRIORITY)
           movingPCB = SearchPCBDequeue(ptrCurrentExecuting->PID, ptrPCBReadyNull);
       if(movingPCB != ptrCurrentExecuting){
           printf("Inside k_receive_message_env: The proper PCB was not returned by the SearchPCBDequeue function. ERROR.\n");
*/
       int result = Enqueue(ptrCurrentExecuting, ptrPCBBlockedReceive);
       if (result != 1)
           printf("Inside k_receive_message_env: The invoking PCB is meant to be blocked. The Enqueue function was unable to enqueue the PCB to the Blocked on Receive Q.\n");
       //Call Process_switch();
   }
		
    //temp = (stuct messageEnvelope*)malloc(sizeof( struct messageEnvelope ));
    
    if(ptrCurrentExecuting->ptrMessageInboxHead->ptrNextMessage==NULL) //Inbox is size 1
    {
    	ptrCurrentExecuting->ptrMessageInboxTail = NULL;
    }
    
    //
    temp = ptrCurrentExecuting->ptrMessageInboxHead;
    ptrCurrentExecuting->ptrMessageInboxHead = ptrCurrentExecuting->ptrMessageInboxHead->ptrNextMessage;
    return temp;
    
   	//store the sender, receiver, and time information of env into trace struct
		
}

int  k_get_console_chars( struct messageEnvelope * temp )
{		
        if(temp == NULL)
            return -1;
        /*
		temp->messageType = keyboard input message
		*/
		k_send_message((int)PIDiProcessKeyboard,temp);
		return 0;
}

int  k_send_console_chars(struct messageEnvelope * temp )
{
		if(temp == NULL)
			return -1;
		/*
		temp->messageType = CRT output message;
		*/
		k_send_message((int)PIDiProcessCRT, temp);
		kill(getpid(), SIGUSR1);
		return 0;
}

void atomic(int on) {
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


/////////////////////////////////////// NEW. 
void  k_process_switch(){} //TO WRITE.

int  k_release_processor( )
{
	ptrCurrentExecuting->PCBState = 1; //Ready
	//enqueue current process to ready queue
	k_process_switch();
	return 1;
}


int  k_request_process_status(struct messageEnvelope * temp )
{
    if(temp == NULL)
        return 0;
    int i;
    int j;
    int k=0;
    int PCBstatus[numProcessesTotal][3]; //Make global variable #ofProcesses to replace '8'???
    struct PCB *process;
    process = ptrPCBList;
    while(process->ptrNextPCBList != NULL)
    {
        PCBstatus[i][0] = process->PID;
        PCBstatus[i][1] = process->PCBState;
        PCBstatus[i][2] = process->processPriority;
        i++;
        process = process->ptrNextPCBList;
    }
    for(i=0; i<8; i++)
    {
        for(j=0; j<3; j++)
        {
            temp->messageText[k] = PCBstatus[i][j];
            k++;
        }
    }
    return 1;
}

int  k_change_priority(int new_priority, int targetID){
    if (targetID != PIDUserProcessA || targetID != PIDUserProcessB || targetID != PIDUserProcessC 
		|| targetID != PIDcci || targetID != PIDNullProcess || targetID != PIDiProcessKeyboard 
		|| targetID != PIDiProcessCRT || targetID != PIDiProcessTimer || new_priority != HIGH_PRIORITY 
		||new_priority != MED_PRIORITY || new_priority != LOW_PRIORITY || new_priority != NULL_PRIORITY)
		return -1;
    struct PCB *temp;
    temp = getPCB(targetID);	
    //Since PCB is in a ready Q it must be changed immediately.
    if(temp->PCBState == READY){
	  struct PCB* movingPCB;
          if(temp->processPriority == HIGH_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyHigh);
          elseif(temp->processPriority == MED_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyMed);
          elseif(temp->processPriority == LOW_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyLow);
          else(temp->processPriority == NULL_PRIORITY)
              movingPCB = SearchPCBDequeue(targetID, ptrPCBReadyNull);
          if(movingPCB != temp){
              printf("Inside k_change_priority: The proper PCB was not returned by the SearchPCBDequeue function. ERROR.\n");
          }
          int result;
          if(new_priority == HIGH_PRIORITY)
              result = Enqueue(movingPCB, ptrPCBReadyHigh);
          elseif(new_priority == MED_PRIORITY)
              result = Enqueue(movingPCB, ptrPCBReadyMed);
          elseif(new_priority == LOW_PRIORITY)
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

int  k_request_delay( int delay, int wakeup_code, struct messageEnvelope * temp )
{
    if(temp == NULL)
        return -1;
    ptrCurrentExecuting->PCBState = BLOCKED_SLEEPING;
    //temp->messageType = MSGTYPEWAKEUP;
    //temp->sleepTicks = delay;
	temp->PIDSender = ptrCurrentExecuting->PID;
	//k_send_message( TODO , temp); //TODO timer delay i-process PID
	return 1;
}

int k_terminate()
{
    cleanup();
    return 1;
}

//////////////// PRIMITIVE SUPPORT FUNCTIONS /////////////////////////

int Enqueue(struct PCB* ptr,struct nodePCB* Q){//general enqueue function
    if(Q == NULL)
         return -1;
	if(Q->queueHead == NULL){//Queue empty case
		Q->queueHead = ptr;
		Q->queueTail = ptr;
	}
	else{
	     ptr->ptrNextPCBQueue = NULL;
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
                Q->queueTail = NULL;
            return returnPCB;
		}
		if(Q->queueHead == Q->queueTail)
            return NULL; //Q of length 1.
        temp = returnPCB;
		while (!(temp->ptrNextPCBQueue==NULL)){//general case iteration
			if(temp->ptrNextPCBQueue->PID == searchPID){ //Found the node
			    returnPCB=temp->ptrNextPCBQueue; //Node to return
				temp->ptrNextPCBQueue = temp->ptrNextPCBQueue->ptrNextPCBQueue; //Jump node
				if(temp->ptrNextPCBQueue == NULL){//special case: last element
					Q->queueTail = returnPCB;
				}
				return returnPCB;
				}
			temp = temp->ptrNextPCBQueue;
			}
	return NULL;//if element is not in Queue.
}
