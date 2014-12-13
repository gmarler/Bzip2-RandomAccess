use ExtUtils::MakeMaker;
$Verbose = 1;
 
WriteMakefile(
    NAME => 'Bzip2::RandomAccess::libmicrobunzip',
    OBJECT => ['micro-bunzip.o', 'bzip2-index.o' ],
    OPTIMIZE => '-g',
    SKIP => [qw(all test)],
    clean => {'FILES' => 'libmicrobunzip${LIB_EXT}'},
);
 
sub MY::top_targets {
return <<'END'
 
all :: static main-indexbz2
 
test ::
 
static :: libmicrobunzip$(LIB_EXT)

main-indexbz2: $(O_FILES)
	$(CC) -g -m64 -o main-indexbz2 $(O_FILES)
 
libmicrobunzip$(LIB_EXT): $(O_FILES)
	$(AR) cr libmicrobunzip$(LIB_EXT) $(O_FILES)
	$(RANLIB) libmicrobunzip$(LIB_EXT)


END
}
