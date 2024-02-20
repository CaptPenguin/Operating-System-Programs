// #    # #####        #####  ####  #####         ####  ###### #####  #    # ###### #####       ####
// ##  ## #    #         #   #    # #    #       #      #      #    # #    # #      #    #     #    #
// # ## # #    # #####   #   #      #    # #####  ####  #####  #    # #    # #####  #    #     #
// #    # #####          #   #      #####             # #      #####  #    # #      #####  ### #
// #    # #              #   #    # #            #    # #      #   #   #  #  #      #   #  ### #    #
// #    # #              #    ####  #             ####  ###### #    #   ##   ###### #    # ###  ####
//
// Forking multi-process TCP echo server.  
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFSIZE 100

int main(int argc, char *argv[]) {
  char buf[BUFSIZE+1];
  int listen_fd;                 // file descriptor for listening
  int fd;                        // file descriptor for connection to current client
  struct sockaddr_in servaddr;   // info about the server's port number, etc.
  struct sockaddr_in clientaddr; // info about the connected client
  socklen_t size;                // size of info about connected client
  char ip[INET_ADDRSTRLEN];      // IP address of connected client in
				 // string form
  
  // the socket() system call creates one endpoint for network
  // communication.  AF_INET indicates we want "internet style"
  // sockets.  SOCK_STREAM indicates we want to use TCP.  Use "man
  // socket" for more info!  Importantly, the handle that's returned
  // is used to listen for connections--a separate handle will be used
  // for communication with clients.  See below.
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  // only certain fields in the sockaddr_in structure are required.
  // bzero() initializes everything in the struct to zero for safety.
  bzero(&servaddr, sizeof(servaddr));

  // use "internet style" sockets.
  servaddr.sin_family = AF_INET;

  // we will accept ("source") connections from any IP address.
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);

  // we will listen on port 6666 for client connections.
  servaddr.sin_port = htons(6666);

  // associate the handle returned by socket() with the sockaddr_in
  // structure we just populated.  Most important info is the port
  // number.
  bind(listen_fd, (struct sockaddr *) &servaddr,
       sizeof(servaddr));

  // listen for clients.  The 20 indicates only how many clients will
  // block waiting for the server to pay attention to them.
  listen(listen_fd, 20);

  // loop, accepting a connection, then handing it off to a child
  // process to handle before waiting for another connection.

  // this is where things are different from the single-threaded
  // client. Instead of the main process handling the client
  // connection via the handle returned by accept(), a new child is
  // created and the handle is handed off.  The main process then goes
  // back to waiting for for another connection.
  while (1) {
    printf("Waiting for a client...\n");
    
    // zero structure that holds info about connected client.
    bzero(&clientaddr, sizeof(clientaddr));

    // accept() waits for a client connection and then returns a
    // handle associated with the client connection.  This handle can
    // be used for read(), write(), etc. calls to communicate with the
    // client.  The sockaddr_in structure is populated with
    // information about the client.
    size=sizeof(clientaddr);
    fd = accept(listen_fd, (struct sockaddr*) &clientaddr,
		&size);

    // got a new client!  Create a new child to handle the connection.
    // Notice below that the parent (us) doesn't do anything except
    // create the child and move on.

    unsigned int pid = fork();
    if (! pid) {
      // new child to handle the new connection
      
      // first extract the IP address and convert it to a string.
      inet_ntop(AF_INET, &(clientaddr.sin_addr), ip, INET_ADDRSTRLEN);
      
      // Then print a message revealing the IP address and port.
      printf("Child handling connection from client @ %s:%d\n",
	     ip, clientaddr.sin_port);
      
      bzero(buf, BUFSIZE+1); 
      while(read(fd, buf, BUFSIZE) > 1) {
	printf("Received from client: %s", buf);
	write(fd, buf, strlen(buf)+1);
	bzero(buf, BUFSIZE+1); 
      }
      printf("Lost connection with client @ %s:%d\n",
	     ip, clientaddr.sin_port);
      // at this point the connection with the client has broken and
      // the child will die
    }
  }
}

