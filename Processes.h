#ifndef Processes
#define Processes

#include "Struct.h"

//contains the function prototypes of important functions.

/////////////////// EXECUTED URIN INITIALIZATION ///////////////
void signalAssociation(); //Will associate signals with the signal handler who will in turn call the corresponding i process

struct PCB * initializeProcessPCB(); //Will use the initialization table to generate the PCBs and link them in a linked list and will initialize the context for the process.
void initializeProcessReadyQueue();
void initializeProcessContext(struct PCB* ptrPCBList);
void cleanup();
void forkAuxillaryProcess();

struct messageEnvelope* initializeMessageEnvelopes(); //Will create and link together the message envelopes.
struct messageEnvelope* retrieveEnvelopeTail(struct messageEnvelope* messageHead); //retrieves envelope Tail, which will be used to deallocate messages.
void TraverseEnvelopes(); //TODO REMOVE LATER. 

///////////////////// IPROCESSES ///////////////

void iProcessKeyboard() ;
void iProcessCRT();
void iProcessAlarm();

////////////////// K PRIMITIVES ///////////////

struct messageEnvelope* k_request_message_env( );
int  k_release_message_env ( struct messageEnvelope* temp );
int k_send_message( int dest_process_id, struct messageEnvelope* temp );
struct messageEnvelope* k_receive_message( );
int  k_get_console_chars( struct messageEnvelope * temp );
int  k_send_console_chars(struct messageEnvelope * temp );

int  release_processor();
int  request_process_status(struct messageEnvelope * temp );
int  change_priority(int new_priority, int targetID); 
int  request_delay(int delay, int wakeup_code, struct messageEnvelope * temp);
int k_terminate();

struct PCB * getPCB(int findPID);
void atomic(int on);


/////////////TO BE WRITTEN //////////////
void requestSleep();




	#endif
