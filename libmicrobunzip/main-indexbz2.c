#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "micro-bunzip.h"
#include "bzip2-index.h"

int main( int argc, char *argv[] )
{
  int         bz2_fd;
  bunzip_data bd;
  bz2_index   index;
  char        *bz2_file[PATH_MAX];

  if (argc == 2) {
    strncpy(index.fname,argv[1],PATH_MAX);
    bz2_fd = open(index.fname,O_RDONLY);
    bzip2_index_build(bz2_fd,&bd,&index);
  } else {
    printf("Please specify the path to a bz2 file to examine\n");
    exit(1);
  }
}
