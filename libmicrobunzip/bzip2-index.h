#ifndef __BZIP2_INDEX_H__
#define __BZIP2_INDEX_H__

#include <limits.h>

#define BUF_SIZE 8192

typedef struct {
 uint64_t bz2_offset_bits;         /* offset in bits from beginning of bz2 file */
 off_t    block_size;              /* Uncompressed size of data in block */
} bz2_index_entry;

typedef struct {
  char            fname[PATH_MAX]; /* Index filename */
  int             fd;              /* File descriptor for bz2 file */
  bz2_index_entry *index;          /* The actual mmap()'ed index file */
  int             entries;         /* Number of entries in the index */
  off_t           bz2_size;        /* Size of compressed file */
                                   /* Just grab size of file from stat() */
  off_t           unc_size;        /* Size of uncompressed file */
                                   /* sum of block_size across all index entries */
} bz2_index;



int bzip2_index_build( int bz2_fd, bunzip_data *bd, bz2_index *index );

#endif
