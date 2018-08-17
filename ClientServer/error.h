#ifndef ERROR_H
#define ERROR_H

void error(char *msg)
{
    perror(msg);
    exit(1);
}

#endif