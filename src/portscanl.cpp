#include <winsock2.h>
#include <stdio.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include "ThreadPool.h"

#pragma comment(lib, "WS2_32.lib")

using namespace std;

void ScanThread(int port);

int main(int argc, char *argv[])
{
    WSADATA wsd;
    int port;
    int MAX_PORT = 65535;
    clock_t start, end;
    HANDLE handle;
    DWORD dwThreadId;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("Startup failed!\n");
        return 1;
    }
    start = clock();
    ThreadPool pool(5000);
    for (port = 1; port <= MAX_PORT; port++)
    {
        pool.enqueue([port]
                     { ScanThread(port); });
    }
    pool.waitAll();
    end = clock();
    float duration = end - start;
    printf("spend %.2f s", duration / 1000);
    return 0;
}

void ScanThread(int port)
{
    int Port = port;
    int retval;
    SOCKET sHost;
    SOCKADDR_IN servAddr;
    sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == sHost)
    {
        printf("socket failed!\n");
        WSACleanup();
        return;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(Port);
    // setsockopt(sHost, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    // setsockopt(sHost, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
    retval = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
    if (retval == SOCKET_ERROR)
    {
        closesocket(sHost);
        return;
    }
    printf("[+]\t%d\n", Port);
    closesocket(sHost);
    return;
}
