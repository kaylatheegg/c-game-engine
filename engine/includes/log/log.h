#pragma once
#define LOG_LOG_H

enum level {
	INF,
	WRN,
	ERR,
	SVR
};

int logtofile(const char* string, int type, const char* caller);