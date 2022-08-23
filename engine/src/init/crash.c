#include "engine.h"

/**
 * @brief      Handles crashing signals
 */
#ifndef __WIN32__
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

/**
 * @brief      Handles the signal passed in
 *
 * @param[in]  sig       The signal
 * @param      info      The information
 * @param      ucontext  The ucontext
 */
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

		case SIGFPE:
			logtofile("Fatal arithmetic error! (its probably a division by zero)", SVR, "Signal Handler");
			crash();

		default:
			logtofile("Unhandled signal caught, crashing!", SVR, "Signal Handler");
			printf("unhandled signal id: %d\n", sig);
			crash();
	}
}



/**
 * @brief      Creates the signal handler
 */
void initSignalHandler() {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signalHandler;
	sigfillset(&sa.sa_mask);
	if (sigaction(SIGSEGV, &sa, NULL) == -1) {
		logtofile("sigaction call failed, these are dark days.", SVR, "Something is TERRIBLY wrong!");
		exit(-1);
	}
	if (sigaction(SIGFPE, &sa, NULL) == -1) {
		logtofile("sigaction call failed, these are dark days.", SVR, "Something is TERRIBLY wrong!");
		exit(-1);
	}
}

 
#else
void crash() {
	logtofile("-------------------CRASH!!!!-------------------", SVR, "Crash");
	
	logtofile("Warning: To prevent segfaults/UB after crash during init, the program will terminate.", SVR, "Crash");
	logtofile("This can and will lead to memory leaks! Please beware!", SVR, "Crash");

	exit(-1);
}

void initSignalHandler() {
	return;
}
#endif

