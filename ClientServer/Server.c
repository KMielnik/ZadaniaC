#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include "error.h"

int openConnection()
{
    struct sockaddr_in serverAdress, clientAdress;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        error("Couldn't open socket");

    bzero((char *)&serverAdress, sizeof(serverAdress)); //write 0 to all bytes
    int portNumber = 3456;
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(portNumber);
    serverAdress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAdress, sizeof(serverAdress)) < 0)
        error("Couldn't bind");

    listen(serverSocket, 5);

    int clientAddressLength = sizeof(clientAdress);
    int newSocketFd = accept(serverSocket, (struct sockaddr *)&clientAdress, &clientAddressLength);
    if (newSocketFd < 0)
        error("Couldn't accept");

    return newSocketFd;
}

int main()
{
    char buffer[255];
    int charactersRead;

    int newSocketFd = openConnection();

    bzero(buffer, 256);
    charactersRead = read(newSocketFd, buffer, 255);
    if (charactersRead < 0)
        error("Error reading from socket");
    printf("Message: %s\n", buffer);
    
    return 0;
}