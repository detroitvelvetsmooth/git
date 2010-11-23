#include "publicProcesses.h"

struct messageEnvelope* request_message_env()
{	
/*	printf("Turning Atomic On - request_message_env\n");*/
	atomic(1);
	struct messageEnvelope *temp;
	temp = k_request_message_env();
/*	printf("Turning Atomic Off - request_message_env\n");*/
	atomic(0);
    return temp;
}

int  release_message_env(struct messageEnvelope* temp )
{
/*	printf("Turning Atomic On - release_message_env\n");*/
	atomic(1);
	k_release_message_env(temp);
/*	printf("Turning Atomic Off - release_message_env\n");*/
	atomic(0);
    return 1;
}

int  send_message(int dest_process_id, struct messageEnvelope* temp )
{
/*	printf("Turning Atomic On - send_message\n");*/
	atomic(1);
	k_send_message(dest_process_id, temp);	
/*	printf("Turning Atomic Off - send_message\n");*/
	atomic(0);
    return 1;
}

struct messageEnvelope* receive_message()
{
/*	printf("Turning Atomic On - receive_message\n");*/
	atomic(1);
 	struct messageEnvelope *temp;
	temp = k_receive_message();
/*	printf("Turning Atomic Off - receive_message\n");*/
	atomic(0);
   return temp;
}

int  get_console_chars(struct messageEnvelope * temp )
{
/*	printf("Turning Atomic On - get_console_chars\n");*/
	atomic(1);
	k_get_console_chars(temp);
/*	printf("Turning Atomic Off - get_console_chars\n");*/
	atomic(0);
    return 1;
}

int  send_console_chars(struct messageEnvelope * temp )
{
/*	printf("Turning Atomic On - send_console_chars\n");*/
	atomic(1);
	k_send_console_chars(temp);
/*	printf("Turning Atomic Off - send_console_chars\n");*/
	atomic(0);
    return 1;
}

int  release_processor( )
{	
	//printf("Turning Atomic On - release_processor\n");
	atomic(1);
	k_release_processor();
	//printf("Turning Atomic Off - release_processor\n");
	atomic(0);
    return 1;
}


int request_process_status(struct messageEnvelope * temp )
{
	atomic(1);
	k_request_process_status(temp); //will populate our message of temp with the commands in tabulated format. 
	atomic(0);
    return 1;
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
