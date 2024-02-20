//  ####   ####  #####  ##### # #    # #####  ####       ####  
// #      #    # #    #   #   # ##   #   #   #          #    # 
//  ####  #    # #    #   #   # # #  #   #    ####      #      
//      # #    # #####    #   # #  # #   #        # ### #      
// #    # #    # #   #    #   # #   ##   #   #    # ### #    # 
//  ####   ####  #    #   #   # #    #   #    ####  ###  ####  
//
// Dynamically create and initialize a 1D array of integers, then use
// qsort() to sort the integers into ascending order.  A bit easier to
// understand than the strings example in dstrings.c, because the
// dynamic memory allocation necessary for integers is easier than for
// C strings.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

// a more "laid back" approach for the qsort() comparison function:

int cmp(const void *p1, const void *p2) {
  int *ip1=(int *)p1;
  int *ip2=(int *)p2;

  int s1 = *ip1;
  int s2 = *ip2;
  
  return s1 - s2;   // -, 0, +  for less than, equal, greater than
}


// the version that most experienced C programmers would write
//int cmp(const void *p1, const void *p2) {
//  
//  return *((int *)p1) - *((int *)p2);  <--------------
//
//}


int main(int argc, char *argv[]) {
  int *ints = NULL;
  int something[5];     
  int i,n, len;
  char a=64;
  char b='A';   // == 64
  char buf[BUFSIZE+1];

  printf("Enter # of ints:");
  fflush(stdout);
  fgets(buf, BUFSIZE, stdin);             // scanf()  
  n=atoi(buf);
  ints=malloc(n * sizeof(int));
  for (i=0; i < n; i++) {
    printf("Enter int # %d.\n", i+1);
    fgets(buf, BUFSIZE, stdin);
    ints[i]=atoi(buf);
  }

  qsort(ints, n, sizeof(int), cmp);  

  printf("Sorted ints:\n");
  for (i=0; i < n; i++) {
    printf("%d\n", ints[i]);
  }

  free(ints);
  
  return 0;
}

