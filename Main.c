#include "Struct.h"
#include "startProcesses.h"
#include "publicProcesses.h"

/*Struct.h will contain the struct definitions to be used. MSG envelopes and PCBs and any
other. Processes.h will contain the function prototypes for every process (user and iprocess) to be used.
*/

int main() {


    //Every major process will be split into separate files.
    
/////////////// TIMING IPROCESS /////////////
    
absoluteTime = 0;
relativeTime = 0; // the wallclock will initially commence at the same time as the absoluteTime
displayWallClock= 0;

atomic_count = 0;
ptrTimingList=NULL; // pointer to messages that have the timing countdown. 


///////////// GENERAL PCB LIST /////////////////////////

ptrCurrentExecuting = NULL; //will point to the currently executing Process. and used for context initialization.

ptrPCBList = NULL; //ptr that will link to the PCB list (which will remain somewhat static once initialized
ptrPCBListTail = NULL; //this tail ptr may or may not be required.

/////////////// PRIORITY QUEUES.///////////////

ptrPCBReadyNull = (struct nodePCB*) malloc(sizeof(struct nodePCB));
(*ptrPCBReadyNull).queueHead =NULL;
(*ptrPCBReadyNull).queueTail =NULL; // ptrs to the various priority queues for PCBs that are READY. for the nullPriority we will only have one process there.

ptrPCBReadyLow = (struct nodePCB*) malloc(sizeof(struct nodePCB));
(*ptrPCBReadyLow).queueHead =NULL;
(*ptrPCBReadyLow).queueTail =NULL;

ptrPCBReadyMed = (struct nodePCB*) malloc(sizeof(struct nodePCB));
(*ptrPCBReadyMed).queueHead =NULL;
(*ptrPCBReadyMed).queueTail =NULL;

ptrPCBReadyHigh = (struct nodePCB*) malloc(sizeof(struct nodePCB));
(*ptrPCBReadyHigh).queueHead =NULL;
(*ptrPCBReadyHigh).queueTail =NULL;  // pointer of the PCBs that will be sleeping.  ( acts similar to a blocked on resource queue ) 

//////////// BLOCKED ON X QUEUES ////////////

ptrPCBBlockedReceive = (struct nodePCB*) malloc(sizeof(struct nodePCB));
(*ptrPCBBlockedReceive).queueHead= NULL;
(*ptrPCBBlockedReceive).queueTail= NULL; //pointer to the blocked on message receive queue.

ptrPCBBlockedAllocate = (struct nodePCB*) malloc(sizeof(struct nodePCB));
(*ptrPCBBlockedAllocate).queueHead= NULL;
(*ptrPCBBlockedAllocate).queueTail= NULL;

ptrTimingList = NULL;

////////////////// TRACE BUFFERS /////////////////////////////
sendTraceBuffer = (struct traceBuffer*) malloc(sizeof(struct traceBuffer));

(*sendTraceBuffer).head = 0;
(*sendTraceBuffer).tail = 0;

receiveTraceBuffer = (struct traceBuffer*) malloc(sizeof(struct traceBuffer));

(*receiveTraceBuffer).head = 0;
(*receiveTraceBuffer).tail = 0;

////////////////// MESSAGE ENVELOPES /////////////////////////

ptrMessage = NULL;
ptrMessageTail = NULL;  //will be used as pointers to the head and tail of the messageEnvelope queue. The tail will be used to add

/////////////// LOGIC EXECUTION STARTS HERE ////////////////////
	
		
		signalAssociation(); //Will associate signals with the signal handler who will in turn call the corresponding i process
	   ualarm(alarmDelayTime, alarmFrequency); //sets Ualarm to start running. Used for the timing services.
	
    	ptrPCBList = initializeProcessPCB();   //Will use the initialization table to generate the PCBs and link them in a linked list and will initialize the context for the process.
  	    initializeProcessReadyQueue();

       ptrMessage = initializeMessageEnvelopes();   // Create and list the memory envelopes.
       ptrMessageTail = retrieveEnvelopeTail(ptrMessage); // retrieves the tail ponter of the MessageEnvelopes.
       
       forkAuxillaryProcess();  //forks the keyboard and CRT helper processes. It also initializes the shared memory used by the communication.
   
       initializeProcessContext();  //Will actually initialize the context of each method.
   	
   	   initializeProcessReadyQueue();

   	
  /* 	struct PCB* temp = ptrPCBList; 
   	
   	while(temp !=NULL)
   	{
   		printf("PID: %d\n", temp->PID); 
   		temp = temp->ptrNextPCBList;
   	
   	}
   	
	 temp =	ptrPCBReadyMed->queueHead;
	 while(temp !=NULL)
   	{
   		printf("PID Meds: %d\n", temp->PID); 
   		temp = temp->ptrNextPCBQueue;
   	
   	}
*/
 //	ptrCurrentExecuting = ptrPCBList; //When we call release processor/context swith we need to make sure that ptrCurrentExecuting is pointing to the right PCB. 
		
	 //because we call initializeProcessContext(), ptrCurrentExecuting will point to the CCI (highest priority process)
	 
	 printf("...First PCB Execute  PID: %s\n", debugProcessName(ptrCurrentExecuting->PID));
	 
	 longjmp(ptrCurrentExecuting->contextBuffer,1);//WILL JUMP TO THE FIRST EXECUTION.	
	 printf("In Main: Made the jump to first process and came back: FATAL ERROR - cleaning up\n");
	
	cleanup(); // execute cleanup if for some reason it made it here.
  return 0;
}


void initializeProcessReadyQueue(){

 //The method will take in the PCB list and go one by one appending the PCBs onto the apprioriate priority ready queue. The method should be in the InitializeProcess.c but it is MUCH easier to place it here.
 //Additionally, it will return the tail pointer of the queue for reference.

struct PCB* ptrPCBTemporary = ptrPCBList;  //make a duplicate of the head pointer since we will shuffle down.
int priority = -1;

	while(ptrPCBTemporary!=NULL){ // will stop when we reach the end of the queue. THIS ONLY WORKS IF EACH PCB WAS INITIALIZED TO POINT TO NULL.
	
	if((*ptrPCBTemporary).PCBState == READY){ // if a PCB is of type Iprocess we do not want to enqueue it onto any queue. since the iprocess is always ready to run and does not block. 
	
	priority = (*ptrPCBTemporary).processPriority; //retrieves the priority of the current PCB.

		if(priority == HIGH_PRIORITY){

			//EXECUTES BRIAN'S ENQUEUE.
			/*			
			if(ptrPCBReadyHigh->queueHead==NULL){ //MEANS IT IS THE FIRST ONE TO JOIN THE QUEUE.
			ptrPCBReadyHigh->queueHead = ptrPCBTemporary; //ptrPCBTemporary would be pointing to a pcb of high priority
			ptrPCBReadyHigh->queueTail = ptrPCBTemporary ; // we wish to update the tail as well.
			
			}
			else{ //we will follow the tail, append it to the tail and update the tail.

			ptrPCBReadyHigh->queueTail->ptrNextPCBQueue = ptrPCBTemporary;  // adds it to pointer of the PCB
			ptrPCBReadyHigh->queueTail = ptrPCBTemporary;   // updates the tail.
			}
			*/
			//SINCE THERE IS ONLY ONE HIGH PRIORITY FOR THE MOMENT, THE CCI DOES NOT GET PLACED IN PRIORITY QUEUE AND SIMPLY POINTS THE PTRCURRENTEXECUTING TO THE PCB.
			ptrCurrentExecuting = ptrPCBTemporary; // updates ptrCurrentExecuting to point to the last pcb on the ready queue. 


   }
   else if(priority == MED_PRIORITY){

			//EXECUTES BRIAN'S ENQUEUE.
			if(ptrPCBReadyMed->queueHead==NULL){
			ptrPCBReadyMed->queueHead = ptrPCBTemporary;
			ptrPCBReadyMed->queueTail = ptrPCBTemporary ;
			
			ptrPCBTemporary -> ptrNextPCBQueue = NULL;
			}
			else{

			ptrPCBReadyMed->queueTail->ptrNextPCBQueue  = ptrPCBTemporary;
			ptrPCBReadyMed->queueTail = ptrPCBTemporary;
			ptrPCBTemporary -> ptrNextPCBQueue = NULL;
			}
        }
	else if(priority==LOW_PRIORITY){

			//EXECUTES BRIAN'S ENQUEUE.
			if(ptrPCBReadyLow->queueHead==NULL){
			ptrPCBReadyLow->queueHead = ptrPCBTemporary;
			ptrPCBReadyLow->queueTail = ptrPCBTemporary ;
			ptrPCBTemporary -> ptrNextPCBQueue = NULL;
			}
			else{

			ptrPCBReadyLow->queueTail->ptrNextPCBQueue  = ptrPCBTemporary;
			ptrPCBReadyLow->queueTail = ptrPCBTemporary;
			ptrPCBTemporary -> ptrNextPCBQueue = NULL;
			}

  }
	else if(priority==NULL_PRIORITY){

			//EXECUTES BRIAN'S ENQUEUE.
			if(ptrPCBReadyNull->queueHead==NULL){
			ptrPCBReadyNull->queueHead = ptrPCBTemporary;
			ptrPCBReadyNull->queueTail = ptrPCBTemporary ;
			ptrPCBTemporary -> ptrNextPCBQueue = NULL;
			}
			else{

			ptrPCBReadyNull->queueTail->ptrNextPCBQueue  = ptrPCBTemporary;
			ptrPCBReadyNull->queueTail = ptrPCBTemporary;
			ptrPCBTemporary -> ptrNextPCBQueue = NULL;
			}

		}

	else{
		//What to do here?
		}

	}
	ptrPCBListTail = ptrPCBTemporary; // we are going to return ptrPCBListTail at the end so we'll update it at every step (this could be more elegant)
	ptrPCBTemporary  = (*ptrPCBTemporary).ptrNextPCBList ; //shuffles the temporary pointer down the list.
    }
    
}// we don't need to return the updated tail pointer since this method is within the the same file.

void cleanup(){     //will cleanup the code if a signal is received.

printf("\nHousekeeping...Cleanup\n");


    int bufferSize = BUFFERSIZE;
	
	// terminate child process(es)
  	kill(keyboardChildPID,SIGINT);  //sends a terminate signal to child process.
  	kill(CRTChildPID,SIGINT);  //sends a terminate signal to child process.

   //////////////CLEAN UP KEYBOARD  ////////////////////////
	// remove shared memory segment and do some standard error checks
	
	
	int status = munmap(keyboardmmap_ptr, bufferSize);
    if (status == -1){
      printf("Bad munmap during cleanup Keyboard\n");
    }
	// close the temporary mmap file
    status = close(keyboardFileIdentifier);
    if (status == -1){
      printf("Bad close of temporary mmap file during cleanup\n");
    }
	// unlink (i.e. delete) the temporary mmap file
    status = unlink(keyboardFilename);
    if (status == -1){
    	printf("KeyboardFileName: %s",keyboardFilename);
      printf("Bad unlink during cleanup.\n");
    }

    ////////////CLEAN UP CRT //////////////////

    status = munmap(CRTmmap_ptr, bufferSize);
    if (status == -1){
      printf("Bad munmap during cleanup CRT\n");
    }
	// close the temporary mmap file
    status = close(CRTFileIdentifier);
    if (status == -1){
      printf("Bad close of temporary mmap file during cleanup\n");
    }
	// unlink (i.e. delete) the temporary mmap file
    status = unlink(CRTFilename);
    if (status == -1){
      printf("Bad unlink during cleanup.\n");
    }
	
		/////////////// END EXECUTION ////////////
    printf("\nWill Exit RTX NOW.\n");
    exit(0); // ENDS THE EXECUTION OF THE RTX.

}

char* debugProcessName(int PID)
{
     
     if(PID == PIDUserProcessA)
     strcpy(processName, "ProcessA\0");
     
    else if(PID == PIDUserProcessB)
     strcpy(processName, "ProcessB\0");
     
     else if(PID == PIDUserProcessC)
     strcpy(processName, "ProcessC\0");
     
     else if(PID == PIDcci)
     strcpy(processName, "CCI\0");
     
     else if(PID == PIDNullProcess)
     strcpy(processName, "NullProcess\0");
     
     else if(PID == PIDiProcessKeyboard)
     strcpy(processName, "Keyboard iProcess \0");
     
     else if(PID == PIDiProcessCRT)
     strcpy(processName, "CRT iProcess\0");
     
     else if(PID == PIDiProcessTimer)
     strcpy(processName, "Timer iProcess\0");
     
      else if(PID == PIDWallClock)
     strcpy(processName, "Wall Clock\0");

     return processName;
}

char* debugMessageType(int Type)
{
     if(Type == MSGTYPEDATA)
     strcpy(msg, "MSGTYPEDATA\0");
     
     else if(Type == MSGTYPEACK)
     strcpy(msg, "MSGTYPEACK\0");
     
     else if(Type == MSGTYPEWAKEUP)
     strcpy(msg, "MSGTYPEWAKEUP\0");
     
     else if(Type == MSGCONSOLEINPUT)
     strcpy(msg, "MSGCONSOLEINPUT\0");
     
     else if(Type == MSGBLANK)
     strcpy(msg, "MSGBLANK\0");

	 else if(Type == MSGTYPECOUNT)
		 strcpy(msg, "MSGTYPECOUNT");
     
     else if(Type == MSGREQUESTDELAY)
     strcpy(msg, "MSGREQUESTDELAY\0");
     
     return msg;
}

