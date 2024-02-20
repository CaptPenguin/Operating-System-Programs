// ######  ####  #####  #    #      ####  
// #      #    # #    # #   #      #    # 
// #####  #    # #    # ####       #      
// #      #    # #####  #  #   ### #      
// #      #    # #   #  #   #  ### #    # 
// #       ####  #    # #    # ###  ####  
//
// Simple example illustrating the use of fork() to create child processes.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// GLOBAL VARIABLES
int a=55;

void child(void) {
  int pid;

 printf("A child is born! Current value of a is %d, setting my a to 666.\n", a);
  a=666;
  printf("In the child:  a=%d.\n", a);  
}

int main(int argc, char *argv[]) {
  int pid;
  int sum=0;
  int status;

  printf("BEFORE fork in parent.  Value of a is %d.\n", a);
  pid = fork();

  if (pid > 0) {  // parent--return value is PID
    printf("Parent successfully created child with pid %d.\n", pid);
    printf("AFTER fork in parent.  Value of a is %d.\n", a);
    
    printf("Parent waiting for the child to finish...\n");
    waitpid(pid, &status, 0);
    
    printf("Parent: child has finished and a=%d.\n", a);
    
  }
  else {          // child--return value is zero to indicate child-hood
    child();
  }
  
  return 0;
}


