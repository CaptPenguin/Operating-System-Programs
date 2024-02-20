// # #    # #    # ###### #   #      ####  
// # ##   # #   #  #       # #      #    # 
// # # #  # ####   #####    #       #      
// # #  # # #  #   #        #   ### #      
// # #   ## #   #  #        #   ### #    # 
// # #    # #    # ######   #   ###  ####  
//
// Asynchronous character I/O.  Emulates INKEY$ in BASIC. Works only
// under Unix!  Useful for simple games and other situations where you
// want to monitor input but *not* block while doing so.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#define ESC 27
#define SPACE 32

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  
  struct termios current_term, raw_term;

  // get current terminal settings
  tcgetattr(STDIN_FILENO, &current_term);
  raw_term = current_term;

  // "canonical" mode in the Unix terminal essentially means
  // "line-oriented".  The following turns this (and echo) OFF by 
  // zeroing the bits corresponding to ECHO and ICANON.
  raw_term.c_lflag &= ~(ECHO | ICANON);

  // minimum # of chars to read is 0 and wait time is 0, so we get
  // control back immediately when reading
  raw_term.c_cc[VMIN] = 0;
  raw_term.c_cc[VTIME] = 0;

  // apply changes
  tcsetattr(STDIN_FILENO, TCSANOW, &raw_term);

  char ch = 0; 
  int numchars = 0;   
  do {
    numchars = read(STDIN_FILENO, &ch, 1);
    if (numchars > 0) {
      printf("You pressed the key corresponding to ASCII character %1d ('%c').\n",
	     ch, 
             (ch >= SPACE && ch < 127) ? ch : ' ');
    }
  } while (ch != ESC);

  // chew up any remaining characters 
  while(read(STDIN_FILENO, &ch, 1)==1);

  // restore terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &current_term);
  
  return 0;
}

