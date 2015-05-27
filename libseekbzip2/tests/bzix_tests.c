#include "minunit.h"
#include <dlfcn.h>
#include <link.h>
#include "bz2ix.h"


/* typedef int (*lib_function)(const char *data); */
char *lib_file = "build/libseekbzip2.so";
void *lib = NULL;

/*
int check_function(const char *func_to_run, const char *data, int expected)
{
  lib_function func = dlsym(lib, func_to_run);
  check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());

  int rc = func(data);
  check(rc == expected, "Function %s return %d for data: %s", func_to_run, rc, data);

  return 1;
error:
  return 0;
}

char *test_dlopen(void)
{
  lib = dlopen(lib_file, RTLD_NOW);
  mu_assert(lib != NULL, "Failed to open the library to test.");

  return NULL;
}
*/

char *test_open_bz2_file()
{
  int fd = open_bz2_file("/dev/random");

  mu_assert(fd > 2, "Open a bz2 file should produce an fd > 2." );
  /*    mu_assert(check_function("uppercase", "Hello", 0), "uppercase failed.");
        mu_assert(check_function("lowercase", "Hello", 0), "lowercase failed.");
        */

  return NULL;
}

char *test_bz2ix_file_name()
{
  char *bz2ix_file_path, *junk_path;

  bz2ix_file_path = bz2ix_file_name("/tmp/testarchive.bz2");
  junk_path = bz2ix_file_name("/tmp/dumas.bz2");

  mu_assert(bz2ix_file_path != NULL, "Produced a .bz2ix filename");
  mu_assert(junk_path == NULL, "Skipped a bogus .bz2 file");

  return NULL;
}

char *test_open_bz2ix_file()
{
  char *bz2ix_file_path;
  FILE *bz2ix_file;

  bz2ix_file_path = bz2ix_file_name("/tmp/testarchive.bz2");

  mu_assert(bz2ix_file_path != NULL, "Produced a .bz2ix filename");

  /* TODO: Fix this so a .bz2ix file is created */
  /* This creates the file if it doesn't exist */
  bz2ix_file = open_bz2ix_file(bz2ix_file_path,"r");

  mu_assert(bz2ix_file != NULL, "Open a .bz2ix file");

  return NULL;
}

char *test_generate_bz2ix_file()
{
  generate_bz2ix_file("/tmp/testarchive.bz2");

  load_bz2ix("/tmp/testarchive.bz2");

  return NULL;
}


/*
char *test_failures()
{
    mu_assert(check_function("fail_on_purpose", "Hello", 1), "fail_on_purpose should fail.");

    return NULL;
}
*/

char *test_dlclose()
{
  int rc = dlclose(lib);
  mu_assert(rc == 0, "Failed to close lib.");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_open_bz2_file);
  mu_run_test(test_bz2ix_file_name);
  mu_run_test(test_open_bz2ix_file);
  mu_run_test(test_generate_bz2ix_file);

  return NULL;
}

RUN_TESTS(all_tests);


