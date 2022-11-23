///  _____
/// (___  )      _
///     | |  _  (_) ___
///  _  | |/ _ \| |  _  \
/// ( )_| | (_) ) | ( ) |
///  \___/ \___/(_)_) (_)




//Join Registry needs to track peers that JOIN
//Establishes a unique socket for continued communication

// Structure to hold the peers information
struct peer_entry {
  uint32_t id;  // ID of peer, we assign this
  int socket_descriptor; // Socket descriptor for connection
  char files[MAX_FILES][MAX_FILENAME_LEN]; // Files published by peer
  struct sockaddr_in address; // Contains IP address and port number
}


///  ___                          _
/// (  _ \                       ( )
/// | (_(_)  __    _ _ _ __   ___| |__
///  \__ \ / __ \/ _  )  __)/ ___)  _  \
/// ( )_) |  ___/ (_| | |  ( (___| | | |
///  \____)\____)\__ _)_)   \____)_) (_)






///  ___         _     _          _
/// (  _ \      ( )   (_ ) _     ( )
/// | |_) )_   _| |_   | |(_) ___| |__
/// |  __/( ) ( )  _ \ | || |  __)  _  \
/// | |   | (_) | |_) )| || |__  \ | | |
/// (_)    \___/(_ __/(___)_)____/_) (_)






///                        ___                     _
/// / \_/ \      _        (  _ \                  ( )_ _
/// |     |  _ _(_) ___   | (_(_)_   _  ___    ___|  _)_)  _    ___
/// | (_) |/ _  ) |  _  \ |  _) ( ) ( )  _  \/ ___) | | |/ _ \/  _  \
/// | | | | (_| | | ( ) | | |   | (_) | ( ) | (___| |_| | (_) ) ( ) |
/// (_) (_)\__ _)_)_) (_) (_)    \___/(_) (_)\____)\__)_)\___/(_) (_)

struct peer_entry {
  uint32_t id;  // ID of peer, we assign this
  int socket_descriptor; // Socket descriptor for connection
  char files[MAX_FILES][MAX_FILENAME_LEN]; // Files published by peer
  int numFiles; //how many files published
  struct sockaddr_in address; // Contains IP address and port number
}; //
peer[PEER_LIST_SIZE].address.sin_port; //port number
peer[PEER_LIST_SIZE].address.sin_addr; //ip address
//This Global variable will dictate how large the array of peers will be
//Value can range up to 4,294,967,295!
uint32_t PEER_LIST_SIZE = 10; //5 peers minimum for this implementation
uint32_t NUMBER_OF_PEERS = 0; //Accumulates the number of peers JOINed

int main (){

//use recvAll to parse this!
//recvAll(socket file discriptor,action)
uint8_t action = 0; //Action will hold the 1st byte of the message recieved.
uint32_t peer_id = 0; //peer_id will hold 4 bytes of the JOIN message recieved.
uint32_t count = 0; //count will hold 4 bytes of the PUBLISH  message recieved

action = XXXXXX;//recvAll to populated the action field from the message 1 Byte

//convert from little indian look to existing join()

peer_entry peer[PEER_LIST_SIZE]; //Array that will hold the list of peers indexed by id
int sockDes[PEER_LIST_SIZE]; // Array that will hold the peer id indexed by socket discriptor, lets you manage socket discriptors without peer_ids!


 //This switch statement handles the Message format sent by the peer.
 //The first byte of any message recieved will be the action field which
 //Dictates what action the registry will perform. Response messages
 //DO NOT contain an action field!
 switch(action){
   //If the action variable is == 0 then we have received a JOIN request
   //The registry does not send a response to a JOIN request & multiple JOIN
   //Requests should return an error according to program 2! Must track the files
   //PUBLISHed, peer's IP address, port number and socket descriptor for the socket connected to the peer. Doesn't maintain state if a peer disconnects.
   case 0:
    peer[NUMBER_OF_PEERS]; //Access the index for a peer who just JOINed

    //peer_id must be in network byte order!(Program2)
    peer_id = XXXXXX; // recvAll to populate the peer_id member variable, 4 Bytes
    peer[NUMBER_OF_PEERS].id = peer_id; //4 Bytes of the JOIN message

    peer[NUMBER_OF_PEERS].socket_descriptor = XXXXXX; //How do we assign socket descriptors? What if someone dissconnects and that descriptor is now available?

    //Retrieves the connection information into the variable addr
    //for the already connected socket s, change s to
    //peer[NUMBER_OF_PEERS].socket_descriptor?
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int ret = getpeername(s,(struct sockaddr*)&addr, &len);


    NUMBER_OF_PEERS++; //Accumulate the number of peers currently JOINed
   break;

   //If the action variable is == 1 we have received a publish request
   //needs to track the files available at each
   case 1:

   break;

   //If the action variable is == 2 we have received a search request
   //nested for loop use num of files to string compare each file for each peer
   //return the first peer found, dont worry about duplicate peer
   case 2:

   break;
 }
  return 0;
}
