#include<iostream>
#include <limits>
#include<string>
#include<ios>
#include <algorithm>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")



using namespace std;

void main() {
	string  ipAddress = "10.1.45.88"; // IP addresss of the server
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
		WSACleanup; // Cleaning the memory and network ports, winsocket
		return;
	}
	// Dp-while loop to send and recieve data

	char buf[4096]; // 1024*4*1 bytes // used to recieve/store the message from the server
	string userInput;


	cout << "Welcome to the Server!" << endl;
	do {
		// Prompt the user for some text
		char select;
		cout << "SERVER> Please select required option below:" << endl;
		cout << "        1. Data Base" << endl;
		cout << "        2. Basic Calculator" << endl;
		cout << "        3.Exit" << endl;
		cin >> select;


		if (select == '1') {
			string userInput;
			cout << "Please enter the Roll Number for searching in the database." << endl;
			cout << "YOU> ";
			cin >> userInput;
			cout << "Your data will be recieved by the server in a while. Please wait..." << endl;

			if (userInput.size() > 0) {  // Make sure the user has typed in something 
				// Send the text

				// Utility or Data Base selection
				int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); // To the server
				if (sendResult != SOCKET_ERROR) {
					// Wait for the response
					// string - char array
					// string -> const char* conversion ---- .c_str()

					ZeroMemory(buf, 4096);
					int bytesRecieved = recv(sock, buf, 4096, 0);
					if (bytesRecieved > 0) {
						// Data response to console

						string recieved = buf;

						if (recieved != "not found :(") {
							cout << "SERVER> " << "Data recieved from the server successfully!" << endl;
							cout << "SERVER> " << "Detail of the student with the given Roll Number:" << endl;;
							cout << "        Name: ";
							cout << string(buf, 0, bytesRecieved) << endl;
							cout << "        Roll Number: " << userInput << endl << endl;
						}

						else if (recieved == "not found :(") {
							cout << "SERVER> ";
							cout << "The student you are searching is not present in the data base :(" << endl;
							cout << "        Enter a valid Roll Number for searching next time." << endl;
							cout << "        You will be redirected to the searching page." << endl << endl;
						}

					}

				}


			}
		}

		else if (select == '2') {
			string userInput;
			cout << "SERVER> Please enter the operands and operators in the following format." << endl;
			cout << "        operand,operand,operator" << endl;
			cout << "YOU> ";
			cin >> userInput;
			cout << "SERVER> Your data will be recieved by the server in a while. Please wait..." << endl;

			if (userInput.size() > 0) {  // Make sure the user has typed in something 
				// Send the text

				// Utility or Data Base selection
				int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); // To the server
				if (sendResult != SOCKET_ERROR) {
					// Wait for the response
					// string - char array
					// string -> const char* conversion ---- .c_str()

					ZeroMemory(buf, 4096);
					int bytesRecieved = recv(sock, buf, 4096, 0);
					if (bytesRecieved > 0) {
						// Data response to console

						string recieved = buf;


						cout << "SERVER> The result:";
						cout << string(buf, 0, bytesRecieved) << endl << endl;

					}

				}


			}
		}

		else if (select == '3') {
			cout << "Exiting the Connection!" << endl;
			return;
		}
		else cout << "Invalid input!" << endl << "Please eneter a valid choice from the below.";

		userInput = "true";




	} while (userInput.size() > 0);

	// Shutting down

	closesocket(sock);
	WSACleanup();

}