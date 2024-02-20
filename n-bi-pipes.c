// #    #       #####  #       #####  # #####  ######  ####       ####  
// ##   #       #    # #       #    # # #    # #      #          #    # 
// # #  # ##### #####  # ##### #    # # #    # #####   ####      #      
// #  # #       #    # #       #####  # #####  #           # ### #      
// #   ##       #    # #       #      # #      #      #    # ### #    # 
// #    #       #####  #       #      # #      ######  ####  ###  ####  
//
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
// There are some significant limitations with this approach if the
// program being controlled is interactive.  Specifically, the program
// being controlled must always execute fflush() calls to stdout after
// each printf(), et al. or call setvbuf(stdout, NULL, _IONBF, 0).
//
// Should work on either MacOS or Linux.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/wait.h> 

#define BUFSIZE 100

int main(int argc, char *argv[]) {
  char buf[BUFSIZE+1];
  // we need TWO pipes.  Data written to the handle at index [1] can
  // be read from the handle at index [0].  
  int fd1[2];  // handles for both ends of one pipe
  int fd2[2];  // handles for both ends of the other pipe
  pid_t p;     // PID of created process
  unsigned int num1, num2, answer;
  char operator;

  // pipe() takes an array of 2 file handles as input and returns 0
  // if it succeeds
  
  if (! pipe(fd1) && ! pipe(fd2)) {
    // yay!  Both pipe() calls succeeded.  This is expected.

    // fork a new process that we will interact with
    p = fork();
    if (p) {

      // we don't use these
      close(fd1[0]);
      close(fd2[1]);
      
      // try to win the numbers game
      
      FILE *in=fdopen(fd2[0], "r");
      if (! in) {
	printf("fdopen() failed on input.\n");
	exit(0);
      }
      FILE *out=fdopen(fd1[1], "w");
      if (! out) {
	printf("fdopen() failed on output.\n");
	exit(0);
      }

      printf("Waiting on some input from the child...\n");
      while (fgets(buf, BUFSIZE, in) > 0) {
	printf("Parent received %s", buf);
	sscanf(buf, "%d%c%d?", &num1, &operator, &num2);
	printf("Parsing: %1d %c %1d\n", num1, operator, num2);
	answer=operator == '+' ? num1+num2 :
	  operator == '-' ? num1-num2 :
	  operator == '*' ? num1*num2 : num1/num2;
	sprintf(buf ,"%1d", answer);
	printf("Sending answer %s to child.\n", buf);
	fputs(buf, out);
	fputs("\n", out);   // puts outputs a newline, while fputs() does not!
	fflush(out);
	fgets(buf, BUFSIZE, in);
	printf("Child responded: %s", buf);
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

      // We'd love for this to work on this side of the pipes, but it
      // doesn't.  The C standard library evaluates interactivity
      // after the exec() call and will reset buffering to full
      // (instead of line-oriented) if the program isn't connected to
      // a tty (i.e.., it's non-interactive).  
      setvbuf(stdout, NULL, _IONBF, 0);  //   :(  

      // 1st arg to execlp is program to run
      // 2nd arg is argv[0] == name of program
      // NULL terminates the list of arguments
      execlp("./numbers", "numbers", NULL);
    }
  }
  else {
    perror("Sorry, man. pipe() call failed: ");
  }
  
  return 0;
} 

