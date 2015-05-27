#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

/*  #include <bz2ix.h> */

/* Custom C functions go here */

MODULE = Bzip2::RandomAccess      PACKAGE = Bzip2::RandomAccess

# XS code goes here

PROTOTYPES: ENABLED

SV *
new( const char *class )
  CODE:
    /* Create a hash */
    HV* hash = newHV();

    /* Create a reference to the hash */
    SV* const self = newRV_noinc( (SV *)hash );

    /* Bless into the proper package */
    RETVAL = sv_bless( self, gv_stashpv( class, 0 ) );
  OUTPUT: RETVAL

const char *
version(...)
  CODE:
    RETVAL = "0.1.0";
  OUTPUT: RETVAL
