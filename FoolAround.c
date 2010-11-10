#include "Struct.h"


void context_switch(struct PCB * new) {

		int return_code = setjmp(ptrCurrentExecuting->contextBuffer);
		if(return_code==0)
	{
  	ptrCurrentExecuting = new;
		longjmp(ptrCurrentExecuting->contextBuffer, 1);
	}
}

void testContextA()
{ 
	struct PCB* new = ptrPCBList->ptrNextPCBList;
	
//	new->programCounter(); (used to prove that the right function pointer is being used.
	 	
	 	while(1) {
		printf("Hi I'm process A\n");
		sleep(1);
		context_switch(new);
  }
}

void testContextB()
{ 	
	struct PCB* new = ptrPCBList;
	 	
	 	while(1) {
		printf("Hi I'm process B\n");
		sleep(1);
		context_switch(new);
  }
}




