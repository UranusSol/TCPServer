#include "stdafx.h"
#include "TCPClient.h"
#include <string>
#include <thread>





TCPClient::TCPClient()
{
	sock = -1;
	port = 0;
	address = "";
}

bool TCPClient::setup(std::string address, int port)
{
	WSADATA wsData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsData) == 0) 
		if (sock == -1)
		{
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET)
			{
				std::cout << "Could not create socket" << std::endl;
				//std::this_thread::sleep_for(std::chrono::seconds(3));
			}
		}
		if (inet_addr(address.c_str()) == -1)
		{
			struct hostent *he;
			struct in_addr **addr_list;
			if ((he = gethostbyname(address.c_str())) == NULL)
			{

				std::cout << "Failed to resolve hostname\n";
				return false;
			}
			addr_list = (struct in_addr **) he->h_addr_list;
			for (int i = 0; addr_list[i] != NULL; i++)
			{
				server.sin_addr = *addr_list[i];
				break;
			}
		}
		else
		{
			server.sin_addr.s_addr = inet_addr(address.c_str());
		}
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			perror("connect failed. Error");
			std::this_thread::sleep_for(std::chrono::seconds(3));
			return 1;
		}
		return true;
	
}

bool TCPClient::Send(std::string data)
{
	if (send(sock, data.c_str(), data.length(), 0) < 0)
	{
		std::cout << "Send failed : " << data << std::endl;
		return false;
	}
	return true;
}

std::string TCPClient::receive()
{

	char buffer[4096];
	ZeroMemory(buffer, sizeof(buffer));
	std::string reply;
	if (int n = recv(sock, buffer, sizeof(buffer), 0) < 0) 
	{
		std::cout << "receive failed!" << std::endl;
	}
	reply = buffer;
	return reply;
}


