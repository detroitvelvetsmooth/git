#include "Struct.h"

//#include "FoolAround.c"



/* This method will initialize the processes appropriately by first looping through an initialization table and creating and linking the PCBs
into a linked list. Then once the PCBs are created it should
*/

#define processStackSize 16384
#define processStackOffset 4

struct PCB * initializeProcessPCB(){

//INITIALIZES REGULAR PROCESSES. IPROCESSES ARE INITIALIZED AFTERWARDS. 
int processPID[numProcesses];
int processPriority[numProcesses];
int processType[numProcesses];  //will define what type of process it is. iprocess or regular process.
void (*processProgramCounter[numProcesses])(); //array of function pointers.

// iProcess will have no priority associated with them or program counter since they are executed on demand. and are just functions.

processPID[0] = PIDUserProcessA;
processPriority[0] = MED_PRIORITY;
processType[0] = READY;
processProgramCounter[0] = ProcessA; 

processPID[1] = PIDUserProcessB;
processPriority[1] = MED_PRIORITY;
processType[1] = READY;
processProgramCounter[1] =ProcessB;

processPID[2] = PIDUserProcessC;
processPriority[2] = MED_PRIORITY;
processType[2] = READY;
processProgramCounter[2] =ProcessC;

processPID[3] = PIDcci;
processPriority[3] = HIGH_PRIORITY;
processType[3] = READY;
processProgramCounter[3] =CCI;

processPID[4] = PIDNullProcess;
processPriority[4] = NULL_PRIORITY; 
processType[4] = READY;
processProgramCounter[4] =NullProcess; 

processPID[5] = PIDWallClock;
processPriority[5] = LOW_PRIORITY;
processType[5] = READY;
processProgramCounter[5] =WallClock;

//Now that we can define create a PCB for all the processes and link each new process.

struct PCB* ptrPCBHead = NULL;
struct PCB* temporaryPCB = NULL;

ptrPCBHead = (struct PCB*)malloc(sizeof(struct PCB));
temporaryPCB = ptrPCBHead;

// temporaryPCB and ptrPCBHead are equal we will now initialize each PCB field.


(*temporaryPCB).PID = processPID[0]; //sets the Process Id as specified in IT
(*temporaryPCB).PCBState = processType[0];   // sets all processes to Ready to Execute
(*temporaryPCB).processPriority = processPriority[0]; // defines the priority it has.

(*temporaryPCB).programCounter = processProgramCounter[0];   //specifies the program counter.
(*temporaryPCB).ptrStack = ((char *)malloc(processStackSize))+ processStackSize - processStackOffset ; //allocates the stack size for each PCB.

(*temporaryPCB).ptrNextPCBList = NULL;   // This is set to null but will change as more PCBs are created.
(*temporaryPCB).ptrNextPCBQueue = NULL;  // originally set to null but will be placed on ready queue

(*temporaryPCB).ptrMessageInboxHead = NULL;
(*temporaryPCB).ptrMessageInboxTail = NULL;

//(*temporaryPCB).contextBuffer;  //initializes the jmp_buf to NULL. MAY NOT BE CORRECT <TODO>

int i = 0;
for(i = 1; i < numProcesses; i ++ )
{

	(*temporaryPCB).ptrNextPCBList =  (struct PCB*)malloc(sizeof(struct PCB)); //defines the New PCB and links it to the linked list. 
	temporaryPCB = (*temporaryPCB).ptrNextPCBList; //moves the temporary pointer to the new PCB

	(*temporaryPCB).PID = processPID[i];
	(*temporaryPCB).PCBState = processType[i];
	(*temporaryPCB).processPriority = processPriority[i];

	(*temporaryPCB).programCounter = processProgramCounter[i];
	(*temporaryPCB).ptrStack = ((char*)malloc(processStackSize))+ processStackSize - processStackOffset; 

	(*temporaryPCB).ptrNextPCBList = NULL;
	(*temporaryPCB).ptrNextPCBQueue = NULL;

	(*temporaryPCB).ptrMessageInboxHead = NULL;
	(*temporaryPCB).ptrMessageInboxTail = NULL;

//	(*temporaryPCB).contextBuffer;

}

// UP TO HERE, REGULAR PROCESSES HAVE BEEN ALLOCATED, NOW WE APPEND THE IPROCESSES. WHICH WILL BE HARDCODED.

/////////////keyboard iprocess ///////////

	(*temporaryPCB).ptrNextPCBList =  (struct PCB*)malloc(sizeof(struct PCB)); //defines the New PCB and links it to the linked list. 
	temporaryPCB = (*temporaryPCB).ptrNextPCBList; //moves the temporary pointer to the new PCB
	
(*temporaryPCB).PID = PIDiProcessKeyboard; //sets the Process Id as specified in IT
(*temporaryPCB).PCBState = IPROCESS;   // specifies iprocess.
(*temporaryPCB).processPriority = -1; // iProcess have no priority

(*temporaryPCB).programCounter = NULL;   //specifies the program counter.
(*temporaryPCB).ptrStack = NULL;

(*temporaryPCB).ptrNextPCBList = NULL;  
(*temporaryPCB).ptrNextPCBQueue = NULL; 

(*temporaryPCB).ptrMessageInboxHead = NULL;
(*temporaryPCB).ptrMessageInboxTail = NULL;
	
	
////// crt iprocess //////////////

	(*temporaryPCB).ptrNextPCBList =  (struct PCB*)malloc(sizeof(struct PCB)); //defines the New PCB and links it to the linked list. 
		temporaryPCB = (*temporaryPCB).ptrNextPCBList; //moves the temporary pointer to the new PCB

	(*temporaryPCB).PID = PIDiProcessCRT; //sets the Process Id as specified in IT
	(*temporaryPCB).PCBState = IPROCESS;   // specifies iprocess.
	(*temporaryPCB).processPriority = -1; // iProcess have no priority

	(*temporaryPCB).programCounter = NULL;   
	(*temporaryPCB).ptrStack = NULL;

	(*temporaryPCB).ptrNextPCBList = NULL;  
	(*temporaryPCB).ptrNextPCBQueue = NULL; 
	
	(*temporaryPCB).ptrMessageInboxHead = NULL;
	(*temporaryPCB).ptrMessageInboxTail = NULL;
	

////// timing iprocess ////////////

	(*temporaryPCB).ptrNextPCBList =  (struct PCB*)malloc(sizeof(struct PCB)); //defines the New PCB and links it to the linked list. 
	temporaryPCB = (*temporaryPCB).ptrNextPCBList; //moves the temporary pointer to the new PCB
	
	(*temporaryPCB).PID = PIDiProcessTimer; //sets the Process Id as specified in IT
	(*temporaryPCB).PCBState = IPROCESS;   // specifies iprocess.
	(*temporaryPCB).processPriority = -1; // iProcess have no priority

	(*temporaryPCB).programCounter = NULL; 
	(*temporaryPCB).ptrStack = NULL;

	(*temporaryPCB).ptrNextPCBList = NULL;  
	(*temporaryPCB).ptrNextPCBQueue = NULL;  

	(*temporaryPCB).ptrMessageInboxHead = NULL;
	(*temporaryPCB).ptrMessageInboxTail = NULL;
	

// AT THIS POINT, ALL THE PCB structs have been created and are placed in a linked list.
// They now need to be placed in their appropriate ready queue ( based on priority)
// Additionally, their context must be initialized. (both of these items are in another method).


return ptrPCBHead;

}

void initializeProcessContext(){
//INITIALIZES THE PROCESS CONTEXT. UNSURE ABOUT THIS SECTION.

//TODO - I AM UNSURE OF WHEN TO PUT THE CURRENT PROCESS POINTER, SHOULD THAT BE DONE BY THE SCHEDULER OR WHAT?

  ptrCurrentExecuting = ptrPCBList; // create temporary pointer to loop through
	jmp_buf kernel_buf;  // temporary context buffer.
	char * stackInit;  //temporary stack pointer.

	while(ptrCurrentExecuting!=NULL){


		if((*ptrCurrentExecuting).PCBState!= IPROCESS) //IPROCESSES WILL HAVE NO CONTEXT AND WILL BE JUST PCB.s
		{


		if (setjmp(kernel_buf)==0){ //saves current context temporarily.

			stackInit = (*ptrCurrentExecuting).ptrStack; //obtain the stack pointer for the current PCB

			__asm__ ("movl %0,%%esp" :"=m" (stackInit)); // if Linux i386 target
          
				// replace the current stack pointer with the PCB stack pointer.

			if(setjmp((*ptrCurrentExecuting).contextBuffer)==0){ //initialize the context of the process, which will return zero.

						longjmp(kernel_buf,1);
			}
			else{
                ptrCurrentExecuting->programCounter(); //executes for the first time.
			    printf("initializeProcessContext: Process failed to execute correctly or never called a context switch\n");
			
			}
		}
	// we update our current process
		}
		
	ptrCurrentExecuting = (*ptrCurrentExecuting).ptrNextPCBList;
	}

}

