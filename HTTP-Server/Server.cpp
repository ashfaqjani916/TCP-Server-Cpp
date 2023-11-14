#include <iostream>
#include <WS2tcpip.h>     //contains the API/Framework that windows uses to access network sockets

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	//initialize a winsock
	WSADATA wsaDate;

	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsaDate);

	if (wsOk != 0)
	{
		cerr << "Can't Initialise WinSock,Quitting......" << endl;
	}
	//create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	//bind the Ip and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //could also use inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));


	//tell winsock that the socket if for listening 
	listen(listening, SOMAXCONN);


	//wait for connection
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	char host[NI_MAXHOST];		//client's remote name
	char service[NI_MAXHOST];	//Service (i.e. port) the client is connnected on 

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "connected on port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected  on port" << ntohs(client.sin_port) << endl;
	}



	//close listening socket
	closesocket(listening);

	//while loop:  accept and echo message back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);



		// wait for the client to send data
		int bytesRecieved = recv(clientSocket, buf, 4096, 0);
		if (bytesRecieved == SOCKET_ERROR)
		{
			cerr << "ERROR in recv(). Quitting" << endl;
			break;
		}
		if (bytesRecieved == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}

		cout << string(buf, 0, bytesRecieved) << endl;

		//echo message back to client
		send(clientSocket, buf, bytesRecieved + 1, 0);

	}



	//close the socket
	closesocket(clientSocket);

	//clean up winsock
	WSACleanup();

}