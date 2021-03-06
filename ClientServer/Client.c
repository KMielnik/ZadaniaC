#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "error.h"

int connectToServer(char *hostname)
{
    struct sockaddr_in serverAddress;

    int portNumber = 3456;
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket < 0)
        error("ERROR opening socket");

    struct hostent *server = gethostbyname("localhost");
    if (server == NULL)
        error("No such host");

    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],
          (char *)&serverAddress.sin_addr.s_addr,
          server->h_length);
    serverAddress.sin_port = htons(portNumber);

    if (connect(clientSocket, (const struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
        error("ERROR connecting");

    return clientSocket;
}



int main()
{
    char buffer[256];
    int charactersWritten;
    int clientSocket = connectToServer("localhost");
    printf("Enter message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    charactersWritten = write(clientSocket,buffer,strlen(buffer));
    if (charactersWritten <0)
        error("ERROR writing to socket");
    

    
    return 0;
}