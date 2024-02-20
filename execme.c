// ###### #    # ######  ####  #    # ######      ####  
// #       #  #  #      #    # ##  ## #          #    # 
// #####    ##   #####  #      # ## # #####      #      
// #        ##   #      #      #    # #      ### #      
// #       #  #  #      #    # #    # #      ### #    # 
// ###### #    # ######  ####  #    # ###### ###  ####  
//
// execme is the target of the execv call in forkexec.c
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Hi, I'm the other program and my pid is %d\n", getpid());
  return 0;
}

