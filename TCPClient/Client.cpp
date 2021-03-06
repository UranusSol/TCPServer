// TCPClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "TCPClient.h"
#include <string>
#include <thread>




int main(int argc, char *argv[])
{
	if (argc >= 2)
	{
		TCPClient tcp;
		std::string adress = argv[1];
		int port = atoi(argv[2]);
		tcp.setup(adress, port);
		std::cout << adress << ":" << port << std::endl;
		//std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "client started" << std::endl;
		while (true)
		{
			std::string msg;
			std::getline(std::cin, msg);
			if (!std::cin.fail()) {
				tcp.Send(msg);
				std::string rec = tcp.receive();
				if (rec != "")
				{
					std::cout << "Server Response:" << rec << std::endl;
				}
				
			}
		}
		exit(0);
		return 0;
	}
	else
		std::cerr << "invalid ip port" << std::endl;
	    std::this_thread::sleep_for(std::chrono::seconds(3));
	return 1;
}