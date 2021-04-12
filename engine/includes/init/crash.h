#pragma once
#define INIT_CRASH_H

void crash() __attribute__((noreturn));
void initSignalHandler();