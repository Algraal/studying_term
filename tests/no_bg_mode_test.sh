#!/bin/sh
# input_to_commands(), main.cpp

while read a; do
       echo $a  
done <<END
echo "Test 1:"; ls -r ; ls -a
echo "Test 2:"; mkdir new_dir_test ; ls -a && rmdir new_dir_test
echo "Test 3:"; echo hello && touch new.txt ; ls && rm new.txt ; echo by ; ls
echo "Test 4:"; mkdir new_test_dir ; ls; cd new_test_dir ;
echo "Test 5:"; pwd; cd .. ; rmdir new_test_dir
echo "Test 6:"; echo "This >text<< && that gr>eetings">test_created.txt ; cat test_created.txt ; echo "Append test " >> test_created.txt "After red test"; cat test_created.txt
echo "Test 7:"; rm test_created.txt ; ls -ra >> main_directory.txt ; cat main_directory.txt; rm main_directory.txt
exit
END
