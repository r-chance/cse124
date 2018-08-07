#include <iostream>
#include "httpd.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define MAXPENDING 128


using namespace std;

void DieWithError(const char *errorMessage);

void HandleTCPClient(int clntSocket);

void start_httpd(unsigned short port, string doc_root)
{
  cerr << "Starting server (port: " << port <<
          ", doc_root: " << doc_root << ")" << endl;

  int servSock;
  int clntSock;
  struct sockaddr_in echoServAddr;
  struct sockaddr_in echoClntAddr;
  unsigned short echoServPort = port;
  unsigned int clntLen;

  // create socket for incoming connections
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
    DieWithError("socket() failed");
  
  // construct local address structure
  memset(&echoServAddr, 0, sizeof(echoServAddr));
  echoServAddr.sin_family = AF_INET;
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  echoServAddr.sin_port = htons(echoServPort);

  // bind to the local address
  if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    DieWithError("bind() failed");

  /* Mark the socket so it will listen for incoming connections */
  if (listen(servSock, MAXPENDING) < 0)
    DieWithError("listen() failed");

  for(;;) {// infinite loop

    // set size of the in-out param
    clntLen = sizeof(echoClntAddr);
    // wait for a client to connect
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                           &clntLen)) < 0)
      DieWithError("accept() failed");

    if (!fork()) {

      // clntSock is now connected to a client
      printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

      HandleTCPClient(clntSock);
      cout << getpid() << endl;
      close(clntSock);
    }
    else {  cout << getpid() << endl;
      close(clntSock); 
    }
  }
}
