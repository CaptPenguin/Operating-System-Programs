//                                                                        
// ######  ####  #####  #    # #####  ##### #   #       #    #      ####  
// #      #    # #    # #   #  #    #   #    # #        ##   #     #    # 
// #####  #    # #    # ####   #    #   #     #   ##### # #  #     #      
// #      #    # #####  #  #   #####    #     #         #  # # ### #      
// #      #    # #   #  #   #  #        #     #         #   ## ### #    # 
// #       ####  #    # #    # #        #     #         #    # ###  ####  
//                                                                       
// Illustrates use of forkpty() to fork/exec a process and then interact with
// the process via its stdin/stdout streams.  This will work even with programs
// that do not set line buffering or use fflush() operations after after output
// operation.
//
// Different include files are necessary depending on whether Linux or
// Mac (BSD) is the target.  
// 
// Compile:
//
// gcc -o forkpty-n forkpty-n.c -lutil
//
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sysexits.h>

#if defined(__APPLE__)
#include <util.h>
#endif

#if defined(__linux)
#include <pty.h>
#endif

#define BUFSIZE 512
  
int main(int argc, char *argv[]) { 
  
  int master;  // master file handle for pty

  FILE *out;   // buffered output stream associated with master
  FILE *in;    // buffered input stream associated with master
  
  char buf[BUFSIZE];
  pid_t p;
  int stop=0;
  int num1, num2, answer;
  char operator;

  // fork process in pty
  p = forkpty(&master, NULL, NULL, NULL);
  if (p < 0) {
    perror("forkpty failed");
    return 0;
  }
  else if (p > 0) {
    // parent

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // prevent echo from the pty (!!!!!!!!!!!!)
    struct termios tios;
    tcgetattr(master, &tios);
    tios.c_lflag &= ~(ECHO | ECHONL);
    tcsetattr(master, TCSAFLUSH, &tios);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    // get buffered input descriptor associated with pty
    in=fdopen(master, "r");
    if (! in) {
      printf("fdopen on read stream failed, exiting.\n");
	return 0;
    }

    // get buffered output descriptor associated with pty
    out=fdopen(master, "w");
    if (! out) {
      printf("fdopen on write stream failed, exiting.\n");
      return 0;
    }

    // now can interact with child using handles in and out!
    
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
    // child
    execlp("./numbers", "numbers", NULL);
  }
  
  return 0;
}

