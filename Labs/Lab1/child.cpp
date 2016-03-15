#ifdef _WIN32
#include <windows.h>
#include <stdio.h> 
#include <iostream>
#elif __linux__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif
using namespace std;

#ifdef _WIN32
int main()
{
	HANDLE hPipe;
	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe");
	DWORD BytesToRead;
	DWORD BytesToWrite;
	int i;
	Sleep(3000);
	hPipe = CreateFile(
		PipeName,	      
		GENERIC_READ |  
		GENERIC_WRITE,
		0,             
		NULL,         
		OPEN_EXISTING,  
		0,              
		NULL);         
	ReadFile(hPipe, &i, sizeof(int), &BytesToRead, NULL);
	i = i*i;
	WriteFile(hPipe, (LPVOID)&i, sizeof(int), &BytesToWrite, NULL);
}
#elif __linux__
int main(int argc, char *argv[])
{
	int fd[2];
	int res;

	fd[0] = atoi(argv[0]);
	fd[1] = atoi(argv[1]);


	read(fd[0], &res, sizeof(int));
	close(fd[0]);

	res = res*res;

	write(fd[1], &res, sizeof(int));
	close(fd[1]);
	
	usleep(5000000);
	return 0;
}
#endif
