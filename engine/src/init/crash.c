#include "../../includes/engine.h"

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

	free(strings);
	abort();
}