#include <signal.h>
#include "Processes.h"

//The method here will associate each unix signal with a particular method to call .

void signalAssociation(){

// all signals will be caught and handled by the signal handler.
sigset(SIGINT,cleanup);
sigset(SIGBUS,cleanup);
sigset(SIGHUP,cleanup);
sigset(SIGILL,cleanup);
sigset(SIGQUIT,cleanup);
sigset(SIGABRT,cleanup);
sigset(SIGTERM,cleanup);
sigset(SIGSEGV,cleanup);

//USR defined signals. (the ones we are interested in).
sigset(SIGUSR2,iProcessKeyboard);
sigset(SIGUSR1,iProcessCRT);
sigset(SIGALRM,iProcessAlarm);

}
