#!/bin/ksh

print -- "Running unit tests:"

# Set up test compressed data archive
ARCHIVE="/tmp/testarchive.bz2"
if [[ ! -f ${ARCHIVE} ]]; then
  dd if=/dev/urandom of=${ARCHIVE%.bz2} bs=1024k count=50 conv=sync
  bzip2 -9 ${ARCHIVE%.bz2}
fi

for i in tests/*_tests
do
    if test -f $i
    then
        # if $VALGRIND ./$i 2>> tests/tests.log
        if ./$i 2>> tests/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's tests/tests.log"
            echo "------"
            tail tests/tests.log
            exit 1
        fi
    fi
done

echo ""

