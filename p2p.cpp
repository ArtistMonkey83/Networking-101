#include "p2p.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

//join and publish functions made by Yolanda Reyes
//search function made by Tom Laurison
//publish modified 
void join(int socketFd, int peer_id)
{
    unsigned char join_request[5];
    char join_response[5];
    int bytes_recieved = 0;
    int bytes_sent = 0;
    // Action = 0 in hexodecimal
    join_request[0] = 0x00;
    // Peer ID must be in network byte order, this should be 4 bytes total
    join_request[1] = (peer_id >> 24) & 0xFF;
    join_request[2] = (peer_id >> 16) & 0xFF;
    join_request[3] = (peer_id >> 8) & 0xFF;
    join_request[4] = peer_id & 0xFF;

    for(int i = 0; i < 5; i++) printf("%d\n", join_request[i]);
    // send join request
    bytes_sent = send(socketFd, join_request, 5, 0);
    if (bytes_sent == -1)
    {
        perror("[<Error> Client] onSend:\n");
        exit(1);
    }else{
        printf("successfully joined!!\n");
    }
}

void publish(int socketFd, int count, char files[]){
    char publish_request[1200];
    int bytes_sent = 0;printf("%s\n", files);
        // bytes_sent = send(socketFd, publish_request, 1200, 0);
        bytes_sent = send(socketFd, files, 1200, 0);
        if (bytes_sent == -1)
        {
            perror("[<Error> Client] onSend:\n");
            exit(1);
        }else{
            printf("%s is published!!\n", files);
        
    }

}

bool search(int socket, char *fileName, int length)
{
    //create search query
    char request[] = "";
    char action[] = { 0x02 };
    strcat(request, action);
    strcat(request, fileName);
    //send search query
    int bytes_sent = 0;
    bytes_sent = send(socket, fileName, length + 1, 0);
    if (bytes_sent == 0)
    {
        std::cout << "Request failed to send." << std::endl;
        return false;
    }

    //recieve search result
    char response[10];
    int bytes_recieved = 0;
    bytes_recieved = recv(socket, response, 10, 0);

    if (bytes_recieved != 0)
    {
        if (strcmp(response, "0000000000") != 0)
        {
            //convert peer id
            int peerIDBase256[] = { (int)response[0], (int)response[1], (int)response[2], (int)response[3] };
            int convertedID = (peerIDBase256[0] * 256 * 256 * 256) + (peerIDBase256[1] * 256 * 256) + (peerIDBase256[2] * 256) + peerIDBase256[3];

            //convert port number
            int portIDBase256[] = { (int)response[8], (int)response[9] };
            int base10Port = ((int)portIDBase256[0]*256) + (int)portIDBase256[1];

            //begin print
            std::cout << "File found at" << std::endl
                << "Peer " << convertedID << std::endl;

            //print ip address
            std::cout << (int)response[4] << (int)response[5] << (int)response[6] << (int)response[7] << ":" << base10Port << std::endl;                   
        }
        else
        {
            std::cout << "File not indexed by registry." << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "No data recieved. The connection was likely closed." << std::endl;
        return false;
    }

    return true;
}

/*
original publish function:
void publish(int socketFd, int count, char files[]){
    char publish_request[1200];
    int bytes_sent = 0;
    for(int i = 0; i < count; i++){
        printf("%s\n", files[i]);
        // bytes_sent = send(socketFd, publish_request, 1200, 0);
        bytes_sent = send(socketFd, files[i], 1200, 0);
        if (bytes_sent == -1)
        {
            perror("[<Error> Client] onSend:\n");
            exit(1);
        }else{
            printf("%s is published!!\n", files[i]);
        }
    }

}
*/