use strict;
use warnings;

use Test::More;

use Bzip2::RandomAccess;

my $bz2ra = Bzip2::RandomAccess->new();

isa_ok( $bz2ra, 'Bzip2::RandomAccess' );
can_ok( $bz2ra, 'version'             );

is ($bz2ra->version, '0.1.0', 'bz2ix library is version 0.1.0' );

done_testing();
