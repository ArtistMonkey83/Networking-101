#include <iostream>
#include "lookup-and-connect.h"
#include "p2p.h"
#include <dirent.h>
#include <string.h>
using namespace std;

/*
command line arguments:
1: registry location(string)
2: port number(convert to int)
3: peer ID(convert to int 0 < i < 2^32)
*/
int main(int argc, char* argv[])
{
  //store the peer ID into an unsigned int, since that perfectly fits the range of numbers we need
  int port = strtol(argv[2], NULL, 10);
  unsigned int peerID = strtoul(argv[3], NULL, 10);

  //connect to peer location on given port
  int socket = lookup_and_connect(argv[1], argv[2]);

  //begin input loop
  //commands will be entered until the EXIT command is given
  //assumes that the user knows to enter the commands in all caps
  string input = "";
  cout << "Enter a command: ";
  cin >> input;
  while(input != "EXIT")
  {
    if(input == "JOIN")
    {
      //send join request
      join(port, peerID);
    }
    else if(input == "PUBLISH")
    {
       
      //publish request to registry
      //create a char-string to store all filenames
      //the search functionality uses 12000 characters for file names, to be safe
        char fileList[12001];
        char action = 0x01;
        strcat(fileList, &action);
        //make a character to separate file names before replacing with null
        char prelimSeparator = '\n';
        //store the number of files that are found
        int length = 0;
        //read the program's current directory
        DIR* dir;
        struct dirent* ent;
        if ((dir = opendir(".")) != NULL)
        {
            
            while ((ent = readdir(dir)) != NULL)
            {
                //increment number of files by 1
                length++;

                //add file name to list, followed by a delimiter character
                strcat(fileList, ent->d_name);
                strcat(fileList, &prelimSeparator);
            }
        }
        else
        {
            //give a message that the directory cannot be opened
        }
        closedir(dir);

        //only continue with the rest if there are files to publish
        if (length > 0)
        {
            //replace old seperator character with null character, to make it work with the p2p protocol
            int length = strlen(fileList);
            for (int i = 0; i < length; i++)
            {
                if (fileList[i] == prelimSeparator)
                {
                    fileList[i] = '\0';
                }
            }

            //use publish to do what the command says it does
            publish(socket, length, fileList);
        }
        else
        {
            cout << "There are no files to publish." << endl;
        }
    }
    else if(input == "SEARCH")
    {
      //search for file named by terminal
      char fileName[1200] = "";
      cout << "Enter a file name: ";
      cin >> fileName;
      
      search(socket, fileName, strlen(fileName));

    }
    //might as well add this. it's not like the professor would dock points for an extra command
    else if (input == "HELP")
    {
      cout << "JOIN: joins the pre-entered peer-to-peer network" << endl
           << "PUBLISH: publishes all files within the program's directory to the network" << endl
           << "SEARCH: searches for the given file name within the network";
    }
    else
    {
        cout << "The command entered was not recognized. Please enter something else." << endl;
    }
    cout << "Enter a command: ";
    cin >> input;
  }
  return 1;
}

