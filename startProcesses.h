#ifndef startProcesses
#define startProcesses

#include "Struct.h"

//contains the function prototypes of important functions used during initialization.

/////////////////// EXECUTED DURING INITIALIZATION ///////////////

void signalAssociation(); //Will associate signals with the signal handler who will in turn call the corresponding i process

struct PCB * initializeProcessPCB();									 //Creates PCB list
void initializeProcessReadyQueue();  									 //Inserts PCB list into priority queues. 
void initializeProcessContext(struct PCB* ptrPCBList); //Initializes Process context for each PCB
void cleanup();  																			 //Cleans up the shared memory and kills helper processes. 
void forkAuxillaryProcess();                           //Forks childs proceses.

struct messageEnvelope* initializeMessageEnvelopes();  //Will create and link together the message envelopes.
struct messageEnvelope* retrieveEnvelopeTail(struct messageEnvelope* messageHead); //retrieves envelope Tail, which will be used to deallocate messages.

void TraverseEnvelopes(); //TODO REMOVE LATER. 

///////////////////// IPROCESSES ///////////////

void iProcessKeyboard() ;
void iProcessCRT();
void iProcessAlarm();

	#endif
