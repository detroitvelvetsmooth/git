#ifndef publicProcesses
#define publicProcesses

#include "Struct.h"

////////////////// K PRIMITIVES ///////////////

struct messageEnvelope* k_request_message_env( );
struct messageEnvelope* k_receive_message( );

int k_release_message_env ( struct messageEnvelope* temp );
int k_send_message( int dest_process_id, struct messageEnvelope* temp );
int k_get_console_chars( struct messageEnvelope * temp );
int k_send_console_chars(struct messageEnvelope * temp );

int  k_release_processor();
void k_process_switch();

int  k_request_process_status(struct messageEnvelope * temp );
int  k_change_priority(int new_priority, int targetID); 
int  k_request_delay(int delay, struct messageEnvelope * temp);
int  k_get_trace_buffers(struct messageEnvelope* temp);
int  k_terminate();

//////////// AUXILLARY TO PRIMITIVES ///////////////

void context_switch(jmp_buf* last_PCB, jmp_buf* next_PCB);
void atomic(int on);
struct PCB * getPCB(int findPID);
int Enqueue(struct PCB* ptr,struct nodePCB* Q);
struct PCB* Dequeue(struct nodePCB* Q);
struct PCB* SearchPCBDequeue(int searchPID, struct nodePCB* Q);
struct PCB* ReadyProcessDequeue();
void add_to_traceBuffer(struct messageEnvelope* temp);

	#endif
