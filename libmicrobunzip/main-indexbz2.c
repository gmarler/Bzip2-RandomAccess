#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "micro-bunzip.h"
#include "bzip2-index.h"

int main( int argc, char *argv[] )
{
  bunzip_data *bd;

  bzip2_index_build(bd);
}
