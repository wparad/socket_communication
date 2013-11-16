// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#include "targetver.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
//#include <tchar.h>

#define LOG_STRING_SIZE 80
#define TIME_SIZE 26
#define BUFFER_SIZE 256
#define ERROR_SOCKET_NOT_OPEN 10
#define ERROR_SOCKET_NOT_BIND 20
#define ERROR_SOCKET_NOT_ACPT 30
#define ERROR_SOCKET_NOT_FORK 40

