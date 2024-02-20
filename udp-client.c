// #    # #####  #####         ####  #      # ###### #    # #####      ####
// #    # #    # #    #       #    # #      # #      ##   #   #       #    #
// #    # #    # #    # ##### #      #      # #####  # #  #   #       #
// #    # #    # #####        #      #      # #      #  # #   #   ### #
// #    # #    # #            #    # #      # #      #   ##   #   ### #    #
//  ####  #####  #             ####  ###### # ###### #    #   #   ###  ####
//
// Single-threaded UDP client.
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

#define BUFSIZE 100  // maximum size for one message

int main(int argc, char *argv[]) {
  char buf[BUFSIZE+1];   
  int fd;                // file descriptor for network endpoint
  struct sockaddr_in servaddr;
  struct sockaddr_in scratch;
  socklen_t len;

  // the socket() system call creates one endpoint for network
  // communication.  AF_INET indicates we want "internet style"
  // sockets.  SOCK_DGRAM indicates we want to use UDP.  Use "man
  // socket" for more info!  The return value is a handle that can be
  // used with standard read(), write(), etc. system calls.
  fd=socket(AF_INET,SOCK_DGRAM, 0);
  
  // only certain fields in the sockaddr_in structure are required.
  // bzero() initializes everything in the struct to zero for safety.
  bzero(&servaddr, sizeof(servaddr));
  
  // use "internet style" sockets.
  servaddr.sin_family=AF_INET;
  
  // we will talk to the other side on port 1434.
  servaddr.sin_port=htons(1434);
  
  // local host only--the server is assumed to be on the local
  // machine. The inet_pton() function converts the string format for
  // the IP address into network byte order binary format.  That needs
  // to be stored in the sin_addr field of the sockaddr_in structure.
  inet_pton(AF_INET,"127.0.0.1", &(servaddr.sin_addr));

  // accept strings and send them to the server
  while (1) {
    bzero(buf, BUFSIZE+1);

    printf("Enter a message to send.\n");
    fgets(buf, BUFSIZE, stdin);
    buf[strlen(buf)-1]=0;

    printf("Sending.\n");

    // send
    sendto(fd,                                    // socket
	   buf,                                   // buffer
	   BUFSIZE+1,                             // length
	   0,                                     // flags (obscure, generally 0)
	   (struct sockaddr *)&servaddr,          // dest_addr
	   sizeof(servaddr));                     // dest_len
  }
    
  return 0;
}

