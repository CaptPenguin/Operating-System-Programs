//  ####  #    # #####  #      # #    # ######      ####  
// #    # ##  ## #    # #      # ##   # #          #    # 
// #      # ## # #    # #      # # #  # #####      #      
// #      #    # #    # #      # #  # # #      ### #      
// #    # #    # #    # #      # #   ## #      ### #    # 
//  ####  #    # #####  ###### # #    # ###### ###  ####
//
// Expands the simple C skeleton to illustrate access to command line
// arguments and how simple for loops work.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int i;

  puts("The command line arguments are:");
  
  for (i=0; i < argc; i++) {
    puts(argv[i]);
  }

  return 0;
}

