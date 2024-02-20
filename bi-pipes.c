// #####  #       #####  # #####  ######  ####       ####  
// #    # #       #    # # #    # #      #          #    # 
// #####  # ##### #    # # #    # #####   ####      #      
// #    # #       #####  # #####  #           # ### #      
// #    # #       #      # #      #      #    # ### #    # 
// #####  #       #      # #      ######  ####  ###  ####  
//
// Bidirectional pipes in C.  Traditionally the popen() system call
// has allowed only unidirectional pipes (either read OR write).  BSD
// (includes macOS) now supports "enhanced" bidirectional pipes using
// the "r+" parameter, so if you're programming a system like that,
// you might consider using popen(), but your implementation will then
// be non-portable.  We'll use pipe(), fork(), and dup2() system calls
// to get bidirectional pipes, which is more complex, but also more
// powerful as well as being portable.
//
// Should work on either MacOS or Linux.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include<sys/types.h> 
#include<sys/wait.h> 

#define BUFSIZE 100

int main(int argc, char *argv[]) {
  char buf[BUFSIZE+1];
  // we need TWO pipes.  Data written to the handle at index [1] can
  // be read from the handle at index [0].  
  int fd1[2];  // handles for both ends of one pipe
  int fd2[2];  // handles for both ends of the other pipe
  pid_t p;     // PID of created process

  // pipe() takes an array of 2 file handles as input and returns 0
  // if it succeeds
  
  if (! pipe(fd1) && ! pipe(fd2)) {
    // yay!  Both pipe() calls succeeded. This is expected.

    // fork a new process that we will interact with
    p = fork();
    
    if (p) {
      // parent process.  Important!  The parent and child share
      // copies of the file handles associated with the pipes and can
      // use these to communicate!  If we write to fd1[1], the child
      // receives this data through stdin because of the dup2() call in
      // the child code.  Similarly, if we read from fd2[0], we
      // get the child's stdout, because of the dup2() call in the child.

      // we don't use these
      close(fd1[0]);
      close(fd2[1]);

      // now write some integers to fd1[1] (== stdin for child)...

      printf("Parent writing integers to child...\n");
      write(fd1[1], "10\n", strlen("10\n"));
      write(fd1[1], "8\n",  strlen("8\n"));
      write(fd1[1], "12\n", strlen("12\n"));
      write(fd1[1], "3\n",  strlen("3\n"));
      write(fd1[1], "13\n", strlen("13\n"));
      write(fd1[1], "1\n",  strlen("1\n"));
      
      printf("Parent done writing.  Closing output pipe.\n");

      // let the other program see end of file
      close(fd1[1]);
      
      // now read some integers from fd2[0]...and they had better be
      // sorted!  Here's a trick that allows use of higher-level
      // buffered I/O functions for pipes: fdopen()

      FILE *fp=fdopen(fd2[0], "r");  
      
      printf("Parent reading integers from child...\n");
      while (fgets(buf, BUFSIZE, fp) > 0) {
	printf("Parent received %s", buf);
      }
      printf("Parent done.\n");
    }      
    else {
      // child process
      printf("Child hacking file handles...\n");

      // we don't use these, so close them
      close(fd2[0]);
      close(fd1[1]);
      
      // dup2() assigns one handle to another.  This hacks stdin for
      // child to be fd1[0]
      dup2(fd1[0], 0);
      close(fd1[0]);
      
      // hack stdout for child to be fd2[1]
      dup2(fd2[1], 1);
      close(fd2[1]);
      
      // what does this mean? stdin for child is associated with
      // fd1[0].  stdout for child is now associated with fd2[1].
      // Functions in the child that read from stdin, e.g., gets(),
      // will automatically take data from fd1[0].  Functions that
      // write to stdout in the child, e.g., printf() or puts(), will
      // automatically write to fd2[1].  Crucially, when we exec(),
      // the new program will **inherit** this mapping!  If we didn't
      // do an exec() call, we could have simply read/write data
      // directly to the pipe endpoints instead of remapping
      // stdin/stdout
      
      // 1st arg to execlp is program to run
      // 2nd arg is argv[0] == name of program ("sort")
      // 3rd arg is argv[1] == '-g' option to sort (sort numerically)
      // NULL terminates the list of arguments
      execlp("sort", "sort", "-g", NULL);  
    }
  }
  else {
    perror("Sorry, man. pipe() call failed: ");
  }
  
  return 0;
} 

