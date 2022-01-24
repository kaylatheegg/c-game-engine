#include "engine.h"

FILE *logptr;

/**
 * @brief      Initializes the log.
 *
 * @return     Returns log status
 */
int initLog() {
	logptr = fopen("log/log.log", "w+");
	
	if (logptr == NULL) {
		printf("Cannot open log!\n");
		return 0;
	}

	fclose(logptr);
	

	time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);


	char time[64];
	sprintf(time, "Current day is: %d/%d/%d", timeinfo->tm_mday, timeinfo->tm_mon, (timeinfo->tm_year + 1900));
	logtofile(time, INF, "Logger");
	return 1;
}