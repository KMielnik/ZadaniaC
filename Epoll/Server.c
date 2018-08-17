#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "error.h"

int openListeningConnection(int portNumber)
{
    struct sockaddr_in serverAdress, clientAdress;

    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket < 0)
        error("Couldn't open socket");

    bzero((char *)&serverAdress, sizeof(serverAdress)); //write 0 to all bytes

    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(portNumber);
    serverAdress.sin_addr.s_addr = INADDR_ANY;

    if (bind(listeningSocket, (struct sockaddr *)&serverAdress, sizeof(serverAdress)) < 0)
        error("Couldn't bind");

    listen(listeningSocket, 5);

    return listeningSocket;
}

int createEpoll()
{
    int epoll_fd = epoll_create1(0);

    if (epoll_fd == -1)
        error("ERROR creating epoll fd");
}

int setNonBlocking(int socket)
{
    fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
}

void addListeningSocketToEpoll(int epollFd, int listeningSocket)
{
    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = listeningSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listeningSocket, &event))
    {
        close(epollFd);
        error("ERROR adding fd to epoll");
    }
}

struct User
{
    int userFD;
    char *nickname;
};

struct User *findUserByFd(struct User users[], int userCount, int fd)
{
    for (int i = 0; i < userCount; i++)
        if (users[i].userFD == fd)
            return &users[i];
    struct User badUser;
    badUser.userFD = -1;
}

struct User *findUserByNickName(struct User users[], int userCount, char *nickname)
{
    for (int i = 0; i < userCount; i++)
        if (strcmp(users[i].nickname, nickname) == 0)
            return &users[i];
    struct User badUser;
    badUser.userFD = -1;
    return &badUser;
}

struct Message
{
    struct User *from;
    struct User *to;
    char *message;
};

int main()
{
    int event_count;
    int bytes_read;
    char buffer[255];
    struct epoll_event event, events[10];
    struct User users[10];
    int userCount = 0;

    struct Message messages[100];
    int messageCount = 0;

    int epollFd = createEpoll();

    int listeningSocket = openListeningConnection(72393);

    addListeningSocketToEpoll(epollFd, listeningSocket);

    while (1)
    {
        event_count = epoll_wait(epollFd, &events, 10, 30000);

        for (int i = 0; i < event_count; i++)
        {
            if (events[i].data.fd == listeningSocket)
            {
                struct sockaddr_in clientAddress;
                int clientAddressLength = sizeof(clientAddress);

                int connectionSocket = accept(listeningSocket,
                                              (struct sockaddr *)&clientAddress,
                                              &clientAddressLength);
                if (connectionSocket < 0)
                    error("ERROR accept listeningSocket");
                setNonBlocking(connectionSocket);
                event.events = EPOLLIN | EPOLLET | EPOLLOUT;
                event.data.fd = connectionSocket;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD, connectionSocket, &event) < 0)
                    error("ERROR adding listening socket to epoll");
                users[userCount].userFD = connectionSocket;
                users[userCount].nickname = " ";
                userCount++;
            }
            else
            {
                bzero(buffer, 255);
                int userFd = events[i].data.fd;
                struct User *user = findUserByFd(users, userCount, userFd);

                recv(userFd, buffer, 255,0);
                if (buffer[0] == 0)
                    continue;

                if (user->nickname[0] == ' ')
                {
                    char *nickname = (char *)malloc(255);
                    strcpy(nickname, buffer);
                    struct User *user = findUserByFd(users, userCount, userFd);
                    user->nickname = nickname;
                    continue;
                }

                char *nickname = user->userFD < 0 ? "undefinied" : user->nickname;

                if (buffer[0] == 'q')
                {
                    printf("%s %s", nickname, "wychodzi.");

                    epoll_ctl(epollFd, EPOLL_CTL_DEL, user->userFD, &event);
                    int i = 0;
                    for (; i < userCount; i++)
                        if (users[i].userFD == user->userFD)
                            break;
                    for (; i < userCount - 1; i++)
                    {
                        users[i] = users[i + 1];
                    }

                    userCount--;
                }

                char *messageText = (char *)malloc(255);

                messages[messageCount].from = user;
                messages[messageCount].to = user;
                messages[messageCount].message = (char*) malloc(255);
                strcpy(messages[messageCount].message, buffer);

                messageCount++;


                printf("%s: %s", nickname, buffer);
                send(userFd, "ok", 3,0);
            }
        }

        for(int i=0;i<messageCount;i++)
        {
            int targetFd = messages[i].to->userFD;
            char *fromNickname = messages[i].from->nickname;
            char *message = messages[i].message;
            send(targetFd,message,sizeof(message),0);
        }

    }

    if (close(epollFd))
        error("ERROR closing epoll fd");

    return 0;
}