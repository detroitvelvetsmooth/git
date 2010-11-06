#include "Struct.h"

//Struct.h is used to obtain the structure of the message envelopes.

#define totalMessages 128
// we define the total amount of messsages to be used in the system.
//This method will allocate space for the message envelopes and then link them together and set the head and tail pointers accordingly.
//remember for the pointers we have to use *ptr = whatever not ptr = &whatever

struct messageEnvelope * initializeMessageEnvelopes() {

// the message tail will have to be obtained later by another method. we can't do it here since the one defined inside this method would not link to the global one.

struct messageEnvelope * temporaryPointer = (struct messageEnvelope*)malloc(sizeof(struct messageEnvelope));
struct messageEnvelope * messageHead = temporaryPointer;
    // both pointers would point to the first envelope. Then we can start looping.
		//creates the first envelope. The envelop will have a pointer to another message envelop set to null

(*temporaryPointer).ptrNextMessage = NULL;
(*temporaryPointer).PIDSender =-1; 
(*temporaryPointer).PIDReceiver = -1;
(*temporaryPointer).messageType = -1;
// For the messageTimeStamp and actual message, they are arrays of characters which will be defined when first used. 
 // we assign values of -1 for the integers since 0,1,2,3, could be potential values.

int i = 0;
for (i=0; i<totalMessages-1; i ++)
{
	(*temporaryPointer).ptrNextMessage = (struct messageEnvelope*)malloc(sizeof(struct messageEnvelope)); //links the current envelope pointer to the next envelope.

	temporaryPointer = (*temporaryPointer).ptrNextMessage; // updates our temporary pointer.

	(*temporaryPointer).ptrNextMessage = NULL;
	(*temporaryPointer).PIDSender =i;  //TODO CHANGE LATER BACK TO -1
	(*temporaryPointer).PIDReceiver = -1;
	(*temporaryPointer).messageType = -1;

}

return messageHead; //messageHead would contain be a pointer to the first envelope created.
}

struct messageEnvelope * retrieveEnvelopeTail(struct messageEnvelope* messageHead) // the method will retrieve the messageEnvelopes tail. We need loop through the envelopes to reach the end
{
  struct messageEnvelope*  temporaryPointerLead = NULL;
  struct messageEnvelope* temporaryPointerLagging = NULL;

  temporaryPointerLead = messageHead;
  temporaryPointerLagging = messageHead;
  temporaryPointerLead = (*messageHead).ptrNextMessage;

  while(temporaryPointerLead!=NULL)
  {
  temporaryPointerLagging = temporaryPointerLead;
  temporaryPointerLead = (*temporaryPointerLead).ptrNextMessage;

  }

  return temporaryPointerLagging;
}

void TraverseEnvelopes(struct messageEnvelope* messageHead) // the method will retrieve the messageEnvelopes tail. We need loop through the envelopes to reach the end
{
  struct messageEnvelope*  temporary = messageHead;
	int numEnvelope = 0;
	
  while(temporary!=NULL)
  {
  printf("MessageEnvelope #: %d: Message PID: %d \n",numEnvelope,(*temporary).PIDSender);
  temporary = (*temporary).ptrNextMessage;
	numEnvelope++;
  } 
}









