#ifndef __MICRO_BUNZIP_H__
#define __MICRO_BUNZIP_H__

/* ---- Duplicated from micro-bzip.c -------------------------------------- */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/* Constants for huffman coding */
#define MAX_GROUPS   6
#define GROUP_SIZE     50  /* 64 would have been more efficient */
#define MAX_HUFCODE_BITS  20  /* Longest huffman code allowed */
#define MAX_SYMBOLS   258  /* 256 literals + RUNA + RUNB */
#define SYMBOL_RUNA   0
#define SYMBOL_RUNB   1

/* Status return values */
#define RETVAL_OK      0
#define RETVAL_LAST_BLOCK    (-1)
#define RETVAL_NOT_BZIP_DATA   (-2)
#define RETVAL_UNEXPECTED_INPUT_EOF  (-3)
#define RETVAL_UNEXPECTED_OUTPUT_EOF (-4)
#define RETVAL_DATA_ERROR    (-5)
#define RETVAL_OUT_OF_MEMORY   (-6)
#define RETVAL_OBSOLETE_INPUT   (-7)

/* Other housekeeping constants */
#define IOBUF_SIZE   4096

/* This is what we know about each huffman coding group */
struct group_data
{
    /* We have an extra slot at the end of limit[] for a sentinal value. */
    int limit[MAX_HUFCODE_BITS+1], base[MAX_HUFCODE_BITS], permute[MAX_SYMBOLS];
    int minLen, maxLen;
};

/* Structure holding all the housekeeping data, including IO buffers and
   memory that persists between calls to bunzip */
typedef struct
{
    /* State for interrupting output loop */
    int writeCopies, writePos, writeRunCountdown, writeCount, writeCurrent;
    /* I/O tracking data (file handles, buffers, positions, etc.) */
    int in_fd, out_fd, inbufCount, inbufPos /*,outbufPos*/;

    // james@jamestaylor.org: track relative position in input so we don't need tell
    /* TODO:
     * gmarler@gmarler.com: May need to reinstate this to get tell() back, as we
     *                      might want it in our Perl XS module
     */
    off_t position;
    unsigned char *inbuf /*,*outbuf*/;
    unsigned int inbufBitCount, inbufBits;

    /* The CRC values stored in the block header and calculated from the data */
    unsigned int crc32Table[256], headerCRC, totalCRC, writeCRC;
    /* Intermediate buffer and its size (in bytes) */
    unsigned int *dbuf, dbufSize;
    /* These things are a bit too big to go on the stack */
    unsigned char selectors[32768];   /* nSelectors=15 bits */
    struct group_data groups[MAX_GROUPS]; /* huffman coding tables */
    /* For I/O error handling */
    jmp_buf jmpbuf;
}
bunzip_data;

static char * const bunzip_errors[] =
    {
        NULL, "Bad file checksum", "Not bzip data",
        "Unexpected input EOF", "Unexpected output EOF", "Data error",
        "Out of memory", "Obsolete (pre 0.9.5) bzip format not supported."
    };

/* ---- Forward declarations for micro-bzip.c ---------------------------- */

extern int get_next_block( bunzip_data *bd );
extern int start_bunzip( bunzip_data **bdp, int in_fd, char *inbuf, int len );

#endif
