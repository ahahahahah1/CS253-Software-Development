#!/bin/bash

<< description
    The command to run this file is expected in the format
    bash players.sh file1.csv file2 OR ./players.sh ./file1.csv ./file2
    The 2 files for I/O will be taken as input arguments in the command line
    This file then sorts players first by their goals_overall and then prints
    the players in the order of their position. The output stored in file 2
    is in the format full_name, nationality, position, goals_overall ($1, $7, $12, $16 columns in the csv file) 
description

# Check if both input files are provided as arguments
if [ $# -ne 2 ]; then
        echo "Incorrect format."
        echo "Usage: $0 input_file output_file"
        exit 1
fi

# rm temp.csv

if [ ! -f $file ]; then
        echo "The input file doesn't exist."
        exit 1
fi

sed 1d $1 | sort -t, -nr -k16,16 >| "temp.csv"

goals=$(awk -F',' 'NR==1 {print $16}' temp.csv)

echo "full_name,nationality,position,goals_overall" >| "$2"

while [ $goals -gt -1 ]; do
        awk -F',' -v goals="$goals" '$16 == goals && $7 == "Goalkeeper" {print $1"," $12"," $7"," $16}' temp.csv >> "$2"
        awk -F',' -v goals="$goals" '$16 == goals && $7 == "Defender" {print $1"," $12"," $7"," $16}' temp.csv >> "$2"
        awk -F',' -v goals="$goals" '$16 == goals && $7 == "Midfielder" {print $1"," $12"," $7"," $16}' temp.csv >> "$2"
        awk -F',' -v goals="$goals" '$16 == goals && $7 == "Forward" {print $1"," $12"," $7"," $16}' temp.csv >> "$2"
        goals=$((goals-1))
done

rm temp.csv
echo "Task completed successfully."