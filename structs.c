//  ####  ##### #####  #    #  ####  #####  ####       ####  
// #        #   #    # #    # #    #   #   #          #    # 
//  ####    #   #    # #    # #        #    ####      #      
//      #   #   #####  #    # #        #        # ### #      
// #    #   #   #   #  #    # #    #   #   #    # ### #    # 
//  ####    #   #    #  ####   ####    #    ####  ###  ####
//
// Declaration and initialization of struct and union types in C.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FALSE 0
#define TRUE 1

#define NONE_OF_YOUR_BUSINESS 55

typedef struct Person {
  int age;
  char name[15];
  int dead;
} Person;

typedef struct SFavorite {
  char color[15];
  int number;
} SFavorite;

typedef union UFavorite {
  char color[15];
  int number;
} UFavorite;

typedef struct Favorite {
  char which;  // 0 == color, 1 == number
  UFavorite u;
} Favorite;

int main(int argc, char *argv[]) {

  SFavorite s, *s1;         // variable, pointer variable
  UFavorite u, *u1;          
  Favorite f, *f1;

  // simple initializations
  printf("Initializing s.\n");
  s.number=13;
  strcpy(s.color,"purple");
  printf("s: s.number = %d, s.color = %s.\n", s.number, s.color);

  printf("Initializing u.\n");
  u.number = 17;
  printf("u: before setting color: u.number = %d.\n", u.number);
  strcpy(u.color, "red");
  printf("u: after setting color:  u.number = %d.\n", u.number);

  printf("Initializing f.\n");
  f.u.number = 22;
  printf("f: f.u.number = %d.\n", f.u.number);

  // for pointers to structures or unions, you *must* malloc() space for the
  // struct!  Then to access the members, use -> instead of .

  printf("Initializing s1.\n");
  s1 = malloc(sizeof(SFavorite));
  s1->number=13;                          // equivalent to (*s1).number
  strcpy(s1->color,"purple");
  printf("s1: s1->number = %d, s1->color = %s.\n", s1->number, s1->color);

  printf("Initializing u1.\n");
  u1 = malloc(sizeof(UFavorite));
  u1->number = 17;          
  printf("u1: before setting color: u1.number = %d.\n", u1->number);
  strcpy(u1->color, "red");
  printf("u1: after setting color:  u1.number = %d.\n", u1->number);

  printf("Initializing f1.\n");
  f1 = malloc(sizeof(Favorite));
  f1->u.number = 22;
  printf("f1: f1->u.number = %d.\n", f1->u.number);

  // free all allocated space
  free(s1);
  s1=NULL;
  free(u1);
  u1=NULL;
  free(f1);
  f1=NULL;

  return 0;
}

