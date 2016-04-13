#pragma once

#define MAX_NUM 10

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using std::cout;
using std::endl;

static const char * processesNames[] = { "1. First process\r\n",
									"2. Second process\r\n",
                                    "3. Third process\r\n", 
									"4. Fourth process\r\n",
                                    "5. Fifth process\r\n",	
									"6. Sixth process\r\n",
                                    "7. Seventh process\r\n", 
									"8. Eighth process\r\n",
                                    "9. Ninth process\r\n", 
									"10. Tenth process\r\n" };

#ifdef _WIN32

#include <Windows.h>
#include <conio.h>

#define SHORT_SLEEP 1
#define LONG_SLEEP 50
#define PROCESS_ID Proc

#elif __linux__

#include <curses.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define Sleep tempSleep
#define getch_noblock getchar
#define SHORT_SLEEP 1000
#define LONG_SLEEP 50000
#define PROCESS_ID pid_t

#endif