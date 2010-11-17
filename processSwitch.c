SORRY I BUTCHERED THIS PAGE BRIAN. EVERYTHING GOT PLACED ONTO KPRIMITIVES.C

	
//Below it our over-qualified TA's example of context_switch
//it may help us to test context initialization



	/**
 * Context switch example.
 * Thomas Reidemeister

#define STACK_SZ	16384

/** generic function pointer type, used to "hack"
 * starting addresses
 
typedef void (*startAddress)(void);

/** Mini PCB.
 * Hint: in your project this will be more complex!
 
typedef struct {
	jmp_buf context;
	startAddress addr; 
	char * sp;
} tinyPCB;

tinyPCB procA;
tinyPCB procB;
tinyPCB *current_process;
char *stackInit;

/** Switch the context from one process to another! 
void context_switch(tinyPCB *current, tinyPCB * next) {
	int return_code = setjmp(current->context);
	if(return_code == 0) {
  	current_process = next;
		longjmp(next->context, 1);
	}
}


/** Process logic for two processes 
void proc_A() {
  while(1) {
		printf("Hi I'm process A\n");
		sleep(1);
		context_switch(&procA, &procB);
  }
}

void proc_B() {
	while(1) {
		printf("Hi I'm process B\n");
		sleep(1);
		context_switch(&procB, &procA);
  }
}

int main(int argc, char**argv) {
	jmp_buf tmp;
	int i;

	tinyPCB *initTable[2];
	procA.addr = proc_A;
	procA.sp   = malloc(STACK_SZ)+STACK_SZ-4;
	procB.addr = proc_B;
	procB.sp   = malloc(STACK_SZ)+STACK_SZ-4;
	
	initTable[0] = &procA;
	initTable[1] = &procB;


  for(i=0; i < 2; i++) {
		if(setjmp(tmp) == 0) {
		  tinyPCB *proc = initTable[i];
      stackInit = proc->sp;
 			__asm__ ("movl %0,%%esp":"=m" (stackInit));

			if(setjmp(proc->context) == 0) {
				longjmp(tmp, 1);
			} else {
  			startAddress start = current_process->addr;
				start();
			}
		}
	}

  current_process = &procA;
	longjmp(procA.context, 1);

	return 0;
}
*/







