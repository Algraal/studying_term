#!/bin/sh
# input_to_commands(), main.cpp

while read a; do
       echo $a  
done <<END
    echo "Test 1:" ; ls -lr
    echo "Test 2:"; mkdir new_dir_test ; ls -la && rmdir new_dir_test
    echo "Test 3:"; echo hello && touch new.txt ; ls -l && rm new.txt ; echo by ; ls -l
    echo "Test 4:"; ls -l ; mkdir new_test_dir ; ls -l ; cd new_test_dir ; pwd; ls -l
    echo "Test 5:"; cd ; ls -la;
    exit
END
