#include "publicProcesses.h"

struct messageEnvelope* request_message_env()
{
	atomic(1);
	struct messageEnvelope *temp;
	temp = k_request_message_env();
	atomic(0);
    return temp;
}

int  release_message_env(struct messageEnvelope* temp )
{
	atomic(1);
	k_release_message_env(temp);
	atomic(0);
    return 1;
}

int  send_message(int dest_process_id, struct messageEnvelope* temp )
{
	atomic(1);
	k_send_message(dest_process_id, temp);
	atomic(0);
    return 1;
}

struct messageEnvelope* receive_message()
{
	atomic(1);
 struct messageEnvelope *temp;
	temp = k_receive_message();
	atomic(0);
   return temp;
}

int  get_console_chars(struct messageEnvelope * temp )
{
	atomic(1);
	k_get_console_chars(temp);
	atomic(0);
    return 1;
}

int  send_console_chars(struct messageEnvelope * temp )
{
	atomic(1);
	k_send_console_chars(temp);
	atomic(0);
    return 1;
}

int  release_processor( )
{
	atomic(1);
	k_release_processor();
	atomic(0);
    return 1;
}


struct messageEnvelope*  request_process_status(struct messageEnvelope * temp )
{
	atomic(1);
	temp = k_request_process_status(temp); //will populate our message of temp with the commands in tabulated format. 
	atomic(0);
    return temp;
}

int  change_priority(int priority, int target_process_id)
{
    atomic(1);
	int result;
	result = k_change_priority(priority, target_process_id);
	atomic(0);
    return result;
}

int  request_delay( int delay, struct messageEnvelope * temp ) 
{
    atomic(1);
	k_request_delay(delay, temp);
	atomic(0);
    return 1;
}

int  terminate()
{
    atomic(1);
	k_terminate();
	atomic(0);
    return 1;
}

int  get_trace_buffers(struct messageEnvelope * temp)
{
    atomic(1);
	k_get_trace_buffers(temp);
	atomic(0);
    return 1;
}
