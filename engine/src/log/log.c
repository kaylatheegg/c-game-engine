#include "engine.h"


//logtofile is a function that takes a string e.g "hello there" and a type of log, e.g INF or SVR
//an example usage would be logtofile("hello there", INF);


int logtofile(const char* string, int type, const char* caller) {
	logptr = fopen("log/log.log", "a+");
	if (logptr == NULL) {
		printf("Cannot open log!\n");
		return 0;
	}
	const char* typeID;
	const char* fileTypeID;
	switch (type) {
		case INF:
			typeID = GRN"INFO"RESET;
			fileTypeID = "INFO";
			break;

		case WRN:
			typeID = YEL"WARN"RESET;
			fileTypeID = "WARN";
			break;
			
		case ERR:
			typeID = RED"ERROR"RESET;
			fileTypeID = "ERROR";
			break;
			
		case SVR:
			typeID = MAG"SEVERE"RESET;
			fileTypeID = "SEVERE";
			break;
	}

	time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
	fprintf(logptr, "[%02d:%02d:%02d] [%s] [%s] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, fileTypeID, caller, string);
	printf("[%02d:%02d:%02d] [%s] [%s] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, typeID, caller, string);
	fclose(logptr);
	return 1;
}
