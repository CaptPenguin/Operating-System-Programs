// ########     ###     ######  ####  ######       ######  
// ##     ##   ## ##   ##    ##  ##  ##    ##     ##    ## 
// ##     ##  ##   ##  ##        ##  ##           ##       
// ########  ##     ##  ######   ##  ##           ##       
// ##     ## #########       ##  ##  ##           ##       
// ##     ## ##     ## ##    ##  ##  ##    ## ... ##    ## 
// ########  ##     ##  ######  ####  ######  ...  ######  
//
// printf, basic control structures, simple function calls, and
// numeric types in C
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function prototype--helps the compiler do type checking of
// arguments and return values.  ALWAYS use function prototypes.
void do_something(char a, int b, long c, long long d, float e, double f,
		unsigned int g, unsigned long h, unsigned long long i);

// function declaration--actually defines the function mentioned in
// the prototype.  This function illustrates basic numeric types and
// the use of printf().  The function has no return value (that's why the return
// value is "void").
void do_something(char a, int b, long c, long long d, float e, double f,
		unsigned int g, unsigned long h, unsigned long long i) {
  
  printf("a=%c, b=%d, c=%ld, d=%lld, e=%f, f=%f, g=%u, h=%lu, i=%llu\n",
	 a, b, c, d, e, f, g, h, i);
}


int main(int argc, char *argv[]) {

  int a = 5;
  int i;

  // SIMPLE FUNCTION CALL.  All arguments are passed by value.
  do_something('a', -3, 4, 5, 5.6, 3.1415926535, 57, 99, 128);

  // IF
  if (a == 0) {
    printf("a=0\n");
  }
  else if (a == 1) {
    printf("a=1\n");
  }
  else {
    printf("a is neither 0 nor 1.\n");
  }
  

  // WHILE.  The condition (in this case, the integer variable 'a')
  // can be anything that evaluates to a numeric value.  0 is false,
  // anything else is true.  The loop terminates when the condition
  // evaluates to false and is checked BEFORE each loop iteration.
  while (a) {
    a--;
    printf("a=%d\n", a);
  }
  
  a = 4;

  // DO WHILE.  The condition can be anything that evaluates to a
  // numeric value.  0 is false, anything else is true.  The loop
  // terminates when the condition evaluates to false and is checked
  // AFTER each loop iteration.
  do {
    a--;
    printf("a=%d\n", a);
  } while (a > 0);
  
  // FOR (three components, separated by semicolons.  FIRST is
  // initialization, SECOND determines if loop continues and is
  // checked at the TOP of the loop, LAST executes at the BOTTOM of
  // the loop.
  for (i=0; i < 10; i++) {
    a++;
    printf("i=%d, a = %d\n", i, a);
  }
  
  int u, v;
  printf("Enter a value for a, u, v.\n");
  scanf("%d  %d%d", &a, &u, &v );    // DO NOT PUT A NEW LINE IN THERE!!!!!!!!!!!
  
  // SWITCH statement. Used when a variable is compared to a number of
  // discrete choices.  Note that 'break' is REQUIRED at the end of
  // the code for each choice to prevent fall-through!  The 'default'
  // case is triggered if there are no other matches.
  switch (a) {
  case 0:
    printf("a is zero.\n");
    break;
    
  case 1:
    printf("a is one.\n");
    break;

  case 5:
    printf("a is five.\n");
    break;

  default:
    printf("a is neither zero nor one nor five.\n");
    break;
  }

  // TERNARY OPERATOR. Best illustrated with examples. This if statement:

  if (a == 5) {
    i = 7;
  }
  else {
    i = 9;
  }

  // ... can be written using the ternary operator instead (this is
  // obviously much more consise):

  i = (a == 5) ? 7 : 9;

  // The ternary operator asks a question and supplies a different
  // value depending on whether the "answer" is true (any non-zero
  // value!) or false.

  // Here's something (much?) more complex.  The ternary operator can be nested and 
  // the switch above could be implemented using ternary operators like this:
  
  printf("a is %s.\n",
	 ! a ? "zero" : a == 1 ? "one" : a == 5 ? "five" : "neither zero nor one nor five");

  // Of course this isn't more readable.

  return 0;
}

