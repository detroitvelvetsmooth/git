#ifndef publicProcesses
#define publicProcesses

#include "Struct.h"

////////////////// K PRIMITIVES ///////////////

struct messageEnvelope* k_request_message_env( );
int  k_release_message_env ( struct messageEnvelope* temp );
int k_send_message( int dest_process_id, struct messageEnvelope* temp );
struct messageEnvelope* k_receive_message( );
int  k_get_console_chars( struct messageEnvelope * temp );
int  k_send_console_chars(struct messageEnvelope * temp );

int  k_release_processor();
int  k_request_process_status(struct messageEnvelope * temp );
int  k_change_priority(int new_priority, int targetID); 
int  k_request_delay(int delay, int wakeup_code, struct messageEnvelope * temp);
int  k_terminate();

//////////// AUXILLARY TO PRIMITIVES ///////////////

int Enqueue(struct PCB* ptr,struct nodePCB* Q);
struct PCB* Dequeue(struct nodePCB* Q);
struct PCB * getPCB(int findPID);
void atomic(int on);

/////////////TO BE WRITTEN //////////////
void requestSleep();

	#endif
