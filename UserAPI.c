#include "Processes.h"

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
