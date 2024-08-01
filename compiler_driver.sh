#!/bin/bash

input_file="$1"
output_file="${input_file%.c}.i"

gcc -E -P "$input_file" -o "$output_file"
