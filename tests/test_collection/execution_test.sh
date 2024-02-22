#!/bin/sh
# input_to_commands(), main.cpp

while read a; do
       echo $a  
done <<END
    echo "Test 1:"; ls -lr
    echo "Test 2:"; mkdir new_dir_test ; ls -la && rmdir new_dir_test
    echo "Test 3:"; echo hello && touch new.txt ; ls -l && rm new.txt ; echo by ; ls -l
    echo "Test 4:"; ls -l ; mkdir new_test_dir ; ls -l ; cd new_test_dir ; pwd; ls -l
    echo "Test 5:"; cd ; ls -la;
    echo "Test 6:"; xterm top &
    echo "Test 7:"; sleep 100 & xterm top
    echo "Test 8:"; echo "This >text<< && that gr>eetings">test_created.txt ; cat test_created.txt ; echo "Append test " >> test_created.txt "After red test"; cat test_created.txt
    echo "Test 9:"; rm test_created.txt ; ls -l >> main_directory.txt ; cat main_directory.txt; rm main_directory.txt
    exit
END
