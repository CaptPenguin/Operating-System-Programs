// ######  ####  #####  #    # ###### #    # ######  ####       ####  
// #      #    # #    # #   #  #       #  #  #      #    #     #    # 
// #####  #    # #    # ####   #####    ##   #####  #          #      
// #      #    # #####  #  #   #        ##   #      #      ### #      
// #      #    # #   #  #   #  #       #  #  #      #    # ### #    # 
// #       ####  #    # #    # ###### #    # ######  ####  ###  ####  
//
// Simple example illustrating the use of fork() and exec() to create
// child processes.  Relies on execme.c.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pid;
  int sum=0;
  int status;
  int a=55;
  
  pid = fork();

  if (pid) {  // parent
    printf("Successfully created child with pid %d.\n", pid);
    
    printf("Parent waiting for the child to finish...\n");
    waitpid(pid, &status, 0);
    
    printf("Important: child has finished.\n");
    
  }
  else {     // child
    // use exec to run a different program!
    execv("./execme", 0);
    // shouldn't make it here, otherwise an error occurred!
    perror("Something bad happened in execv() call");
  }
  
  return 0;
}

