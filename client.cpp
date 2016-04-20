/************************************
	client.cpp
*************************************/
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

void error(const char* message){
	perror(message);
	exit(1);
}

int main(int argc, char* argv[]){

	int socketFD, portNumber;
	struct sockaddr_in serverAddr;
	struct hostent *server;
	char writeBuffer[256];
	char readBuffer[256];

	if(argc < 3){
		fprintf(stderr, "Error, incomplete arguments!\n");
		exit(1);
	}
	portNumber = atoi(argv[2]);

	// Create a socket with the socket() system call
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0)
		error("Error opening socket");
		
	// Obtain server details by using the gethostbyname system call()
	bzero((char*)&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);

	server = gethostbyname(argv[1]);
	if (server == NULL){
		fprintf(stderr, "ERROR, no such host\n");
		exit(1);
	}

	bcopy((char*)server->h_addr, (char*)&serverAddr.sin_addr.s_addr, server->h_length);

	// Connect the socket to the address of the server using the connect() system call
	if(connect(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error("ERROR connecting!");

	// Send and receive data
	cout << "Please enter a message: ";
	cin.getline(writeBuffer, sizeof(writeBuffer));
	if (write(socketFD, writeBuffer, strlen(writeBuffer)) < 0)
		error("ERROR writing to the socket!");

	bzero(readBuffer, sizeof(readBuffer));
	if(read(socketFD, readBuffer, sizeof(readBuffer)) < 0)
		error("ERROR reading from socket!");	
	cout << "\nThe server messsage is : " << readBuffer << endl;

	// Close file descriptors
	close(socketFD);

	return 0;
}