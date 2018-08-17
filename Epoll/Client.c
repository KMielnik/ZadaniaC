#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include "error.h"

int connectToServer(char *hostname)
{
    struct sockaddr_in serverAddress;

    int portNumber = 72393;
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

    if (connect(clientSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        error("ERROR connecting");

    return clientSocket;
}

int main(int argc, char *argv[])
{
    char buffer[256];
    int charactersWritten;
    int clientSocket = connectToServer("localhost");

    bzero(buffer, 256);
    char *nickname = argv[1];

    write(clientSocket, nickname, strlen(nickname));

    while (1)
    {
        bzero(buffer, 256);
        printf("%s(you): \n",nickname);
        fgets(buffer,255,stdin);
        if(buffer[0]=='q')
            break;
        write(clientSocket, buffer, strlen(buffer));
        bzero(buffer, 256);
        read(clientSocket, buffer, 255);
        printf("Odebrano: %s\n", buffer);
    }

    write(clientSocket, "q", strlen("q"));

    close(clientSocket);

    return 0;
}