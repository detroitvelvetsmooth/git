#include "Struct.h"
#include "Processes.h"

/*Struct.h will contain the struct definitions to be used. MSG envelopes and PCBs and any
other. Processes.h will contain the function prototypes for every process (user and iprocess) to be used.
*/

int main() {

    //Every major process will be split into separate files.
    
absoluteTime = 0;
relativeTime = 0; // the wallclock will initially commence at the same time as the absoluteTime
displayWallClock= 0;

receiveTraceBufferIndexHead = 0; 
receiveTraceBufferIndexTail = 0;

sendTraceBufferIndexHead = 0;
sendTraceBufferIndexTail = 0;

ptrCurrentExecuting = NULL; //will point to the currently executing Process.

ptrPCBList = NULL; //ptr that will link to the PCB list (which will remain somewhat static once initialized
ptrPCBListTail = NULL; //this tail ptr may or may not be required.

ptrPCBReadyNull = NULL; // ptrs to the various priority queues for PCBs that are READY. for the nullPriority we will only have one process there.
ptrPCBReadyNullTail = NULL;

ptrPCBReadyLow = NULL;
ptrPCBReadyLowTail = NULL;

ptrPCBReadyMed = NULL;
ptrPCBReadyMedTail = NULL;

ptrPCBReadyHigh = NULL;
ptrPCBReadyHighTail = NULL;

ptrPCBTiming = NULL;  // pointer of the PCBs that will be sleeping.  ( acts similar to a blocked on resource queue ) 
ptrTimingList = NULL; // pointer to messages that have the timing countdown. 


/*The head and tail ptrs will be used the head will be used to dequeue from the list and the tail will be used to enqueue onto the list. (we could change this into an array if required if it becomes very messy)
*/

ptrPCBBlockedReceive = NULL;
ptrPCBBlockedReceiveTail = NULL; //pointer to the blocked on message receive queue.

ptrPCBBlockedAllocate = NULL;
ptrPCBBlockedAllocateTail = NULL; //pointer to the blocked on envelope allocate receive queue.

ptrMessage = NULL;
ptrMessageTail = NULL;  //will be used as pointers to the head and tail of the messageEnvelope queue. The tail will be used to add

	
	
		signalAssociation(); //Will associate signals with the signal handler who will in turn call the corresponding i process
		printf("Signals Associated\n");
	
		ualarm(alarmDelayTime, alarmFrequency); //sets Ualarm to start running. Used for the timing services.
		printf("Ualarm Set\n");
	
	
//	ptrPCBList = initializeProcessPCB();   //Will use the initialization table to generate the PCBs and link them in a linked list and will initialize the context for the process.
//	initializeProcessReadyQueue();

    	ptrMessage = initializeMessageEnvelopes();   // Create and list the memory envelopes.
    	ptrMessageTail = retrieveEnvelopeTail(ptrMessage); // retrieves the tail ponter of the MessageEnvelopes.
		  printf("Message Envelopes Created and Linked\n");
		 
  	 forkAuxillaryProcess();  //forks the keyboard and CRT helper processes. It also initializes the shared memory used by the communication.
	   printf("Helper Process Forked\n");

		 
	//initializeProcessContext(ptrPCBList);  //Will actually initialize the context of each method.
	//KreleaseProcessor(); //calls the dispatcher which would schedule the highest process to run.  In theory, the OS should never come back after this call.


	while(1){} //TODO THIS WHILE LOOP SHOULD NEVER BE EXECUTED UNDER PROPER CONDITIONS. 
	
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


			if(ptrPCBReadyHigh==NULL){ //MEANS IT IS THE FIRST ONE TO JOIN THE QUEUE.
			ptrPCBReadyHigh = ptrPCBTemporary; //ptrPCBTemporary would be pointing to a pcb of high priority
			ptrPCBReadyHighTail = ptrPCBTemporary ; // we wish to update the tail as well.
			
			}
			else{ //we will follow the tail, append it to the tail and update the tail.

			(*ptrPCBReadyHighTail).ptrNextPCBQueue  = ptrPCBTemporary;  // adds it to pointer of the PCB
			ptrPCBReadyHighTail = ptrPCBTemporary;   // updates the tail.
			}

   }
   else if(priority == MED_PRIORITY){

			if(ptrPCBReadyMed==NULL){
			ptrPCBReadyMed = ptrPCBTemporary;
			ptrPCBReadyMedTail = ptrPCBTemporary ;
			}
			else{

			(*ptrPCBReadyMedTail).ptrNextPCBQueue  = ptrPCBTemporary;
			ptrPCBReadyMedTail = ptrPCBTemporary;
			}
        }
	else if(priority==LOW_PRIORITY){

			if(ptrPCBReadyLow==NULL){
			ptrPCBReadyLow = ptrPCBTemporary;
			ptrPCBReadyLowTail = ptrPCBTemporary ;
			}
			else{

			(*ptrPCBReadyLowTail).ptrNextPCBQueue  = ptrPCBTemporary;
			ptrPCBReadyLowTail = ptrPCBTemporary;
			}

  }
	else if(priority==NULL_PRIORITY){

			if(ptrPCBReadyNull==NULL){
			ptrPCBReadyNull = ptrPCBTemporary;
			ptrPCBReadyNullTail = ptrPCBTemporary ;
			}
			else{

			(*ptrPCBReadyNullTail).ptrNextPCBQueue  = ptrPCBTemporary;
			ptrPCBReadyNullTail = ptrPCBTemporary;
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


// routine to clean up things before terminating main program
// This stuff must be cleaned up or we have child processes and shared
//	memory hanging around after the main process terminates

void cleanup(){     //will cleanup the code if a signal is received.

printf("\nHousekeeping...Cleanup\n");


    int bufferSize = BUFFERSIZE;
	// terminate child process(es)
  	kill(keyboardChildPID,SIGINT);  //sends a terminate signal to child process.
  //	kill(CRTChildPID,SIGINT);  //sends a terminate signal to child process.

   //////////////clean up Keyboard ////////////////////////
	// remove shared memory segment and do some standard error checks
	
	
	int status = munmap(keyboardmmap_ptr, bufferSize);
    if (status == -1){
      printf("Bad munmap during cleanup\n");
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

    ////////////Cleans up CRT //////////////////

  /*  status = munmap(CRTmmap_ptr, bufferSize);
    if (status == -1){
      printf("Bad munmap during cleanup\n");
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
	*/
    printf("\nWill Exit RTX NOW.\n");
    exit(0); // ENDS THE EXECUTION OF THE RTX.

}


