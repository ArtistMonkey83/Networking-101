#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE 500





int bind_and_listen( const char *service );// usually port and similar/same as getadderinfo code?
uint32_t PEER_LIST_SIZE=10;
uint32_t NUMBER_OF_PEERS=0;

int find_max_fd(const fd_set *fs);



void manager(){
	int retval;
	fd_set rfds;
	//struct timeval  tv;
/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	retval = select(1, &rfds, NULL, NULL, NULL);
/* Don't rely on the value of tv now! */
	if (retval == -1)
		perror("select()");
		else if (retval)
		printf("Data is available now.\n");
/* FD_ISSET(0, &rfds) will be true. */
	else
		printf("No data within five seconds.\n");
		exit(EXIT_SUCCESS);
}


/*
 * This function is to Create, bind and passive open a socket on a local interface for the provided service.
 * Argument matches the second argument to getaddrinfo(3).
 */

void sockConnection()
{
	// all_sockets stores all active sockets
	// call_set is a temporary used for the select call
	fd_set all_sockets, call_set; // think of as an array
	FD_ZERO(&all_sockets);

	// listen_socket is the fd on which the program can accept() new connections
	int listen_socket = bind_and_listen(SERVER_PORT);
	FD_SET(listen_socket, &all_sockets); //select gives us indication this needs to be called

	// max_socket should always contain the socket fd with the largest value
	int max_socket = listen_socket;

	while(1) {
		call_set = all_sockets;
		int num_s = select(max_socket+1, &call_set, NULL, NULL, NULL);//listing for traffic num s is some socket that needs handling. resolve the id
		//writefds, is the third argument because we are a registy

		if( num_s < 0 ){
			perror("ERROR in select() call");
			return -1;
		}
		// Skip standard IN/OUT/ERROR -> start at 3
		for( int s = 3; s <= max_socket; ++s ){
			// Skip sockets that aren't ready
			if( !FD_ISSET(s, &call_set) )
				continue;

			// A new connection is available
			if( s == listen_socket ){

				// What should happen with a new connection?
				// You need to call at least one function here
				// and update some variables
				max_socket = accept(s, NULL, NULL); //new socket with data that is already
				FD_SET(max_socket, &all_sockets);
				printf("Socket %i connected\n", max_socket);
			}
			else{
				// What should happen with data from an existing connection?
				// Don't forget to handle a closed connection, which will
				// end up here as well.
				char buf[100];
				int bytes_recv = recv(s, buf, 100, 0);
				if (bytes_recv < 0) continue;
				if (bytes_recv == 0)
				{
					printf("Socket %i closed\n", s);
					FD_CLR(s, &all_sockets);
					close(s);
				}
				else
				{
					printf("Socket %i sent: %s", s, buf);
				}
			}
		}
	}
}

// this is checking the connection to check the file descriptor from top to bottom

int find_max_fd(const fd_set *fs) {
	int ret = 0;
	for(int i = FD_SETSIZE; i>=0 && ret==0; --i){
		if( FD_ISSET(i, fs) ){
			ret = i;
		}
	}
	return ret;
}


//Peer structure
struct peer_entry {
  uint32_t id;  // ID of peer, we assign this
  int socket_descriptor; // Socket descriptor for connection
  char files[MAX_FILES][MAX_FILENAME_LEN]; // Files published by peer
  int numFiles; //how many files published
  struct sockaddr_in address; // Contains IP address and port number
};


//Void function for Join
void JOIN(int s, uint32_t pId)
{
  unsigned char action = 0;
  peer_entry peer[PEER_LIST_SIZE]; //Array that will hold the list of peers indexed by id. Might have to move this to main.c file
  int sockDes[PEER_LIST_SIZE];// Array that will hold the peer id indexed by socket discriptor, lets you manage socket discriptors without peer_ids! Might have to move this to main.c file 
  uint32_t peerid=htonl(pId);
  if (checker<0)            {
      fprintf(stderr, "Unable to connect 1.\n" );
      exit(1);
  }

  //peer_id must be in network byte order!(Program2)
    //peer_id = XXXXXX; // recvAll to populate the peer_id member variable, 4 Bytes
    peer[NUMBER_OF_PEERS].id = peerid; //4 Bytes of the JOIN message

    peer[NUMBER_OF_PEERS].socket_descriptor = s; //How do we assign socket descriptors? What if someone dissconnects and that descriptor is now available?

    //Retrieves the connection information into the variable addr
    //for the already connected socket s, change s to
    //peer[NUMBER_OF_PEERS].socket_descriptor?
    //struct sockaddr_in addr;
    //socklen_t len = sizeof(addr);
    //int ret = getpeername(sendall);

    //peer[NUMBER_OF_PEERS].address = ret;
    peer[NUMBER_OF_PEERS].address.sin_port= host_Port;
    peer[NUMBER_OF_PEERS].address.sin_addr= host_IP;

    NUMBER_OF_PEERS++; //Accumulate the number of peers currently JOINed



}









int sendall(int s, char *msg, int len){

	int total = 0;							//variables to keep track of bytes sent and remaining to be sent
	int bytesRemaining = len;
	int n;

	while(total < len){		//while number of bytes sent is less than len
		n = send(s, msg + total, bytesRemaining, 0);		//call send on msg, indexed by total, trying to send bytesRemaining number of bytes
		if(n == -1){		//if there is an error with the send, break while loop
			break;
		}
		total += n;			//if send success, add number of bytes sent to total,
		bytesRemaining -= n;	//and subtract number of bytes sent from bytesRemaining
	}


	return n == -1?-1:total;		//if success, return total, else, return -1
}






//Void function for SEARCH

void SEARCH(int s){

  char msg[256];
  unsigned char action=2;
  int checker=send(s,&action,sizeof(action),0);    //send action id

  if(checker!=1){
    fprintf( stderr,"Unable to send action.\n");
    printf("test 10");
    exit(1);
  } else {
    printf("Enter a file name: ");
    fgets(msg,sizeof(msg),stdin);
    char* filename;
    filename = strtok(msg, "\n");
    checker=sendall(peerid, host_IP, host_Port);   //search response y sending peerid, ip address, port number


  }


  if(checker<0){
    fprintf( stderr,"Unable to send filename.\n");
    printf("test 10");
    exit(1);
  }
  unsigned char buf[10];

  checker=recv(s,buf,10,0);


  if(checker<10){
    fprintf( stderr,"Unable to connect.\n");
    //printf("test 11\n");
    exit(1);
  }
  //if file not found, inform user
  if (buf[8]==0 && buf[9]==0){
    printf("File not indexed by registry\n");
    return;
  } else {    //else if file found, convert data to host byte order, and print
    //separate recieved data into ID, IP and Port number, as defined by protocol in handout
    uint32_t network_ID;
    uint32_t network_IP;
    uint16_t network_Port;
    memcpy(&network_ID, buf, 4);
    memcpy(&network_IP, buf + 4, 4);
    memcpy(&network_Port, buf + 8, 2);
    //convert from network byte order to host byte order before using data
    uint32_t host_ID = ntohl(network_ID);
    uint32_t host_IP_32 = ntohl(network_IP);
    uint16_t host_Port = ntohs(network_Port);
    uint8_t host_IP[4];
    host_IP[0] = (host_IP_32 & 0xFF000000) >> 24;   //masking 32 bit int into array of 4 8 bit ints
    host_IP[1] = (host_IP_32 & 0x00FF0000) >> 16;
    host_IP[2] = (host_IP_32 & 0x0000FF00) >> 8;
    host_IP[3] = (host_IP_32 & 0x000000FF);
    printf("File found at\n");
    printf(" Peer %u\n", host_ID);
    printf(" %i.%i.%i.%i:%i\n", host_IP[0], host_IP[1], host_IP[2], host_IP[3], host_Port);
    return;
  }

  return;


}










































int registry(char port[6])
{

    int sfd, s;


    char buf[BUF_SIZE];


    ssize_t nread;


    socklen_t  peer_addrlen;


    struct addrinfo hints;


    struct addrinfo *result, *rp;


    struct sockaddr_storage  peer_addr;


    memset(&hints, 0, sizeof(hints));


    hints.ai_family = AF_INET;    /* Allow IPv4*/


    hints.ai_socktype = SOCK_STREAM; /* TCP */


    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */


    hints.ai_protocol = 0;          /* Any protocol */


    hints.ai_canonname = NULL;


    hints.ai_addr = NULL;


    hints.ai_next = NULL;


    s = getaddrinfo(NULL, port, &hints, &result);


    if (s != 0) {


        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));


        exit(EXIT_FAILURE);
    }


    /* getaddrinfo() returns a list of address structures.


       Try each address until we successfully bind(2).


       If socket(2) (or bind(2)) fails, we (close the socket


       and) try the next address. */


    for (rp = result; rp != NULL; rp = rp->ai_next) {


        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);


        if (sfd == -1)


            continue;


        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)


            break;                  /* Success */


        close(sfd);
    }

    freeaddrinfo(result);           /* No longer needed */


    if (rp == NULL) {               /* No address succeeded */


        fprintf(stderr, "Could not bind\n");


        exit(EXIT_FAILURE);

    }


return sfd;
}
