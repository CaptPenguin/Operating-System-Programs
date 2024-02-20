// #####  ######   ##   #####        ##### #    # #####  ######   ##   #####   ####       ####
// #    # #       #  #  #    #         #   #    # #    # #       #  #  #    # #          #    #
// #    # #####  #    # #    # #####   #   ###### #    # #####  #    # #    #  ####      #
// #    # #      ###### #    #         #   #    # #####  #      ###### #    #      # ### #
// #    # #      #    # #    #         #   #    # #   #  #      #    # #    # #    # ### #    #
// #####  ###### #    # #####          #   #    # #    # ###### #    # #####   ####  ###  ####
//
// Illustrates interaction between fork() and thread creation.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// function prototypes for fork_threads.c functions
int main(int argc, char** argv);     
void *Thread(void *args);

int main(int argc, char **argv) {
  int pid;
  pthread_t thread1, thread2;

  printf("PARENT: Creating thread # 1.\n");
  pthread_create(&thread1, NULL, Thread, "1");
  printf("PARENT: Creating thread # 2.\n");
  pthread_create(&thread2, NULL, Thread, "2");
  printf("PARENT: Threads created, forking child.\n");
  printf("Parent creating 2 threads and will fork() in 5 seconds. The point of this\n");
  printf("program is to observe that in 5 seconds, you will *not* see heartbeats from any\n");
  printf("additional threads...a fork() does *not* create threads for a child process.\n");
  sleep(5);
  printf("About to fork()...\n");
  pid = fork();
  printf("Forked.\n");

  if (pid) {  // parent
    while (1) {
      printf("PARENT: heartbeat\n");
      sleep(2);
    }
  }
  else {     // child
    while (1) {
      printf("CHILD:  heartbeat\n");
      sleep(2);
    }
  }
}

//////////////////////////////
// This function is the thread body.  Prints thread ID, sleeps, and repeats.

void *Thread(void *args) {

  while (1) {
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    printf("THREAD %llu: heartbeat\n", (uint64_t)tid);
    sleep (2);
  }
  return NULL;
}

