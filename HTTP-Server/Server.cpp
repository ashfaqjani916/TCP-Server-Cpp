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
	string test;
	Data()	
	{

	}
};

class student : public Data{
public:

	student(string n) {
		test = n;
	}
	string GetFileData()
	{
		ifstream inputFile("names.txt");

		if (!inputFile.is_open())
		{
			cerr << "ERROR OPENING THE FILE.........!!!!" << endl;
		}
		string line;

		bool c = true;
		while (getline(inputFile, line))
		{

			int len = line.size();
			size_t pos = line.find(",");
			string key = line.substr(0, pos);
			string value = line.substr(pos + 1, len - 1);

			bool c = true;

			if (key == test) {
				return value;
				c = false;
			}


		}
		inputFile.close();
		if (c == true) {
			string fail = "not found :(";
			return fail;
		}
	}
};

class calculator : public Data {
public:

	calculator() {

	}
	calculator(string n) {
		test = n;
	}

	string calc() {
		int len = test.size();
		size_t pos = test.find(",");
		string testsub;
		testsub = test.substr(pos + 1);
		size_t pos1 = pos+testsub.find(",");

		string op1 = test.substr(0, pos);
		string op2 = test.substr(pos + 1, pos1);

		char op = test[len - 1];

		double x = stod(op1);
		double y = stod(op2);
		double result;

		switch (op) {
		case '+':
			result = x + y;
			break;
		case '-':
			result = x - y;
			break;
		case '*':
			result = x * y;
			break;
		case '/':
			result = x / y;
			break;
		case '%':
			int a = stoi(op1);
			int b = stoi(op2);
			result = a % b;
			break;
		}

		string msg = to_string(result);

		return msg;
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
	char buf[4096]; // 1024*4 bytes
	while (true)
	{
		ZeroMemory(buf, 4096);

<<<<<<< HEAD
=======
<<<<<<< HEAD
		// initialize the data into an array of vector of pairs

		Data obj;
		obj.GetFileData();

=======
		// buf = "AP22110011164"

>>>>>>> 77b3a7dc66b78bf7571ae40f228ec344a86a8e68
		
>>>>>>> b9deef4770983bbbb8f6d5cb5546e718e710b960
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

		cout << string(buf, 0, bytesRecieved) << endl;

		string nam = buf;
		string funrecieve;

		if (nam[0] != 'A') {
			calculator ob(nam);
			funrecieve = ob.calc();
		}

		else if (nam[0] == 'A') {
			student obj(nam);
			funrecieve = obj.GetFileData();
		}
		

		const char* thename = NULL;


		

		thename = funrecieve.c_str();

		if (funrecieve == "not found :(") {
			cout << "The client is not from R section" << endl;
		}
			


		//message back to client
		send(clientSocket,thename, 100, 0); // Size of the sending message is restricted to be in the size of recieving message - Resolved

	}



	//close the socket
	closesocket(clientSocket);

	//clean up winsock
	WSACleanup();

}