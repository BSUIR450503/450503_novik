#include "Libraries.h"

class Proc
{
private:
	PROCESS_INFORMATION processInfo;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE printEvent;
	HANDLE closeEvent;
public:
	PROCESS_INFORMATION createNewProcess(char*, int);
	HANDLE getPrintEvent();
	HANDLE getCloseEvent();
};

PROCESS_INFORMATION Proc::createNewProcess(char *path, int processNumber)
{
	char procNum[MAX_NUM];
	char toClose[MAX_NUM];
	char command[MAX_NUM] = "-n ";

	_itoa(processNumber, procNum, 10);
	strcat(command, procNum);

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(path, command, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
	{
		cout << "CreateProcess failed!";
	}

	strcpy(toClose, procNum);
	strcat(toClose, procNum);

	closeEvent = CreateEvent(NULL, FALSE, FALSE, toClose);
	printEvent = CreateEvent(NULL, TRUE, FALSE, procNum);

	return pi;
}

HANDLE Proc::getPrintEvent()
{
	return printEvent;
}

HANDLE Proc::getCloseEvent()
{
	return closeEvent;
}