// #    # #####  #####         ####  ###### #####  #    # ###### #####       ####
// #    # #    # #    #       #      #      #    # #    # #      #    #     #    #
// #    # #    # #    # #####  ####  #####  #    # #    # #####  #    #     #
// #    # #    # #####             # #      #####  #    # #      #####  ### #
// #    # #    # #            #    # #      #   #   #  #  #      #   #  ### #    #
//  ####  #####  #             ####  ###### #    #   ##   ###### #    # ###  ####
//
// Single-threaded UDP echo server.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 20  // maximum size for one message

int main(int argc, char *argv[]) {
  char final[30];
  char buf[BUFSIZE+1];   
  int fd;                            // file descriptor for network endpoint
  struct sockaddr_in addr_server;    // server address
  struct sockaddr_in addr_client;    // server address
  socklen_t len;
  
  // the socket() system call creates one endpoint for network
  // communication.  AF_INET indicates we want "internet style"
  // sockets.  SOCK_DGRAM indicates we want to use UDP.  Use "man
  // socket" for more info!  The return value is a handle that can be
  // used with standard read(), write(), etc. system calls.
  fd=socket(AF_INET,SOCK_DGRAM, 0);
  
  // only certain fields in the sockaddr_in structure are required.
  // bzero() initializes everything in the struct to zero for safety.
  bzero(&addr_server, sizeof(addr_server));
  
  // use "internet style" sockets.
  addr_server.sin_family=AF_INET;

  // happy to receive from anyone on any IP associated with this
  // machine (means:  any interface)
  addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
  
  // we listen for messages on port 1434
  addr_server.sin_port=htons(1434);

  // bind socket to the port and IP address we established
  bind(fd, (struct sockaddr*)&addr_server, sizeof(addr_server));
  
  // accept messages from clients and display
  while (1) {
    bzero(buf, BUFSIZE+1);

    printf("Waiting.\n");
    
    // receive
    recvfrom(fd,                                  // socket
	     buf,                                 // buffer
	     BUFSIZE+1,                           // length
	     0,                                   // flags (obscure, generally 0)
	     (struct sockaddr *)&addr_client,     // will be filled with info about client
	     &len);                               // dest_len
    
    printf("Received from client: \"%s\"\n", buf);
    printf("Processing!\n");
    strcpy(final, buf);
    strcat(final, ", yo!");
    printf("Final: \"%s\n", final);
  }

  return 0;
}





