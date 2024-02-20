//  ####  ##### #####  # #    #  ####   ####       ####  
// #        #   #    # # ##   # #    # #          #    # 
//  ####    #   #    # # # #  # #       ####      #      
//      #   #   #####  # #  # # #  ###      # ### #      
// #    #   #   #   #  # #   ## #    # #    # ### #    # 
//  ####    #   #    # # #    #  ####   ####  ###  ####  
//                                                       
// this expands the simple C skeleton to illustrate how strings are
// used in C.  Importantly, C strings are simply arrays of characters,
// with the string terminated with a binary 0 character.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  char buf[512];           // a string that can hold a maximum of 511
			   // characters + the terminating NULL


  buf[0]=0;                // string is now empty

  strcat(buf, "Hi ");      // string is now "Hi "
  strcat(buf, "there.");   // string is now "Hi there."

  // output length of string
  printf("The string is %lu characters in length.\n", strlen(buf));  

  char *location;

  // returns pointer to first 't' or NULL if not found
  location = strchr(buf, 't');  

  if (location) {
    printf("Found: %s\n", location);
  }
  else {
    printf("'t' not found.\n");
  }

  // returns pointer to first "there" or NULL
  location = strstr(buf, "there");      
  
  if (location) {
    printf("Found: %s\n", location);
  }
  else {
    printf("'t' not found.\n");
  }

  // strcmp returns < 0, 0, or > 0 to indicate
  // s1 < s2, s1 == s2, s1 > s2
  int cmp = strcmp("Hi there.", buf);  
                                       

  if (cmp < 0) {
    printf("\"Hi there.\" < buf\n");
  }
  else if (! cmp) {
    printf("\"Hi there.\" = buf \n");
  }
  else {
    printf("\"Hi there.\" > buf)\n");
  }

  // parse a comma-delimited string into separate components and
  // output them

  char *token, *string, *tofree;

  // duplicate literal string
  tofree = string = strdup("milk,bread,cheese,coffee");   
  
  while ((token = strsep(&string, ",")) != NULL) {
    printf("%s\n", token);
  }

  // must be called to release memory associated with strdup() call
  free(tofree); 
  
  // There are LOTS more string functions!  See, e.g.:
  // https://www.tutorialspoint.com/c_standard_library/string_h.htm

  return 0;
}

