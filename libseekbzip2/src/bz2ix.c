#include <bz2ix.h>


/* open_bz2_file(const char *path)
 * Input:  Path to .bz2 file
 * Output: file descriptor for .bz2 file
 */

/*
 * Function implementations
 */

/* We use a file descriptor here, since the bzip2 routines expect to read the
 * compressed data stream from one */
int open_bz2_file(const char *path)
{
  int fd;
  
  fd = open(path, O_RDONLY);
  check(fd > 2, "Unable to open bz2 file");

  return(fd);

error:
  return(-1);
}

/* Given the bzfile_path, generate the bzxix file path
 */
char *bz2ix_file_name(const char *bzfile_path)
{
  char    buf[PATH_MAX];
  char   *bz2ix_path = NULL;
  size_t  bz_path_len, bz_path_strip_len;

  check( realpath(bzfile_path, buf) != NULL, "Unable to resolve bzfile path" );

  bz2ix_path = strndup( buf, PATH_MAX - 1 );

  bz_path_len = strnlen( bz2ix_path, PATH_MAX - 1 );
  bz_path_strip_len = bz_path_len - 4;
  if (strncmp(&bz2ix_path[bz_path_strip_len], ".bz2", PATH_MAX - 1) == 0) {
    printf("Found proper suffix\n");
    bz2ix_path[bz_path_strip_len] = '\0';
  } else {
    printf("Did NOT find proper suffix!\n");
    printf("%s\n",&bz2ix_path[bz_path_strip_len]);
    goto error;
  }

  /*  check( offset != NULL, "Can't find offset of .bz2 suffix" ); */

  strncat( bz2ix_path, ".bz2ix", PATH_MAX - 1 );

  printf("%s\n", buf);
  printf("%s\n", bz2ix_path);

  return bz2ix_path;

error:
  if (bz2ix_path)
    free(bz2ix_path);
  return(NULL);
}

/* Since we'll just be writing text to this file, we can use buffered I/O */
FILE * open_bz2ix_file(const char *path, const char *fopen_mode)
{
  FILE * bz2ix_file;
  int    fd;

  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  fd = open(path, O_WRONLY | O_CREAT | O_EXCL, mode);
  /* If file doesn't exist */
  if (fd < 0) {
    fd = open(path, O_WRONLY | O_CREAT, mode);
    check( fd >= 0, "Unable to create new bz2ix file" );
    close(fd);
  }

  bz2ix_file  = fopen(path, fopen_mode);

  check( bz2ix_file != NULL, "Unable to open bz2ix file" );

  return(bz2ix_file);

error:
  return(NULL);
}

int generate_bz2ix_file(char *bz2_file_path)
{
  int   bz2_file_fd,
        rc,
        status;
  char *bz2ix_file_path;
  FILE *bz2ix_file;
  bunzip_data *bd;
  unsigned long long position;
  int gotcount;
  int totalcount;
  char buffer[BUF_SIZE];

  debug("Opening fd for bz2 file: %s", bz2_file_path);
  bz2_file_fd = open_bz2_file(bz2_file_path);

  check( bz2_file_fd > 2, "Appropriate fd for bz2 file");

  debug("Looking for bz2ix file for: %s", bz2_file_path);

  bz2ix_file_path = bz2ix_file_name(bz2_file_path);
  check( bz2ix_file_path != NULL, "Unable to determine bz2ix path" );
  debug("Opening bz2ix file: %s", bz2ix_file_path );

  bz2ix_file      = open_bz2ix_file(bz2ix_file_path, "w+");
  check( bz2ix_file != NULL, "Unable to open bz2ix file" );

  /* Attempt to open the bzip2 file, if successfull this consumes the
   * entire header and moves us to the start of the first block.
   */
  if ( ! ( status = start_bunzip( &bd, bz2_file_fd, 0, 0 ) ) ) {
    for ( ; ; )
    {
      /* Determine position */
      position = bd->position;
      position = position - bd->inbufCount + bd->inbufPos;
      position = position * 8 - bd->inbufBitCount;

      /* Read one block */
      status = get_next_block( bd );

      /* Reset the total size counter for each block */
      totalcount = 0;

      /* Non-zero return value indicates an error, break out */
      if ( status ) break;

      /* This is really the only other thing init_block does, hrmm */
      bd->writeCRC = 0xffffffffUL;

      /* Decompress the block and throw away, but keep track of the
         total size of the decompressed data */
      for ( ; ; )
      {
        gotcount = read_bunzip( bd, buffer, BUF_SIZE );
        if ( gotcount < 0 )
        {
          status = gotcount;
          goto bzip_table_finish;
        }
        else if ( gotcount == 0 )
        {
          break;
        }
        else
        {
          totalcount += gotcount;
        }
      }
      /* Print the position of the first bit in the block header */
      fprintf( bz2ix_file, "%llu\t%d\n", position, totalcount );
      fprintf( stdout, "%llu\t%d\n", position, totalcount );
    }
  }

  debug("start_bunzip returns: %d", status);

bzip_table_finish:

  /* If we reached the last block, do a CRC check */
  if ( status == RETVAL_LAST_BLOCK && bd->headerCRC == bd->totalCRC )
  {
    status = RETVAL_OK;
  }

  /* Free decompression buffer and bzip_data */
  if ( bd->dbuf ) free( bd->dbuf );
  free( bd );

  /* Print error if required */
  if ( status )
  {
    fprintf( stderr, "\n%s\n", bunzip_errors[-status] );
  }


  fclose(bz2ix_file);

  free(bz2ix_file_path);

  return(status);

error:
  free(bz2ix_file_path);
  return(-1);
}

bz2ix_t *load_bz2ix(const char *bz2_file_path)
{
  char    *bz2ix_file_path;
  char    line_buffer[1024];
  FILE    *bz2ix_file;
  bz2ix_t *bzip2_index;
  unsigned long long position;
  int totalcount;

  debug("Looking for bz2ix file for: %s", bz2_file_path);

  bz2ix_file_path = bz2ix_file_name(bz2_file_path);
  check( bz2ix_file_path != NULL, "Unable to determine bz2ix path" );
  debug("Opening bz2ix file for loading: %s", bz2ix_file_path );

  bz2ix_file      = open_bz2ix_file(bz2ix_file_path, "r");
  check( bz2ix_file != NULL, "Unable to open bz2ix file" );

  /* Read in the contents of the index file into the data structure */
  bzip2_index = calloc(1, sizeof(bz2ix_t));
  check_mem(bzip2_index);

  if (fgets(line_buffer, 1024, bz2ix_file) == NULL) {
    perror("ERROR in fgets");
  }

  printf("LINE: %s\n",line_buffer);

  sscanf(line_buffer, "%llu %d", &position, &totalcount);
  printf("POSITION: %llu,  TOTAL COUNT: %d\n",position,totalcount);
/*
  while (fscanf(bz2ix_file,"%llu %d", &position, &totalcount) != EOF) {
    printf("POSITION: %llu,  TOTAL COUNT: %d\n",position,totalcount);
  }
 */

error:
  free(bz2ix_file_path);
  return(NULL);
}

