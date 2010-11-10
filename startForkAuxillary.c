#include "Struct.h"
#include "startProcesses.h"

void forkAuxillaryProcess(){

int bufferSize = BUFFERSIZE;
int statusTruncuate = -999;		//used to create the shared memory
keyboardFilename = "dummyKeyboardFile";  //the name of the shared_memory file
CRTFilename = "dummyCRTFile";
int in_pid = -999;

	 // Since we will spawn both the keyboard and crt processes here we can factor out operations that are common to both.
	 // For example we factor out obtaining the PID of the RTX process.

	int RTXpid = getpid();			// get current process pid of the RTX
	char argumentRTXpid[20];
	sprintf(argumentRTXpid, "%d", RTXpid); // convert to string to pass to child

	/////////////////////////Keyboard Fork Specific ////////////////////////////

  /* Create a new mmap file for read/write access with permissions restricted
     to owner rwx access only */
  keyboardFileIdentifier = open(keyboardFilename, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );

  if (keyboardFileIdentifier < 0){
  	
 	   printf("Bad Open of mmap file <%s>\n", keyboardFilename);
		 exit(0);
  }

  // make the file the same size as the buffer
  statusTruncuate = ftruncate(keyboardFileIdentifier, bufferSize);
  if (statusTruncuate){
      printf("Failed to ftruncate the file <%s>, status = %d\n", keyboardFilename, statusTruncuate );
      exit(0);
  }

	char argumentKeyboardFID[20]; // arguments to pass to child process(es)
	sprintf(argumentKeyboardFID, "%d", keyboardFileIdentifier);   // convert the file identifier to a string to pass to the child.


	// create the keyboard reader process
	// fork() creates a second process identical to the current process,
	// except that the "parent" process has in_pid = new process's ID,
	// while the new (child) process has in_pid = 0.
	// After fork(), we do execl() to start the actual child program.
	// (see the fork and execl man pages for more info)

	in_pid = fork();
	if (in_pid == 0)	// is this the child process ?
	{
		execl("./UNIXkbd.o", "keyboard", argumentRTXpid, argumentKeyboardFID, NULL);
		// should never reach here
		fprintf(stderr,"demo: can't exec keyboard, errno %d\n",errno);
		cleanup();
		exit(1);
	}
	keyboardChildPID = in_pid;

	// the parent process continues executing here
	// sleep for a second to give the child process time to start
	sleep(1);

	// allocate a shared memory region using mmap
	// the child process also uses this region

    keyboardmmap_ptr = mmap((caddr_t) 0,   /* Memory location, 0 lets O/S choose */
		    bufferSize,              /* How many bytes to mmap */
		    PROT_READ | PROT_WRITE, /* Read and write permissions */
		    MAP_SHARED,    /* Accessible by another process */
		    keyboardFileIdentifier,           /* the file associated with mmap */
		    (off_t) 0);    /* Offset within a page frame */
    if (keyboardmmap_ptr == MAP_FAILED){
      printf("Parent's memory map has failed, about to quit!\n");
	  cleanup();  // do cleanup and terminate
    }

	keyboardSharedMemPointer = (struct Buffer *) keyboardmmap_ptr;   // pointer to shared memory
	  // we can now use 'in_mem_p' as a standard C pointer to access
	  // the created shared memory segment
		// now start doing whatever work you are supposed to do
		// in this case, do nothing;
	keyboardSharedMemPointer->completedFlag = 0;


	////////////////////////////////////CRT PROCESS ////////////////////////////

	//will essentially do the same thing as the keyboard process.

	 Create a new mmap file for read/write access with permissions restricted
     to owner rwx access only 
  CRTFileIdentifier = open(CRTFilename, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );

  if (CRTFileIdentifier < 0){
 	   printf("Bad Open of mmap file <%s>\n", CRTFilename);
		 exit(0);
  }

  // make the file the same size as the buffer
  statusTruncuate = ftruncate(CRTFileIdentifier, bufferSize );
  if (statusTruncuate){
      printf("Failed to ftruncate the file <%s>, status = %d\n", CRTFilename, statusTruncuate );
      exit(0);
  }

	char argumentCRTFID[20]; // arguments to pass to child process(es)
	sprintf(argumentCRTFID, "%d", CRTFileIdentifier);   // convert the file identifier to a string to pass to the child.


	// create the keyboard reader process
	// fork() creates a second process identical to the current process,
	// except that the "parent" process has in_pid = new process's ID,
	// while the new (child) process has in_pid = 0.
	// After fork(), we do execl() to start the actual child program.
	// (see the fork and execl man pages for more info)

	in_pid = fork();
	if (in_pid == 0)	// is this the child process ?
	{
		execl("./UNIXcrt.o", "CRT", argumentRTXpid, argumentCRTFID, NULL);
		// should never reach here
		fprintf(stderr,"demo: can't exec CRT, errno %d\n",errno);
		cleanup();
		exit(1);
	}
	CRTChildPID = in_pid;

	// the parent process continues executing here
	// sleep for a second to give the child process time to start
	sleep(1);

	// allocate a shared memory region using mmap
	// the child process also uses this region

    CRTmmap_ptr = mmap((caddr_t) 0,
		    bufferSize,
		    PROT_READ | PROT_WRITE, 
		    MAP_SHARED, 
		    CRTFileIdentifier,           
		    (off_t) 0); 

    if (CRTmmap_ptr == MAP_FAILED){
      printf("Parent's memory map has failed, about to quit!\n");
	  cleanup();
    }

	CRTSharedMemPointer = (struct Buffer *) CRTmmap_ptr;   // pointer to shared memory
	  // we can now use 'in_mem_p' as a standard C pointer to access
	  // the created shared memory segment
		// now start doing whatever work you are supposed to do
		// in this case, do nothing;
	CRTSharedMemPointer->completedFlag = 0;
	
	


}


