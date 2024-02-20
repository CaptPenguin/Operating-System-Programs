//  ####  ###### #####  ####  #    #   ##   #####       ####  
// #    # #        #   #    # #    #  #  #  #    #     #    # 
// #      #####    #   #      ###### #    # #    #     #      
// #  ### #        #   #      #    # ###### #####  ### #      
// #    # #        #   #    # #    # #    # #   #  ### #    # 
//  ####  ######   #    ####  #    # #    # #    # ###  ####
//  
// Read characters from standard input and evaluate them.  This is
// unworthy as an example except as a prelude to asynchronous I/O in
// inkey.c.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#define ESC 27
#define SPACE 32

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  
  char ch = 0;
  do {
    ch = getchar();
    printf("You pressed the key corresponding to ASCII character %1d ('%c').\n",
	   ch, 
	   (ch >= SPACE && ch < 127) ? ch : ' ');
  } while (ch != ESC);
  
  // chew up any remaining characters 
  while(read(STDIN_FILENO, &ch, 1)==1);
  
  return 0;
}

