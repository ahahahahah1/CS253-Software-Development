#!/bin/bash/

<< description
    The command to run this file is expected in the format
    bash top_goals_scorer.sh file1.csv file2 OR ./top_goals_scorer.sh ./file1.csv ./file2
    The 2 files for I/O will be taken as input arguments in the command line
    Now this script will print the players with goals in top 10 positions
description

# Check if both input files are provided as arguments
if [ $# -ne 2 ]; then
  echo "Incorrect format."
  echo "Usage: $0 input_file output_file"
  exit 1
fi

# rm temp.csv

if [ ! -f $file ];then
	echo "The input file doesn't exist."
	exit 1
fi

sed -n 1p $1 >| $2

sed 1d $1 | sort -t, -nr -k16,16 >| temp.csv
prev_goals=$(awk -F',' 'NR==2 {print $16}' temp.csv)
count=0

awk -F',' '{
  if (prev_goals != $16) {
    count++
    prev_goals = $16
  }
  
  if (count == 11) {
    exit
  }

  print $0 >> "'"$2"'"
  
  
}' temp.csv

rm temp.csv
echo "Task completed successfully."
