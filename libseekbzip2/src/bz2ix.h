#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include "micro-bunzip.h"
#include "idx_list.h"
#include "dbg.h"

#define BUF_SIZE 8192

/* bz2 index file data structure */
typedef struct bz2ix {
/*
  struct bz2ix_node *first;
  struct bz2ix_node *last;
 */
  List              *nodes;
  size_t             total_uncompressed_size; /* Total uncompressed size of entire file */
} bz2ix_t;


/* 
 * bz2 index data structure entry
 */
typedef struct bz2ix_node {
  struct bz2ix_node *previous;
  struct bz2ix_node *next;
  size_t  offset_bits;               /*  Offset in bits from start of bz2 stream */
  size_t  uncompressed_offset_bytes; /*  Offset in bytes from start of uncompressed file */
  size_t  compressed_size;           /*  RESERVED for possible later use */
  size_t  uncompressed_size;         /*  Uncompressed size of this block in bz2 stream */
  char   *uncompressed_data;         /*  If defined, the uncompressed data */
} bz2ix_node_t;


/*
 * Forward declarations
 */
/*
open_bz2_file()
open_bz2ix_file()
create_bz2ix_file()
*/

int      open_bz2_file(const char *path);
char    *bz2ix_file_name(const char *bzfile_path);
FILE    *open_bz2ix_file(const char *path, const char *fopen_mode);
int      generate_bz2ix_file(char *bz2_file_path);

bz2ix_t *load_bz2ix(const char *bz2_file_path);
size_t   total_size_uncompressed(bz2ix_t *index);
char    *seek_byte(bz2ix_t *index, size_t offset);
char    *read_buf(bz2ix_t *index, size_t offset, char *buf, size_t length);


