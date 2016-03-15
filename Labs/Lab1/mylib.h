#ifdef _WIN32
#include <windows.h>
#include <stdio.h> 
#include <iostream>
#define Create() createwinproc()
#elif __linux__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define Create() createlinuxproc()
#endif
using namespace std;

class proc
{
public:
	void createwinproc();
	int createlinuxproc();
};
#ifdef _WIN32
void proc::createwinproc()
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO SI;
	TCHAR  CommandLine[] = TEXT("Lab_1.1.exe");
	HANDLE hPipe;
	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe");
	DWORD BytesToRead;
	DWORD BytesToWrite;
	int i;
	ZeroMemory(&SI, sizeof(STARTUPINFO));
	SI.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piProcInfo, sizeof(piProcInfo));

	hPipe = CreateNamedPipe(
		PipeName,			  
		PIPE_ACCESS_DUPLEX,      
		PIPE_TYPE_MESSAGE |     
		PIPE_READMODE_MESSAGE |   
		PIPE_WAIT,                
		PIPE_UNLIMITED_INSTANCES, 
		sizeof(int),			   
		sizeof(int),		          
		NMPWAIT_USE_DEFAULT_WAIT, 
		NULL);                    
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
		return;
	}

	cout << "Vvod:";
	cin >> i;


	if ((CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, NULL, NULL, NULL, &SI, &piProcInfo)) == 0)
	{
		printf("create client process: error code %d\n", (int)GetLastError());
		return;
	}
	if ((ConnectNamedPipe(hPipe, NULL)) == 0)
	{
		printf("client could not connect\n");
		return;
	}
	WriteFile(hPipe, (LPVOID)&i, sizeof(int), &BytesToWrite, NULL);
	//Sleep(5000);
	ReadFile(hPipe, &i, sizeof(int), &BytesToRead, NULL);
	WaitForSingleObject(piProcInfo.hProcess, INFINITE);
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);
	cout << "Rez:" << i << endl;

}
#elif __linux__
int proc::createlinuxproc()
{
	printf("--beginning of program\n");
	int fd[2];		
	int num;		
	int res; 
	char fd0[8], fd1[8];

	if (pipe(fd) == -1)
	{
		printf("pipe() failed");
		return 1;
	}

	pid_t pid = fork();

	if (pid == 0)
	{
		// child process 
		snprintf(fd0, sizeof(fd0), "%d", fd[0]);
		snprintf(fd1, sizeof(fd1), "%d", fd[1]);
		execl("child", fd0, fd1, NULL);
		exit(0);
	}
	else if (pid > 0)
	{
		// parent process
		printf("Number:");
		scanf("%d", &num);
		write(fd[1], &num, sizeof(int));
		close(fd[1]);
		wait(NULL);
		read(fd[0], &res, sizeof(int));
		close(fd[0]);
		printf("The result is %d", res);
	}
	else
	{
		// fork failed
		printf("fork() failed!\n");
		return 1;
	}

	printf("\n--end of program--\n");
	return 0;
}
#endif
