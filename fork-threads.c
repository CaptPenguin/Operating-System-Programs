// ######  ####  #####  #    #       ##### #    # #####  ######   ##   #####   ####       ####
// #      #    # #    # #   #          #   #    # #    # #       #  #  #    # #          #    #
// #####  #    # #    # ####   #####   #   ###### #    # #####  #    # #    #  ####      #
// #      #    # #####  #  #           #   #    # #####  #      ###### #    #      # ### #
// #      #    # #   #  #   #          #   #    # #   #  #      #    # #    # #    # ### #    #
// #       ####  #    # #    #         #   #    # #    # ###### #    # #####   ####  ###  ####
//
// Illustrates interaction between fork(), threads, and shared data.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// function prototypes for fork-threads.c functions
int main(int argc, char *argv[]);     
void *Thread(void *args);

// globals
int val1 = 1, val2 = 2;

pthread_mutex_t parent_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t child_lock = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char *argv[]) {
  int pid;
  pthread_t thread1, thread2;

  printf("PARENT: Creating thread.\n");
  pthread_create(&thread1, NULL, Thread, "parent");
  printf("PARENT: Thread created, forking child.\n");
  pid = fork();

  if (pid) {  // parent
    pthread_mutex_lock(&parent_lock);
    printf("PARENT: val1 = %d, val2 = %d.\n", val1, val2);
    pthread_mutex_unlock(&parent_lock);

    sleep(3);

    pthread_mutex_lock(&parent_lock);
    printf("PARENT: val1 = %d, val2 = %d.\n", val1, val2);
    pthread_mutex_unlock(&parent_lock);
  }
  else {     // child
    printf("CHILD: Creating thread.\n");
    pthread_create(&thread2, NULL, Thread, "child");
    printf("CHILD: Thread created.\n");
    
    pthread_mutex_lock(&child_lock);
    printf("CHILD: val1 = %d, val2 = %d\n", val1, val2);
    pthread_mutex_unlock(&child_lock);
    
    sleep(3);
    
    pthread_mutex_lock(&child_lock);
    printf("CHILD: val1 = %d, val2 = %d\n", val1, val2);
    pthread_mutex_unlock(&child_lock);
  }
}

//////////////////////////////
// This function is the thread body.  Prints thread ID, sleeps, and repeats.

void *Thread(void *args) {

  if (! strcmp(args,"child")) {
    pthread_mutex_lock(&child_lock);
    printf("CHILD's THREAD: val1 = %d, val2 = %d.\n", val1, val2);
    printf("CHILD's THREAD: Setting val1 = 98, val2 = 99.\n");
    val1 = 98;
    val2 = 99;
    pthread_mutex_unlock(&child_lock);
  }
  else {
    pthread_mutex_lock(&parent_lock);
    printf("PARENT's THREAD: val1 = %d, val2 = %d.\n", val1, val2);
    printf("PARENT's THREAD: Setting val1 = 50, val2 = 51.\n");
    val1 = 50;
    val2 = 51;
    pthread_mutex_unlock(&parent_lock);
  }
  return NULL;
}


