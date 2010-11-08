int Enqueue(struct PCB* ptr,struct PCBQueue* Q){//general enqueue function
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

struct PCB* Dequeue(struct PCBQueue* Q){
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
	struct PCBQueue* Q;
	Q = BlockedOnReceiveQ;
	struct PCB* returnPCB;
    struct PCB* temp;
    returnPCB = Q->queueHead;
		if(Q->queueHead == NULL){//special case: Queue empty case
             return NULL;
		}
		if(Q->queueHead->PID == PIDTo){//special case: first element
			Q->queueHead = Q->head->ptrNextPCBQueue;
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


