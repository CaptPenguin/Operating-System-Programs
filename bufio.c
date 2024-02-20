// #####  #    # ###### #  ####       ####  
// #    # #    # #      # #    #     #    # 
// #####  #    # #####  # #    #     #      
// #    # #    # #      # #    # ### #      
// #    # #    # #      # #    # ### #    # 
// #####   ####  #      #  ####  ###  ####  
//
// Illustrates use of some of the more useful buffered I/O functions
// in the standard C library.
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
  FILE *fd;               // file descriptors for buffered I/O are 
	                  // opaque struct pointers--means: you don't
	                  // care what's inside
  Fancy f;                // one Fancy f!
  int len;                // length of a string written to file
  struct stat fileinfo;   // struct used by fstat() to return lots of
			  // info about file

  // Unix files are just containers that hold bytes--there's no OS-imposed
  // structure.  Read and write whatever you want.

  // murder file if it already exists.  If unlink() fails, it just
  // means the file didn't exist, so don't bother to check return
  // value.
  unlink("important.dat");

  // man fopen for all the details!  mode of "w+" indicates we want
  // both read and write access and that the file should be created if
  // it doesn't exist.
  if ((fd=fopen("important.dat", "w+"))) {

    // write a string using fwrite().  Note the position of the file
    // handle in the argument list--it's the *last* argument.  Also note that
    // fwrite() returns the number of *items* read--not the number of bytes.
    strcpy(buf, "Hi there. I'm some data.");
    len=strlen("Hi there. I'm some data.")+1;
    if (fwrite(buf, len, 1, fd) != 1) {
      perror("fwrite() # 1 failed");
    }      

    // write a struct
    f.a=17;
    strcpy(f.str, "Hi there!");
    f.c=3.1415926535;
    if (fwrite(&f, sizeof(f), 1, fd) != 1) {
      perror("fwrite() # 2 failed");
    }
    
    // Unlike lseek(), fseek() does *not* return the current
    // position--you have to ftell() to peek at the current
    // position. Compare the unbuffered example to understand this
    // better.  Since we're already at end of file, the value returned
    // from ftello() is also the file size.
    
    printf("File is exactly %lld bytes long.\n\n",
	   ftello(fd));
    
    // rewind to beginning so we can read the stuff back in and check
    // it.
    if (fseek(fd, 0, SEEK_SET) == -1) {
      // error
      perror("fseek() failed");
    }
    
    // don't cheat...zero the buffer to make sure the data is really
    // read from the file.
    bzero(&f, sizeof(f));
    bzero(buf, BUFSIZE+1);
    
    // read stuff back in. fread() returns number of *items* read
    if (fread(buf, len, 1, fd) != 1) {   
      perror("fread() # 1 failed");
    }
    
    if (fread(&f, sizeof(f), 1, fd) != 1) {
      perror("fread() # 2 failed");
    }
    
    printf("String: \"%s\"\n", buf);
    printf("f: %d, \"%s\", %10.10f\n", f.a, f.str, f.c);

    // now learn some interesting stuff about the file.  The stat
    // structure contains lots more, but these are the most
    // interesting fields.  Note use of fileno() to get the unbuffered
    // low-level file handle associated with 'fd' for use with fstat().

    if (fstat(fileno(fd), &fileinfo) != -1) {
      printf("inode for file:      %lu\n", (unsigned long)fileinfo.st_ino);
      printf("# of blocks in file: %lu\n", (unsigned long)fileinfo.st_blocks);
      printf("file size in bytes:  %lu\n", (unsigned long)fileinfo.st_size);
    }
    else {
      perror("fstat() call failed");
    }

    // close the file
    fclose(fd);
  }
  else {
    perror("fopen() failed: ");
  }
  
  return 0;
}

