// #    # #    # #####  #    # ###### #  ####       ####  
// #    # ##   # #    # #    # #      # #    #     #    # 
// #    # # #  # #####  #    # #####  # #    #     #      
// #    # #  # # #    # #    # #      # #    # ### #      
// #    # #   ## #    # #    # #      # #    # ### #    # 
//  ####  #    # #####   ####  #      #  ####  ###  ####  
//
// Unbuffered ("low level") I/O in C.  Illustrates some of the more
// important functions related to unbuffered I/O in the C standard
// library.
//
// Written by Golden G. Richard III (@nolaforensix / golden@cct.lsu.edu).
//

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

#define BUFSIZE 100

typedef struct Fancy {
  int a;
  char str[BUFSIZE+1];
  double c;
} Fancy;


int main(int argc, char *argv[]) {
  char buf[BUFSIZE+1];   
  int fd;                  // file descriptors for unbuffered I/O are
			   // just ints
  Fancy f;                 // one Fancy f!
  int len;                 // plength of a string written to file
  struct stat fileinfo;    // struct used by fstat() to return lots of
			   // info about file
  
  // Unix files are just containers that hold bytes--there's no OS-imposed
  // structure.  Read and write whatever you want.

  // man open for all the details!  
  if ((fd=open("important.dat", O_RDWR | O_CREAT | O_TRUNC,
	       S_IRUSR | S_IWUSR)) > -1) {

    // write a string
    strcpy(buf, "Hi there. I'm some data.");
    len=strlen("Hi there. I'm some data.")+1;
    // unbuffered write returns the number of bytes written, unlike
    // fwrite()!
    if (write(fd, buf, len) != len) {
      perror("write() # 1 failed");
    }      

    // write a struct
    f.a=17;
    strcpy(f.str, "Hi there!");
    f.c=3.1415926535;
    if (write(fd, &f, sizeof(f)) != sizeof(f)) {
      perror("write() # 2 failed");
    }

    // lseek() both seeks and returns position after seek. 

    printf("File is exactly %lld bytes long.\n\n",
	   lseek(fd, 0, SEEK_CUR));

    // rewind so we can read the stuff back in and check it.
    if (lseek(fd, 0, SEEK_SET) == -1) {
      // error
      perror("lseek() failed");
    }
    
    // don't cheat...zero the buffer to make sure the data is really
    // read from the file.
    bzero(&f, sizeof(f));
    bzero(buf, BUFSIZE+1);

    // read stuff back in...read() returns number of bytes read,
    // unlike fread().
    if (read(fd, buf, len) != len) {
      perror("read() # 1 failed");
    }
    
    if (read(fd, &f, sizeof(f)) != sizeof(f)) {
      perror("read() # 2 failed");
    }
    
    printf("String: \"%s\"\n", buf);
    printf("f: %d, \"%s\", %10.10f\n", f.a, f.str, f.c);
    
    // now learn some interesting stuff about the file.  The stat
    // structure contains lots more, but these are the most
    // interesting fields.
    
    if (fstat(fd, &fileinfo) != -1) {
      printf("inode for file:      %lu\n", (unsigned long)fileinfo.st_ino);
      printf("# of blocks in file: %lu\n", (unsigned long)fileinfo.st_blocks);
      printf("file size in bytes:  %lu\n", (unsigned long)fileinfo.st_size);
    }
    else {
      perror("fstat() call failed");
    }

    // close the file
    close(fd);
  }
  else {
    perror("open() called failed");
  }

  return 0;
}

