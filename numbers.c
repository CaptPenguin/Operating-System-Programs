// #    # #    # #    # #####  ###### #####   ####       ####  
// ##   # #    # ##  ## #    # #      #    # #          #    # 
// # #  # #    # # ## # #####  #####  #    #  ####      #      
// #  # # #    # #    # #    # #      #####       # ### #      
// #   ## #    # #    # #    # #      #   #  #    # ### #    # 
// #    #  ####  #    # #####  ###### #    #  ####  ###  ####  
//
// simple arithmetic game, used by bi-directional pipes examples.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXBUFSIZE 512

int main(int argc, char *argv[]) {
  char buf[MAXBUFSIZE];
  char operator;   // +, -, *, or /
  unsigned int num1, num2, answer;

  //  setvbuf(stdout, NULL, _IONBF, 0);  // or fflush()'s everywhere!
  srandom(time(0));

  while (1) {
    operator=random() % 4;
    num1=random()%50+1;
    num2=random()%50+1;

    if (num1 < num2) {
      int temp=num1;
      num1=num2;
      num2=temp;
    }

    switch (operator) {
    case 0:
      operator='+';
      answer=num1+num2;
      break;
    case 1:
      operator='-';
      answer=num1-num2;
      break;
    case 2:
      operator='*';
      answer=num1*num2;
      break;
    case 3:
      operator='/';
      answer=num1/num2;
    }
    printf("%1d%c%1d?\n", num1, operator, num2);
    fflush(stdout);                                    // <---- what if removed?
    fgets(buf, MAXBUFSIZE, stdin);
    buf[strlen(buf)-1]=0;
    //    fprintf(stderr,"Processing answer \"%s\".\n", buf);
    if (atoi(buf) == answer) {
      printf("CORRECT.\n");                                 
      fflush(stdout);                               // <---- what if removed?
    }
    else {
      printf("INCORRECT.\n");
      fflush(stdout);                               // <---- what if removed?
    }
  }
  
  return 0;
}
    
