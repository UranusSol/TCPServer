#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <winsock.h>
#include <thread>
#include <string>


#pragma  comment (lib, "ws2_32.lib")



#define MAXPACKETSIZE 4096

class TCPServer
{
public:
	int sockfd, newsockfd, n;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	std::thread serverThread;
	char msg[MAXPACKETSIZE];
	static std::string Message;

	void setup(int port);
	std::string receive();
	std::string getMessage();
	void Send(std::string msg);
	void closeserver();
	void clean();


private:
	static void * Task(void * argv);
};