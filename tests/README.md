tests directory consists of test for different functions and sections of code in
new_terminal project.

Tests are named after function they test, e.g. if input_commands() test for this function is
input_commands.sh. Most of tests echo strigs to the called prog, so to use test enter these commands:

./test_name.sh
./test_name.sh | ../build/term

The first command echos input strings, the second one runs test
