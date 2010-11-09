#ifndef publicAPI
#define publicAPI

#include "Struct.h"

////////////////// API FOR USER PROCESSES ///////////////

// THE USER PROCESSES ONLY HAVE ACCESS TO THE METHODS IN THIS .h FILE.
struct messageEnvelope* request_message_env();
int  release_message_env(struct messageEnvelope* temp );
int  send_message(int dest_process_id, struct messageEnvelope* temp );
struct messageEnvelope* receive_message();
int  get_console_chars(struct messageEnvelope * temp );
int  send_console_chars(struct messageEnvelope * temp );
int  release_processor( );
int  request_process_status(struct messageEnvelope * temp );
int  change_priority(int priority, int target_process_id);
int  request_delay( int delay, int wakeup_code,struct messageEnvelope * temp );
int  terminate();

	#endif