#!/bin/sh
# input_to_commands(), main.cpp

ls -lr
mkdir new_dir_test ; ls -la && rmdir new_dir_test
echo hello && touch new.txt ; ls -l && rm new.txt ; echo by ; ls -l
ls -l ; mkdir new_test_dir ; ls -l ; cd new_test_dir ; pwd; ls -l
cd ; ls -la;
xterm top &
sleep 100 & xterm top
echo "This >text<< && that gr>eetings">test_created.txt ; cat test_created.txt ; echo "Append test " >> test_created.txt "After red test"; cat test_created.txt
rm test_created.txt ; ls -l >> main_directory.txt ; cat main_directory.txt; rm main_directory.txt
