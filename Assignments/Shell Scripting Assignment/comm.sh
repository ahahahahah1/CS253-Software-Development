#!/bin/bash

<< description
    The command to run comm.sh is expected to be in the format 
    bash comm.sh file1.txt file2.txt OR ./comm.sh ./file1.txt ./file2.txt
    All the instances of file1.txt and file2.tx are referred as $1 and $2 resp.
description

i=1

if [ $# -ne 2 ]
then
	echo "Incorrect format."
    echo "Usage: $0 file1 file2"
    exit 1
fi

# read -p "Enter the name of the first file: " file1

if [ ! -f $1 ]
then
    echo "File $1 does not exist"
    exit 1
fi

# read -p "Enter the name of the second file: " file2

if [ ! -f $2 ]
then
    echo "File $2 does not exist"
    exit 1
fi

while read line_file1
do
	if [[ -z $line_file1 ]]; then #empty line
        i=$((i+1))
		continue
	fi
	
	j=1
	while read line_file2
	do
		if [[ "$line_file1" == "$line_file2" ]]
		then
			echo "Line No. $i from the first file matches with Line No. $j from the second file. The line is"
			echo $line_file1
			echo ""
		fi
		j=$((j+1))
	done < $2
	i=$((i+1))
done < $1
