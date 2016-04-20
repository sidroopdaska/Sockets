/************************************
	server.cpp
*************************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
void handleConnection(int, int);
void error(const char* msg){
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]){
	int socketFD, newSocketFD, portNumber, numberOfClients = 0;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientLength;

	// Initializing buffers to zero.
	bzero((char*)&serverAddr, sizeof(serverAddr));
	
	// Obtaining port number from the command line arguments
	if(argc < 2){
		fprintf(stderr, "ERROR, no port provided!\n");
		exit(1);
	}	

	portNumber = atoi(argv[1]);

	// Setting up a socket for - Domain: Internet, Type: Stream and Protocol: TCP/IP
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 1)
		error("ERROR opening socket");
	
	// Setting up Server IP Address and Port Number (as provided by the user)
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	
	// Binding the Socket to the Server IP Addr + Port Number details
	fprintf(stdout, "=> Server socket has been created...\n");
	if(bind(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error("ERROR binding socket");

	// Listen for connection requests from Clients
	listen(socketFD, 5);

	// Accept a connection with the accept() system call. This call typically blocks
	// until a client connects with the server.
	clientLength =  sizeof(clientAddr);
	while(1){
		fprintf(stdout, "\n\n=> Looking for clients...\n");
		
		newSocketFD = accept(socketFD, (struct sockaddr*)&clientAddr, &clientLength);
		if (newSocketFD < 0) 
			error("ERROR making a new connection");
		
		numberOfClients++;
		fprintf(stdout, "=> Connected with the client #%d.\n", numberOfClients);
		
		int pid = fork();
		if(pid < 0)
			error("Error on Fork");

		if(pid == 0){
			close(socketFD);
			handleConnection(newSocketFD, numberOfClients);
			fprintf(stdout, "Terminated connection with client #%d", numberOfClients);
			exit(0);
		} else {
			close(newSocketFD);
		}
	}
	
	close(socketFD);
	return 0;
}

void handleConnection(int newSocketFD, int clientID){

	char readBuffer[256];
	char writeBuffer[256];
	bzero(writeBuffer, sizeof(readBuffer));
	bool stop = false;

	while(!stop){
		// Reading and writing data over the socket once a connection has been established.
		bzero(readBuffer, sizeof(readBuffer));
		if(read(newSocketFD, readBuffer, sizeof(readBuffer)) < 0)
			error("ERROR reading from socket!");
		fprintf(stdout, "Client #%d: %s\n", clientID, readBuffer);

		if(strcmp(readBuffer, "#") == 0){
			strncpy(writeBuffer, "Terminating connection...", sizeof(writeBuffer));
			stop = true;
		} else 
			strncpy(writeBuffer, "I got your message", sizeof(writeBuffer));

		if(write(newSocketFD, writeBuffer, strlen(writeBuffer)) < 0)
			error("ERROR writing to the socket");
	}
}