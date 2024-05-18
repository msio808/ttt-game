#!/bin/bash

# Compile the main.c file with the specified flags
gcc -o main main.c -Wall -Werror -Wextra -pedantic

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Run the compiled program
    ./main
else
    echo "Compilation failed."
fi
