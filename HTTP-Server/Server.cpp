#include <iostream>
#include <WS2tcpip.h>  //contains the API/Framework that windows uses to access network sockets
#include <fstream>
#include <vector>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Data
{
public:
	vector <pair<string, string>> data[10];
	Data()
	{
		for (int i = 0; i < 10; ++i) {
			data[i].emplace_back("", "");
		}
	}
	void GetFileData()
	{
		ifstream inputFile("names.txt");

		if (!inputFile.is_open())
		{
			cerr << "ERROR OPENING FILE.........!!!!" << endl;
		}
		string line;
		while (getline(inputFile, line))
		{
			// Split the line into key and value using a delimiter (e.g., ":")
			std::size_t pos = line.find(":");
			if (pos != std::string::npos) {
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);

				int len = key.length();
				string num = line.substr(len - 2, pos);
				int val = stoi(num);
				// Create a key-value pair and add it to the vector
				std::pair<std::string, std::string> pair = std::make_pair(key, value);
				data[val/10].push_back(pair);
			}
		}
		inputFile.close();

	}
};

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


	//tell winsock that the socket is for listening 
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

		// initialize the data into an array of vector of pairs

		Data obj;
		obj.GetFileData();

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

		// check the value form the data
		string nam = buf;
		int len = nam.length();
		string num = nam.substr(len - 1, len);
		int val = stoi(num);
		bool c = true;
		const char* thename = nullptr;
		for (auto it : obj.data[val])
		{
			string comp = it.first;
			if (it.first == nam)
			{
				thename = it.second.c_str();
				c = false;
				break;
			}
		}
		if (c)
			cout << "You are not from R section......." << endl;

		thename = "hellobabuuu.....";

		cout << string(buf, 0, bytesRecieved) << endl;

		//echo message back to client
		send(clientSocket,thename, bytesRecieved + 1, 0);

	}



	//close the socket
	closesocket(clientSocket);

	//clean up winsock
	WSACleanup();

}