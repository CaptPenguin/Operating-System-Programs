// #    # #    # #       #####  # #####  ######  ####       ####  
// #    # ##   # #       #    # # #    # #      #          #    # 
// #    # # #  # # ##### #    # # #    # #####   ####      #      
// #    # #  # # #       #####  # #####  #           # ### #      
// #    # #   ## #       #      # #      #      #    # ### #    # 
//  ####  #    # #       #      # #      ######  ####  ###  ####  
//                                                                
// Unidirectional pipes in C.  Traditionally the popen() system call
// has allowed only unidirectional pipes (either read OR write).  BSD
// (includes macOS) now supports "enhanced" bidirectional pipes using
// the "r+" parameter, but this will not work on Linux, so it's
// strictly non-portable.  Getting portable bidirectional pipes
// requires the use of pipe(), fork(), and dup2() system calls, in
// conjunction.  That more complex case is covered in the
// bidirectional pipes example.
//
// This example requires cowsay to be installed.  This critical
// utility should be installed on any system you actively use,
// anyway. Otherwise, just use a different command in the popen()
// call.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFSIZE 512

int main(int argc, char *argv[]) {
  char buf[BUFSIZE+1];   
  FILE *fd;  // file descriptor attached to standard input for
             // process created with popen().
  
  // popen() creates a new process and then attaches a pipe to either
  // standard input or output of that process.  We'll use "r" to read
  // from the process' output stream.  The only other *standard*
  // option for popen() is "w", which opens the pipe for writing.  BSD
  // supports "r+", which creates a bidirectional pipe.
  if ((fd=popen("cowsay 'Wow, pipes (and cows) are awesome.'", "r"))) {

    // read response from cowsay command
    while (! feof(fd)) {
      fgets(buf, BUFSIZE, fd);
      printf("%s", buf);
    }
    // close is done with pclose() for handles obtained via popen().
    pclose(fd);
  }
  else {
    perror("popen() called failed");
 }
  return 0;
}

