#include "engine.h"

void crash() {
	logtofile("-------------------CRASH!!!!-------------------", SVR, "Crash");
	
	char stackTraceMessage[256];
	logtofile("Warning: To prevent segfaults/UB after crash during init, the program will terminate.", SVR, "Crash");
	logtofile("This can and will lead to memory leaks! Please beware!", SVR, "Crash");
	sprintf(stackTraceMessage, "Printing stack trace (up to %d entries, changeable in config): ", STACK_TRACE_LIMIT);
	logtofile(stackTraceMessage, SVR, "Crash");
	
	void *array[STACK_TRACE_LIMIT];
  	char **strings;
  	int size;

	size = backtrace(array, STACK_TRACE_LIMIT);
	strings = backtrace_symbols(array, size);

	if (strings != NULL) {
	    sprintf(stackTraceMessage, "Obtained %d stack frames.", size);
	    logtofile(stackTraceMessage, SVR, "Crash");

	    for (int i = 0; i < size; i++) {
	    	logtofile(strings[i], SVR, "Crash");
	    }
	}

	gfree(strings);
	exit(-1);
}

void signalHandler(int sig, siginfo_t *info, void *ucontext) {
	char signalMessage[1024];
	ucontext = ucontext;
	switch(sig) {
		case SIGSEGV:
			sprintf(signalMessage, "Signal caught! SIGSEGV at addr. 0x%lx. CRASHING!", (long)info->si_addr);
			logtofile(signalMessage, SVR, "Signal Handler");
			crash();
		
		case SIGINT:
			logtofile("Debug interupt caught!", INF, "Signal Handler");
			return;

		default:
			logtofile("Unhandled signal caught, crashing!", SVR, "Signal Handler");
			crash();
	}
}



void initSignalHandler() {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = signalHandler;
	if (sigaction(SIGSEGV, &sa, NULL) == -1) {
		logtofile("sigaction call failed, these are dark days.", SVR, "Something is TERRIBLY wrong!");
		exit(-1);
	}
}

 