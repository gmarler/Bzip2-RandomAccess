package Bzip2::RandomAccess;

use strict;
use warnings;
use XSLoader;

# VERSION
our $VERSION = '0.001';

# ABSTRACT: Allows random access to a bz2 compressed file

XSLoader::load('Bzip2::RandomAccess', $VERSION);

1;

__END__

=head1 NAME

Bzip2::RandomAccess - Interface to a library to allows random access to bz2
compressed archive files
