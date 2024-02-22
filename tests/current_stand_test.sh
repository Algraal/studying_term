#!/bin/sh
# This script runs provided test for both real terminal and
# my terminal, cleaning created files. Shows differenced of files.
# Test should be located in the same directory and my term at 
# ../build/term

# Access the command-line arguments
first_argument="$1"

grep '^\(echo "Test \|#!/bin/sh\)' $first_argument > console_test.sh
chmod +x console_test.sh
# Files should be created before executing test to keep directory tree up-to-date
touch real_term_output.txt
touch my_term_output.txt

./console_test.sh > real_term_output.txt

./$first_argument | ../build/term > my_term_output.txt
echo "Differences in execution of my real terminal and my terminal are listed below"
diff real_term_output.txt my_term_output.txt
echo "Deleting output files"
rm real_term_output.txt my_term_output.txt
rm console_test.sh
