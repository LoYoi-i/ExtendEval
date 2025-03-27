#pragma once

#include <windows.h>

typedef void (*ProcessMessage)(struct SPBasicSuite*, const char*, char*);

int pipeServer(LPCWSTR, struct SPBasicSuite*, ProcessMessage);
