use ExtUtils::MakeMaker;
$Verbose = 1;
 
WriteMakefile(
    NAME => 'Bzip2::RandomAccess::libmicrobunzip',
    SKIP => [qw(all test)],
    clean => {'FILES' => 'libmicrobunzip${LIB_EXT}'},
);
 
sub MY::top_targets {
return <<'END'
 
all :: static
 
test ::
 
static :: libmicrobunzip$(LIB_EXT)
 
libmicrobunzip$(LIB_EXT): $(O_FILES)
	$(AR) cr libmicrobunzip$(LIB_EXT) $(O_FILES)
	$(RANLIB) libmicrobunzip$(LIB_EXT)
 
END
}
