use 5.008005;
use ExtUtils::MakeMaker;
WriteMakefile(
  NAME             => 'Bzip2::RandomAccess',
  VERSION_FROM     => 'lib/Bzip2/RandomAccess.pm',
  PREREQ_PM        => { 'Test::More' => 0,
                        'Test::Class::Moose' => 0,
                      },
  ABSTRACT_FROM    => 'lib/Bzip2/RandomAccess.pm',
  AUTHOR           => 'Gordon Marler <gmarler@gmarler.com>',
  LIBS             => [ '-lbz2ix' ],
  DEFINE           => '',
  INC              => '-I.',
  OBJECT           => '$(O_FILES)',
);
