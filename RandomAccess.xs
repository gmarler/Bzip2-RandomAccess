#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include <bz2ix.h>

/* Custom C functions go here */

MODULE = Bzip2::RandomAccess      PACKAGE = Bzip2::RandomAccess

# XS code goes here

PROTOTYPE: ENABLED
