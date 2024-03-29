// #    # #####       #####  ####  #####         ####  ###### #####  #    # ###### #####       ####
// ##  ##   #           #   #    # #    #       #      #      #    # #    # #      #    #     #    #
// # ## #   #   #####   #   #      #    # #####  ####  #####  #    # #    # #####  #    #     #
// #    #   #           #   #      #####             # #      #####  #    # #      #####  ### #
// #    #   #           #   #    # #            #    # #      #   #   #  #  #      #   #  ### #    #
// #    #   #           #    ####  #             ####  ###### #    #   ##   ###### #    # ###  ####
//
// Multi-threaded TCP echo server.  
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

// prototype for function that will be executed in a separate thread
// to handle one incoming client connection
void *Thread(void *args);

// thread creation allows a single void * argument to be passed. We'll
// pass a pointer to a variable of the following struct type, to
// inform the thread of the file descriptor for a new client as well
// as information about the client, including its IP address and
// source port.
typedef struct Client {
  int fd;  // handle for connection to client
  char ip[INET_ADDRSTRLEN];
  unsigned short port;
} Client;

int main(int argc, char *argv[]) {
  int listen_fd;                 // file descriptor for listening
  int fd;                        // file descriptor for connection to current client
  struct sockaddr_in servaddr;   // info about the server's port number, etc.
  struct sockaddr_in clientaddr; // info about the connected client
  socklen_t size;                // size of info about connected client
  pthread_t client_thread;       // thread id
  Client *clientinfo;            // information about client, provided
				 // to thread
  
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

  // loop, accepting a connection, then handing it off to a new thread
  // to handle before waiting for another connection.

  // this is where things are different from the single-threaded
  // client. Instead of the main thread handling the client connection
  // via the handle returned by accept(), a new child is created and
  // the handle is handed off.  The main thread then goes back to
  // waiting for for another connection.
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

    clientinfo=malloc(sizeof(Client));
    // extract the IP address of the client convert it to a string.
    inet_ntop(AF_INET, &(clientaddr.sin_addr), clientinfo->ip, INET_ADDRSTRLEN);
    // also provide the port of the client to the thread
    clientinfo->port=clientaddr.sin_port;
    // and finally, the handle.
    clientinfo->fd=fd;

    // create a new thread to handle the client that has connected and hand off
    // duties for this client.
    pthread_create(&client_thread, NULL, Thread, clientinfo);
  }
}

// thread to handle one client connection.  The thread terminates when
// this function exits, which happens when the client disconnects.
void *Thread(void *args) {
  Client *clientinfo=(Client *)args;
  char buf[BUFSIZE+1];
  pthread_t myid=pthread_self();

  printf("Thread %p is alive and serving new client @ %s:%d\n",
	 myid, clientinfo->ip, clientinfo->port);
  bzero(buf, BUFSIZE+1);
  while(read(clientinfo->fd, buf, BUFSIZE) > 1) {
    printf("Thread %p received from client: %s", myid, buf);
    write(clientinfo->fd, buf, strlen(buf)+1);
    bzero(buf, BUFSIZE+1);
  }
  printf("Thread %p lost connection with client, dying.\n", myid);
  free(args);
  return NULL;
}

