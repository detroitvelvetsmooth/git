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


