#include "Struct.h"
#include "Processes.h"
/*This code should not work. I am uncertain of how we will manage the enqueuing 
and dequeuing of PCBs when the 4 state queues' head and tail are in no way linked.
I do know, however, that we require: general FIFO enqueue and dequeue functions, a special
dequeue of any PCB element iterative function, and one insertion-sort msg env function.
*/

int Enqueue(PCB* ptr,desired Queue){//general enqueue function

	if(head == NULL){//Queue empty case
		ptrPCBBlockedHead = ptr;
		ptrPCBBlockedTail = ptr;
	}
	tail->ptrNextPCBQueue = ptr;
	tail = ptr;
	tail->ptrNextPCBQueue = NULL;
	}



PCB* Dequeue(){

	if(head == NULL){//Queue empty case
		return NULL;
		}
	PCB* ptr = NULL;
	ptr = head;
	head = head->ptrNextPCBQueue;
	return ptr;
}


/*The following code can be used for the PCBTimingList as well,
as PCB elements must be able to be dequeued in any order*/
PCB* BlockedOnReceiveDequeue(int PIDSender){//this should work...
	
	PCB* ptr = ptrPCBBlockedReceive;
	PCB* temp = NULL;
		if(ptr == NULL){//special case: Queue empty case
		return NULL;
		}
		if(ptr->PID == PIDSender){//special case: first element
			ptrPCBBlockedReceive = ptrPCBBlockedReceive->ptrNextPCBQueue;
		return ptr;
		}
		while (ptr->ptrNextPCBQueue){//general case iteration
			if(ptr->ptrNextPCBQueue->PID == PIDSender){
				temp = ptr->ptrNextPCBQueue;
				ptr->ptrNextPCBQueue = ptr->ptrNextPCBQueue->ptrNextPCBQueue;
				if(ptr->ptrNextPCBQueue == NULL){//special case: last element
					ptrPCBBlockedReceiveTail = ptr;
					}
				return temp;
				}
			ptr = ptr->ptrNextPCBQueue;
			}
	return NULL;//if element is not in Queue, bitch!
}
		
/*The TimerList is responsible for the insertion sort of the sleep
request envelopes send to the Alarm iProc*/
int TimingListEnqueue(messageEnvelope* env){
	if (env != NULL){
	messageEnvelope* ptr = NULL;
	ptr = ptrTimingList;
	
	if (env->ticks < ptr->ticks){//case 1: front of list insertion
		ptrTimingList = env;
		env->ptrNextMessage = ptr;
	}
	env->ticks -= ptr->ticks;
	while (ptr->ptrNextMessage){//this while condition sucks
		if (env->ticks < ptr->ptrNextMessage->ticks){//will tail insertion work?
			env->ptrNextMessage = ptr->ptrNextMessage;
			ptr->ptrNextMessage = env;
			}
		ptr = ptr->ptrNextMessage;
		env->ticks -= ptr->ticks;
	}
	ptr = env->ptrNextMessage;
	do{
	ptr->ticks -= env->ticks;
	ptr = ptr->ptrNextMessage;	
	}while (ptr);//decrements all following envs
		
	}
}













		
		

		

