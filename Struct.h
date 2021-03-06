#ifndef Struct
#define Struct

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>


// The libraries included here will be common to all files. If a particular file requires additional libraries they will be imported on the fly.

#define BUFFERSIZE 1200 //will be used by the shared memory of the keyboard and CRT processes.
#define MAXCHAR 1100	 // specifies the buffer length for the shared memory.
#define MAXTRACEBUFFER 16 // Maximum number of entries in trace buffers

#define alarmDelayTime 	900000 //delays 10 seconds
#define alarmFrequency  100000  // set the frequency of the signal to 100 ms. It is a #define since these will not change at all.

#define numProcesses 6 //SPECIFIES NUMBER OF PROCESSES THAT REQUIRE CONTEXT INITIALIZATION (all minus iprocess)
#define numProcessesTotal 9

#define HIGH_PRIORITY 0
#define MED_PRIORITY  1
#define LOW_PRIORITY  2
#define NULL_PRIORITY 3				//Presents the various priorities in the RTX.

#define EXECUTING 0
#define READY 1
#define BLOCKED_MSG_ALLOCATE 2
#define BLOCKED_MSG_RECEIVE 3
#define BLOCKED_SLEEPING 4 
#define IPROCESS 5   //	Specifies the different states of the PCBs. 

#define MSGTYPEDATA 0
#define MSGTYPECOUNT 1
#define MSGTYPEACK 2
#define MSGTYPEWAKEUP 3
#define MSGCONSOLEINPUT 4
#define MSGBLANK 5
#define MSGREQUESTDELAY 6

#define PIDUserProcessA 0
#define PIDUserProcessB 1
#define PIDUserProcessC 2
#define PIDcci 3
#define PIDWallClock 4
#define PIDNullProcess 5
#define PIDiProcessKeyboard 6
#define PIDiProcessCRT 7
#define PIDiProcessTimer 8	

////////////// MISCELLANEOUS FUNCTIONS USED BY INITIALIZING METHODS ////////////
void ProcessA();
void ProcessB();
void ProcessC();
void NullProcess();
void CCI();
void WallClock();
void iProcessCRT();
void cleanup();
char * debugProcessName(int PID);
char processName[20];
char * debugMessageType(int Type);
 char msg[20];
 
 //////////ATOMICITY /////////////////
 
 //sigset_t oldmask;
 //sigset_t newmask;
 int atomicCount; 

//////////////// HELPER PROCESS KEYBOARD INITIALIZATION //////////////////

struct Buffer  * keyboardSharedMemPointer;		// pointer to structure that is the shared memory
int keyboardChildPID;				// pid of keyboard child process
caddr_t keyboardmmap_ptr; //unsure of what caddr_t means.
int keyboardFileIdentifier;
char * keyboardFilename;  //the name of the shared_memory file

/////////////// HELPER PROCESS CRT INITIALIZATION //////////////////
struct Buffer  * CRTSharedMemPointer;		// pointer to structure that is the shared memory
int CRTChildPID;				// pid of keyboard child process
caddr_t CRTmmap_ptr; //unsure of what caddr_t means.
int CRTFileIdentifier;
char * CRTFilename;
 
/////////////// TIMING IPROCESS ///////////////

int absoluteTime;
int relativeTime;
int displayWallClock;

int atomic_count;

struct messageEnvelope* ptrTimingList; // will be a  pointer to a linked list that contains the envelopes for the timing queu

////////////// TRACE BUFFERS ///////////////////

struct traceBuffer* sendTraceBuffer;		
struct traceBuffer* receiveTraceBuffer; 


///////////////// GENERAL PCB LIST ///////////////////
struct PCB* ptrCurrentExecuting; //will point to the currently executing Process.
struct PCB* ptrPCBList; //ptr that will link to the PCB list (which will remain somewhat static once initialized
struct PCB* ptrPCBListTail; //this tail ptr may or may not be required.

////////////// READY PRIORITY QUEUES ////////////////

struct nodePCB* ptrPCBReadyNull; // ptrs to the various priority queues for PCBs that are READY. for the nullPriority we will only have one process there.
struct nodePCB* ptrPCBReadyLow;
struct nodePCB* ptrPCBReadyMed;
struct nodePCB* ptrPCBReadyHigh;

//////////////// BLOCKED ON X QUEUES ////////////////

struct nodePCB* ptrPCBBlockedReceive; //pointer to the blocked on message receive queue.
struct nodePCB* ptrPCBBlockedAllocate; //pointer to the blocked on envelope allocate receive queue.
struct messageEnvelope* ptrTimingList;

//////////////// EMPTY MESSAGE ENVELOPES QUEUE ///////////

struct messageEnvelope* ptrMessage;
struct messageEnvelope* ptrMessageTail;  //will be used as pointers to the head and tail of the messageEnvelope queue. The tail will be used to add


///////// STRUCT DEFINITIONS ////////////

struct Buffer{
	int completedFlag;
	char data[MAXCHAR];
};

struct traceBuffer{
	int head;
	int tail;
	int data[MAXTRACEBUFFER][4];
};

struct nodePCB{  // Will be used to contain the head and tail pointers of whatever it is pointing to. 

struct PCB* queueHead;
struct PCB* queueTail;
};

struct messageEnvelope
{
	struct messageEnvelope * ptrNextMessage; //will point to the next messsage, if it is in an inbox queue or in the free memory queue.
	int PIDSender; //int to track the sending process .
	int PIDReceiver; // int to track the receiving process.
	int sleepTicks; // WILL BE USED TO SPECIFY HOW MANY TICKS ARE REMAINING IF A PROCESS IS SLEEPING.
	int messageType; //NOT sure of what type of messages we will have. we will probably use global variables as well.
	char messageTimeStamp[10]; // contain the time when the message was sent.
	char messageText[MAXCHAR];  //contains the actual message. It will be in string format and it could subsequently be parsed.
};

struct PCB{

	int PID; //will contain the PID as defined in the initialization table
	int PCBState; //will contain an int indicating their running state. Which will also change.
	int processPriority; //Will contain the priority of each process

	void (*programCounter)(); // will contain a ptr to the initial start of the process. hence it is a ptr to a function (THIS MAY BE WRONG)
	char * ptrStack; //contain a ptr to the stack. 

	struct PCB* ptrNextPCBList; //will link to the next PCB in the main PCB List
	struct PCB* ptrNextPCBQueue; //will link to the next PCB in whatever queue they may be (running/blocked on resource/executing)

	struct messageEnvelope* ptrMessageInboxHead;
	struct messageEnvelope* ptrMessageInboxTail; //links to inbox queue of message envelopes the tail may not be required.

	jmp_buf contextBuffer;  // Used for process switching

};
#endif

