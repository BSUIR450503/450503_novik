#pragma once

#include "Libraries.h"
#include "Func.h"

#ifdef _WIN32
#include "Proc.h"
#endif

class Handle
{
public:
	void start(int, char**);
};

void Handle::start(int argNum, char **value)
{
	if (argNum == 2)
	{
#ifdef _WIN32

		char toClose[MAX_NUM];
		strcpy(toClose, value[1]);
		strcat(toClose, value[1]);

		HANDLE canPrintEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, value[1]);
		HANDLE canCloseEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, toClose);
		while (1)
		{
			if (WaitForSingleObject(canPrintEvent, 0) == WAIT_OBJECT_0)
			{
				print(atoi(value[1]));
				ResetEvent(canPrintEvent);
			}
			if (WaitForSingleObject(canCloseEvent, 0) == WAIT_OBJECT_0)
			{
				CloseHandle(canCloseEvent);
				CloseHandle(canPrintEvent);
				return;
			}
		}
#endif
	}
	else
	{
		char key;
		int numOfProcess = 0;
		int currentNum = 1;
		PROCESS_ID process[MAX_NUM];

#ifdef _WIN32

		while (1)
		{
			key = catch_symbol();
			switch (key)
			{
				case '+':
				{
					if (numOfProcess < MAX_NUM)
					{
						numOfProcess++;
						process[numOfProcess - 1].createNewProcess(value[0], numOfProcess);
					}
					break;
				}
				case '-':
				{
					if (numOfProcess > 0)
					{
						SetEvent(process[--numOfProcess].getCloseEvent());
					}
					break;
				}
				case 'q':
				{
					if (numOfProcess > 0)
					{
						for (int i = 0; i < numOfProcess; i++)
						{
							SetEvent(process[i].getCloseEvent());
						}
					}
					return;
				}
			}
			if (numOfProcess > 0 && WaitForSingleObject(process[currentNum - 1].getPrintEvent(), 1) == WAIT_TIMEOUT)
			{
				if (currentNum >= numOfProcess)
				{
					currentNum = 1;
				}
				else
				{ 
					currentNum++;
				}
				SetEvent(process[currentNum - 1].getPrintEvent());
			}
		}

#elif __linux__

		initscr();
		clear();
		noecho();
		refresh();

		toPrintSignal.sa_handler = setPrint;
		sigaction(SIGUSR1, &toPrintSignal, NULL);

		toEndSignal.sa_handler = setEnd;
		sigaction(SIGUSR2, &toEndSignal, NULL);

		while (1)
		{
			key = getch_noblock();

			Sleep(SHORT_SLEEP);
			switch (key)
			{
			case '+':
			{
				if (numOfProcess < MAX_NUM)
				{
					process[numOfProcess] = fork();
					numOfProcess++;

					switch (process[numOfProcess - 1])
					{
					case 0:
					{
						endFlag = 0;
						while (!endFlag)
						{
							Sleep(SHORT_SLEEP);
							if (printFlag)
							{
								print(numOfProcess);
								refresh();
								printFlag = 0;
								kill(getppid(), SIGUSR2);
							}
						}
						return;
						break;
					}
					case -1:
					{
						cout << "CreateProcess failed!" << endl;
						break;
					}
					default:
					{
						break;
					}
					}
				}
				break;
			}

			case '-':
			{
				if (numOfProcess > 0)
				{
					kill(process[--numOfProcess], SIGUSR2);
					waitpid(process[numOfProcess], NULL, 0);
					kill(process[numOfProcess], SIGKILL);
					waitpid(process[numOfProcess], NULL, 0);
				}
				break;
			}

			case 'q':
			{
				if (numOfProcess > 0)
				{
					for (; numOfProcess > 0; numOfProcess--)
					{
						kill(process[numOfProcess - 1], SIGUSR2);
						waitpid(process[numOfProcess - 1], NULL, 0);
						kill(process[numOfProcess - 1], SIGKILL);
						waitpid(process[numOfProcess - 1], NULL, 0);
					}
				}
				clear();
				endwin();
				return;
			}
			}

			// the same to windows 
			if (endFlag && numOfProcess > 0)
			{
				endFlag = 0;
				if (currentNum >= numOfProcess)
				{
					currentNum = 1;
				}
				else
				{
					
					currentNum++;
					
				}
				kill(process[currentNum - 1], SIGUSR1);
			}
			refresh();
		}

#endif

	}
}