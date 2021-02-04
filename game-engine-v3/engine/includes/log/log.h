#pragma once
#define LOG_LOG_H

enum level {
	INF,
	WRN,
	ERR,
	SVR
};

int logtofile(char* string, int type, char* caller);