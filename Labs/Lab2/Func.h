#pragma once

#include "Libraries.h"


#ifdef __linux__

int printFlag = 0;
int endFlag = 1;

struct sigaction toPrintSignal;
struct sigaction toEndSignal;

void setPrint(int sig) {
	printFlag = 1;
}

void setEnd(int sig) {
	endFlag = 1;
}

void tempSleep(int slpTime)
{
	usleep(slpTime);
	fflush(stdout);
	refresh();
}
#endif

#ifdef _WIN32

int catch_symbol()
{
	if (_kbhit())
		return _getch();
	else
		return -1;
}

#endif

void print(int processNumber)
{
	for (int i = 0; i < strlen(processesNames[processNumber - 1]); i++)
	{
		cout << processesNames[processNumber - 1][i];
		Sleep(LONG_SLEEP);
	}

	return;
}
