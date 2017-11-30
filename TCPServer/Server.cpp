#include "stdafx.h"

#include <iostream>
#include "TCPServer.h"
#include <sstream>


TCPServer tcp;


void * loop(void * m)
{

	while (1)
	{
		std::string str = tcp.getMessage();
		if (str != "")
		{
			std::cout << "Message:" << str << std::endl;
			//tcp.Send(tcp.handleMessage(str));
			tcp.clean();
		}
		//	std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	tcp.closeserver();
}

int main(int argc, char* argv[])
{
	int port;
	std::istringstream ss(argv[1]);
	if (!(ss >> port))
	{
		std::cerr << "Invalid port number " << argv[1] << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return 0;
	}
	tcp.setup(port);
	std::thread msg(loop, (void *)0);
	msg.detach();
	tcp.receive();


	return 0;
}


