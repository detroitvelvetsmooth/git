#include "Struct.h"
#include "Processes.h"

struct messageEnvelope* k_request_message_env( )
{
 struct messageEnvelope* temp;
		/*
		while(ptrMessage == NULL)
		{
		  ptrCurrentExecuting->PCBState = blocked on allocate;
		  enqueue current process onto blocked on allocate queue;
		  k_process_switch();
     }
		*/
    //temp = (struct messageEnvelope *)malloc(sizeof(struct messageEnvelope ));
    
    temp = ptrMessage;
    
    if(temp->ptrNextMessage==NULL)
     ptrMessageTail= NULL;
    
    ptrMessage = ptrMessage->ptrNextMessage;
		return temp;
}


int  k_release_message_env ( struct messageEnvelope* temp )
{
    if(temp == NULL)
		return -1; 
     //messageEnvelope *temp;
     //temp = Env; //(messageEnvelope *)malloc(sizeof( messageEnvelope ));    
    	 if(ptrMessage == NULL)
	     {
	         ptrMessage = temp;
	         ptrMessageTail = temp;
	         temp->ptrNextMessage = NULL;
	         
	     }
	     else
	     {
	         ptrMessageTail->ptrNextMessage = temp; 
	         ptrMessageTail = temp;
	         temp->ptrNextMessage = NULL;
	         
	     }
    /*
		if a process is on the blocked-on-allocate queue
			then dequeue from blocked-on-allocate queue
			set PCB state = ready
			enqueue process onto ready queue
		endif
	*/
		return 1;
}

int k_send_message( int dest_process_id, struct messageEnvelope* temp )
{	

	if(temp==NULL)
	{ printf("k_send_message: No envelope passed\n");
		return -1; 
	}
	
	    temp->PIDSender = ptrCurrentExecuting->PID;
			temp->PIDReceiver = dest_process_id;
		
		struct PCB *receiver;
        receiver = getPCB(dest_process_id);
        
         if(receiver->ptrMessageInboxHead == NULL)
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
        /*
		store the sender, receiver, and time information of env into trace struct
		if state of receiving process == blocked on receive 
			then change state of receiving process to ready 
			move process from blocked queue onto ready queue
		if env type = = wakeup
			then change state of sleeping process to ready
			move process from sleep queue to ready queue
		endif
		*/
		return 0;
}

struct messageEnvelope* k_receive_message( )
{
   struct messageEnvelope *temp;
    /*
    if(ptrCurrentExecuting->ptrInboxMessageHead == NULL)
        if(ptrCurrentExecuting->PID == IPROCESS)
            return NULL;
        ptrCurrentExecuting->PIDState = BLOCKED ON RECEIVE
			move current process to blocked on receive queue
			k_process_switch();
		endif
	*/
		
    //temp = (stuct messageEnvelope*)malloc(sizeof( struct messageEnvelope ));
    
    if(ptrCurrentExecuting->ptrMessageInboxHead->ptrNextMessage==NULL)
    {
    	ptrCurrentExecuting->ptrMessageInboxTail = NULL;
    }
    
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
    temp->processPriority = new_priority;
    /*
		if target_process is on ready queue
			dequeue from old priority queue
			enqueue to new priority queue
	*/
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

struct PCB* BlockedOnReceiveDequeue(int PIDTo){//this should work...
	struct nodePCB* Q;
	Q = ptrPCBBlockedReceive;
	struct PCB* returnPCB;
    struct PCB* temp;
    returnPCB = Q->queueHead;
		if(Q->queueHead == NULL){//special case: Queue empty case
             return NULL;
		}
		if(Q->queueHead->PID == PIDTo){//special case: first element
			Q->queueHead = Q->queueHead->ptrNextPCBQueue;
			if(Q->queueHead == NULL)
                Q->queueTail = NULL;
            return returnPCB;
		}
		if(Q->queueHead == Q->queueTail)
            return NULL; //Q of length 1.
        temp = returnPCB;
		while (!(temp->ptrNextPCBQueue==NULL)){//general case iteration
			if(temp->ptrNextPCBQueue->PID == PIDTo){ //Found the node
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


