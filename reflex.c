// ########  ######## ######## ##       ######## ##     ##      ######  
// ##     ## ##       ##       ##       ##        ##   ##      ##    ## 
// ##     ## ##       ##       ##       ##         ## ##       ##       
// ########  ######   ######   ##       ######      ###        ##       
// ##   ##   ##       ##       ##       ##         ## ##       ##       
// ##    ##  ##       ##       ##       ##        ##   ##  ### ##    ## 
// ##     ## ######## ##       ######## ######## ##     ## ###  ######  
//
// Simple, tunable game for testing reflexes.  Works under
// Linux. Issue the following command before compiling and using:
//
// $ sudo apt install espeak
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#define BACKSPACE 8
#define ESC 27
#define SPACE 32

#define FALSE 0
#define TRUE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

// set the terminal to raw mode and enable use of inkey()
void set_term_raw(struct termios *current_term) {

  struct termios raw_term;

  // get current terminal settings
  tcgetattr(STDIN_FILENO, current_term);
  raw_term = *current_term;

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
}


// return the terminal to normal
void unset_term_raw(struct termios old_term) {

  // restore terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}


// getchar() clone that returns immediately, regardless of whether a
// keystoke was read.  0 is returned if there is no keystroke
// available, otherwise the ASCII code of the keystroke is returned.
// The terminal must already be in raw mode for this function to work.
int inkey(void) {

  char ch;

  return read(STDIN_FILENO, &ch, 1) > 0 ? ch : 0;
}


int main(int argc, char *argv[]) {
  
  struct termios current_term;  // current state of terminal
  char ch;                      // character read from keyboard
  char char_to_type;            // character user must type (quickly) to win
  unsigned int difficulty = 20; // default difficulty level, where 0 is impossible
  unsigned int timer;           // countdown to failure
  unsigned char success;        // character successfully typed in time?
  unsigned int i;               // index
  unsigned char done=FALSE;     // sick of this game?


  if (argc > 1 && atoi(argv[1]) >= 0 && atoi(argv[1]) <= 100) {
    difficulty = atoi(argv[1]);
  }
  else {
    printf("Using default difficulty level %d.\n"
	   "Use ./%s <level> (0-100) to change.\n\n",
	   difficulty,
	   argv[0]);
      }
  
  // seed random number generator
  srandom(time(NULL));

  // put terminal in raw mode so inkey() will work properly
  set_term_raw(&current_term);

  while (! done) {
    // choose a character between 'a' and 'z'
    char_to_type = 'a' + random() % 26;
    
    // chew up any pending characters 
    while(inkey() > 0);
    
    // print make the user nervous
    printf("READY...\n");
    usleep(1000000 + random() % 1500000);
    printf("SET...\n");
    usleep(1000000 + random() % 1500000);
    printf("\n\n");
    
    // display key that needs to be pressed...
    
    printf("*** TYPE THE CHARACTER %c ***", char_to_type);
    fflush(stdout);
    
    // ...but only for a short time
    success = FALSE;
    timer=0;
    do {
      ch = inkey();
      if (ch == char_to_type || ch == toupper(char_to_type)) {
	success = TRUE;
      }
      timer++;
      usleep(30000);
    } while (! success && timer <= difficulty);

    // was the user fast enough?
    if (success) {
      system("(say success || espeak success) 2> /dev/null");
    }
    else {
      system("(say \"fail fail fail\" || espeak \"fail fail fail\") 2> /dev/null");
    }
    
    // erase the message
    for (i=0; i < 30; i++) {
      usleep(40000);
      putchar(BACKSPACE);
      putchar(SPACE);
      putchar(BACKSPACE);
      fflush(stdout);
    }
    
    // affirm failure or success visually
    if (success) {
      printf("YESSSSSSSSSS.\n");
    }
    else {
      printf("FAIL.\n");
    }

    // chew up any pending characters 
    while(inkey() > 0);
    
    printf("Again (y/n)?\n");
    while (! (ch = inkey()));
    
    done = toupper(ch) != 'Y';
  }
    
  // chew up any pending characters 
  while(inkey() > 0);
  
  // restore the terminal to normal
  unset_term_raw(current_term);
  
  return 0;
}

