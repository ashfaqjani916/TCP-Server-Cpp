#include<iostream>
#include<string>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main() {
	string  ipAddress = "192.168.0.160"; // IP addresss of the server
	int port = 54000; // Listening port

	// Initialize  winsocket

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	if (wsResult != 0) {
		cerr << "Can't start winsocket, ERR #" << wsResult << endl;
		return;
	}


	// Create socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket, ERR #" << WSAGetLastError() << endl;
		return;
	}

	// Fill in a hint structure

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);


	// Connect to server

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, ERRR #" << WSAGetLastError() << endl;
		WSACleanup;
		return;
	}
	// Dp-while loop to send and recieve data

	char buf[4096];
	string userInput;

	do {
		// Prompt the user for some text

		cout << "Welcome to the Server!" << endl;
		cout << "Please enter the Roll Number for searching in the database." << endl;
		cout << "> ";
		getline(cin, userInput);
		cout << "Your data will be recieved by the server in a while. Please wait..." << endl;

		if (userInput.size() > 0) {  // Make sure the user has typed in something 
			// Send the text

			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// Wait for the response

				ZeroMemory(buf, 4096);
				int bytesRecieved = recv(sock, buf, 4096, 0);
				if (bytesRecieved > 0) {
					// Data response to console
					cout << "SERVER> " << "Data recieved from the server successfully!" << endl;
					cout << "SERVER> " << "The name of the student with the given Roll Number is ";
					cout << string(buf, 0, bytesRecieved) << endl;

				}

			}


		}


	} while (userInput.size() > 0);

	// Shutting down

	closesocket(sock);
	WSACleanup();

}