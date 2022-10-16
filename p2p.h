#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void join(int socketFd, int peer_id);

void publish(int socketFd, int count, char files[]);

bool search(int socket, char* fileName,int length);