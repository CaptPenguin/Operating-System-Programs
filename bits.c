// #####  # #####  ####       ####  
// #    # #   #   #          #    # 
// #####  #   #    ####      #      
// #    # #   #        # ### #      
// #    # #   #   #    # ### #    # 
// #####  #   #    ####  ###  ####  
//
// hex!  binary!  bits bits bits!  octal!  Ok, not octal.  Not right now.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// convert an arbitrary precision integer to binary. 'bits' must be
// divisible by 8.
char *int_to_binary(void *num, long bits, char *buffer) {

  long i, j;
  char *o, *n;
    
  if (bits % 8) {
    printf("int_to_binary: 'bits' parameter must be divisible by 8.\n");
    exit(0);
  }
  
  o = n = malloc(bits / 8);
  memcpy(n, num, bits / 8);
  
  bzero(buffer, bits / 8 + 1);
  
  for (j=0; j < bits / 8; j++) {
    for (i = 7; i >= 0; i--) {
      *buffer = (*n & 1) + '0';
      buffer++;
      *n >>= 1;
    }
    n++;
  }

  free(o);
  
  return buffer;
}


// set or clear the 'z'th bit (numbered from 0) in a number with
// 'bits' bits according to 'flag' (0 = clear, otherwise set).
void touch_bit(void *num, long bits, long z, unsigned char flag) {

  unsigned char *n = (unsigned char *)num;

  if (flag) {
    // set bit
    //    n[z / 8] |= (1 << (7 - (z % 8)));
    n[z / 8] |= (1 << (z % 8));
  }
  else {
    // clear bit
    //    n[z / 8] &= ~(1 << (7 - (z % 8)));
    n[z / 8] &= ~(1 << (z % 8));
  }
}

// pretty print a binary number represented as a null-terminated
// string of ASCII 0s and 1s.  The length of 'buffer' must be
// divisible by 8.  If 'hex' is TRUE, then associated hex digits are
// also output.
void pretty_print_binary(char *buffer, unsigned char hex) {

  long i, j, k, bit;
  unsigned char hexdigits;
    
  if (strlen(buffer) % 8) {
    printf("pretty_print_binary: length of 'buffer' must be divisible by 8.\n");
    exit(0);
  }

  bit=0;
  for (i=0; i < strlen(buffer) / 8; i++) {
    for (j=0; j < 8; j++) {
      putchar(buffer[bit]);
      bit++;
    }
    putchar(' ');
  }
  putchar('\n');

  if (hex) {
    bit=0;
    for (i=0; i < strlen(buffer) / 8; i++) {
      hexdigits=0;
      for (j=0; j < 8; j++) {
	if (buffer[bit] == '1') {
	  hexdigits += (unsigned char)pow(2, bit % 8);
	}
	bit++;
      }
      for (k=0; k < (! i ? 3 : 7); k++) {
	putchar(' ');
      }
      printf("%02x", hexdigits);
    }
    putchar('\n');
  }
}


int main(int argc, char *argv[]) {

  char buf[512];
  unsigned int n;

  n = 1;
  int_to_binary(&n, sizeof(n) * 8, buf);
  printf("%u = \n", n);
  pretty_print_binary(buf, 1);

  putchar('\n');

  printf("Setting bit # 2.\n");
  touch_bit(&n, sizeof(n) * 8, 2, 1);
  int_to_binary(&n, sizeof(n) * 8, buf);
  printf("%u = \n", n);
  pretty_print_binary(buf, 1);
  
  putchar('\n');

  printf("Setting bit # 12.\n");
  touch_bit(&n, sizeof(n) * 8, 12, 1);
  int_to_binary(&n, sizeof(n) * 8, buf);
  printf("%u = \n", n);
  pretty_print_binary(buf, 1);

  putchar('\n');

  printf("Setting bit # 31.\n");
  touch_bit(&n, sizeof(n) * 8, 31, 1);
  int_to_binary(&n, sizeof(n) * 8, buf);
  printf("%u = \n", n);
  pretty_print_binary(buf, 1);

  return 0;
}

