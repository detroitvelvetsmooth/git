#include "Struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAXCHAR 4

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


int main(){
    int count;
    count = 512;
    char temp[5];
    char tab[5];
    
    tab[0] = 0;
    itoa(count, temp, 10);  //Copy contents of count into temp, count is base 10
    
    
    printf("Temp Contents: %s\n", temp);
    printf("Temp Length: %d\n", strlen(temp)); 
    
    strcat(temp, "5");
    
    printf("Temp Contents: %s\n", temp);
    printf("Temp Length: %d\n", strlen(temp)); 
    
    count = atoi(temp); //Copy contents of temp back to count, converts it to base 10 int
    
    printf("Temp Contents: %s\n", temp);    
    printf("Temp Length: %d\n", strlen(temp));
    count++;
    printf("Count Value: %d\n", count);
    
    //EXAMPLE OF PARSING
    char str[] ="hh:mm:ss";
    char * pch;
    printf ("Splitting string \"%s\" into tokens:\n",str);
    pch = strtok (str,":"); // Gets the first parsed section
    while (pch != NULL)
    {
      printf ("%s\n",pch);
      pch = strtok (NULL, ":"); //Retrieve remaining tokens (parsed sections).
    }
    
    system("PAUSE");
    return 0;
    
}


