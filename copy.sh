#!/bin/bash
#
# This script copies "RC Test.txt", "RC Test_2.txt", and "RC Test_3.txt"
# into ./sorted_raw_files
# while maintaining the directory structure.
# 
# Author: Seungbum Koo
# Date: Tue. 11/14/2017
#
mkdir -p ./sorted_raw_files
find . \( -name "RC Test.txt" -o -name "RC Test_2.txt" -o -name "RC Test_3.txt" \) -print0 | while read -d $'\0' file
do
	filename=$(basename "$file")
  	# echo "$filename"
  	pathname=$(dirname "$file" | cut -c 3-)
  	# echo "$pathname"
  	mkdir -p "./sorted_raw_files/$pathname"
  	filename="./sorted_raw_files/$pathname/$filename"
  	echo "Copying $file to $filename..."
  	cp "$file" "$filename"
done