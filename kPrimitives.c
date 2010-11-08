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

void atomic(int on)
{
on = on +1; 
// code for the atomic. 

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


