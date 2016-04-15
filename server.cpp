#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void error(const char* msg){
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]){
	int socketFD, newSocketFD, portNumber;
	char readBuffer[256];
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientLength;

	bzero((char*)&serverAddr, sizeof(serverAddr));
	bzero(readBuffer, sizeof(readBuffer));
	
	if(argc < 2){
		fprintf(stderr, "ERROR, no port provided!\n");
		exit(1);
	}	
	
	portNumber = atoi(argv[1]);
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 1)
		error("ERROR opening socket");
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error("ERROR binding socket");

	listen(socketFD, 5);
	clientLength =  sizeof(clientAddr);
	newSocketFD = accept(socketFD, (struct sockaddr*)&clientAddr, &clientLength);
	if (newSocketFD < 0) {
		error("ERROR making a new connection");
	}

	if(read(newSocketFD, readBuffer, sizeof(readBuffer)) < 0)
		error("ERROR reading from socket!");

	fprintf(stdout, "Here is the message: %s\n", readBuffer);

	const char* writeBuffer = "I got your message";
	cout << strlen(writeBuffer) << endl;

	if(write(newSocketFD, writeBuffer, strlen(writeBuffer)) < 0)
		error("ERROR writing to the socket");

	close(newSocketFD);
	close(socketFD);

	return 0;
}