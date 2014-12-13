#include <stdio.h>
#include <stdlib.h>

#include "micro-bunzip.h"
#include "bzip2-index.h"

/*
bzip2_index_available();

bzip2_index_cleanup()
*/

int bzip2_index_build( int bz2_fd, bunzip_data *bd, bz2_index *index )
{
  int      status;
  uint64_t position;

  /* Attempt to open the bzip2 file, if successfull this consumes the
   * entire header and moves us to the start of the first block.
   */
  if ( ! ( status = start_bunzip( &bd, bz2_fd, 0, 0 ) ) )
  {
    for ( ; ; )
    {
      /* Determine position */
      position = bd->position;
      position = position - bd->inbufCount + bd->inbufPos;
      position = position * 8 - bd->inbufBitCount;

      /* Read one block */
      status = get_next_block( bd );

      /* Non-zero return value indicates an error, break out */
      if ( status ) break;

      /* Print the position of the first bit in the block header */
      fprintf( stdout, "%llu\n", position );
    }
  } else {
    fprintf( stdout, "%s\n", "Unable to open bzip2 archive" );
  }
  return status;
}


