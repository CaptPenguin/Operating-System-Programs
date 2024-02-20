// #####  ####  #####         ####  #      # ###### #    # #####      ####  
//   #   #    # #    #       #    # #      # #      ##   #   #       #    # 
//   #   #      #    # ##### #      #      # #####  # #  #   #       #      
//   #   #      #####        #      #      # #      #  # #   #   ### #      
//   #   #    # #            #    # #      # #      #   ##   #   ### #    # 
//   #    ####  #             ####  ###### # ###### #    #   #   ###  ####  
//
// Single-threaded TCP echo client.  This client can connect to either
// the single- or multi-threaded/multi-process TCP echo servers.
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
  bool connected=false;

  while (! connected) {
    // the socket() system call creates one endpoint for network
    // communication.  AF_INET indicates we want "internet style"
    // sockets.  SOCK_STREAM indicates we want to use TCP.  Use "man
    // socket" for more info!  The return value is a handle that can be
    // used with standard read(), write(), etc. system calls.
    fd=socket(AF_INET,SOCK_STREAM, 0);
    
    // only certain fields in the sockaddr_in structure are
    // required.  bzero() initializes everything in the struct to zero
    // for safety.
    bzero(&servaddr, sizeof(servaddr));
    
    // use "internet style" sockets.
    servaddr.sin_family=AF_INET;
    
    // we will connect to a server on port 6666.
    servaddr.sin_port=htons(6666);
    
    // local host only--the server is assumed to be on the local
    // machine. The inet_pton() function converts the string format for
    // the IP address into network byte order binary format.  That needs
    // to be stored in the sin_addr field of the sockaddr_in structure.
    inet_pton(AF_INET,"127.0.0.1", &(servaddr.sin_addr));
    
    // the connect() system call takes the file descriptor returned from
    // socket, the populated sockaddr_in structure, and the size of that
    // structure, and establishes a connection with the server.  connect()
    // returns 0 when the connection is established.
    printf("Connecting to server...\n");
    connected = ! connect(fd, (struct sockaddr *)&servaddr,sizeof(servaddr));
    
    if (! connected) {
      perror("Didn't connect yet: ");
      sleep(1);
    }
  }
  printf("Connected.\n");

  // once all that's done, the rest of the echo client is pretty
  // simple.  Just read a string from the keyboard, send it to the
  // server, wait for a response, and repeat.
  while (1) {
    bzero(buf, BUFSIZE+1);
    fgets(buf, BUFSIZE, stdin);
    write(fd, buf, strlen(buf)+1);   
    bzero(buf, BUFSIZE+1);
    read(fd, buf, BUFSIZE);
    printf("Received from server: %s\n", buf);
  }
  
}

