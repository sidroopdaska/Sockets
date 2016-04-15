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
	
	server = gethostbyname(argv[1]);
	if (server == NULL){
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0)
		error("Error opening socket");

	bzero((char*)&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	bcopy((char*)server->h_addr, (char*)&serverAddr.sin_addr.s_addr, server->h_length);

	if(connect(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error("ERROR connecting!");

	cout << "Please enter a message: ";
	cin >> writeBuffer;

	if (write(socketFD, writeBuffer, strlen(writeBuffer)) < 0)
		error("ERROR writing to the socket!");

	bzero(readBuffer, sizeof(readBuffer));
	if(read(socketFD, readBuffer, sizeof(readBuffer)) < 0)
		error("ERROR reading from socket!");	

	cout << "\nThe server messsage is : " << readBuffer << endl;

	close(socketFD);

	return 0;
}