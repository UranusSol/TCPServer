#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h> 
#include <vector>


#pragma  comment (lib, "ws2_32.lib")


#define SIZE = 4096


class TCPClient
{
private:
	int sock;
	std::string address;
	int port;
	struct sockaddr_in server;


public:
	TCPClient();
	bool setup(std::string address, int port);
	bool Send(std::string data);
	std::string receive();
	
};