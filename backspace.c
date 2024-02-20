// #####    ##    ####  #    #  ####  #####    ##    ####  ######      ####  
// #    #  #  #  #    # #   #  #      #    #  #  #  #    # #          #    # 
// #####  #    # #      ####    ####  #    # #    # #      #####      #      
// #    # ###### #      #  #        # #####  ###### #      #      ### #      
// #    # #    # #    # #   #  #    # #      #    # #    # #      ### #    # 
// #####  #    #  ####  #    #  ####  #      #    #  ####  ###### ###  ####  
//
// Illustrates simple backspacing in C.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

  printf("HELLO");
  fflush(stdout);
  for (int i=0; i < strlen("HELLO"); i++) {
    sleep(1);
    putchar(8);
    putchar(' ');
    putchar(8);
    fflush(stdout);
  }
    	     
  return 0;    
}
 
