#!/bin/bash

<< description
    The command to run this file is expected in the format
    bash goals_above_30.sh file1.csv file2 OR ./goals_above_30.sh ./file1.csv ./file2
    The 2 files for I/O will be taken as input arguments in the command line
description

if [ $# -ne 2 ]; then
    echo "Incorrect format."
    echo "Usage $0 input_file output_file"
    exit 1
fi

if [ ! -f $1 ]; then
    echo "File $1 does not exist"
    exit 99
fi

age_limit=30 #The age above which the players are to be considered
goal_criteria=0 #The number of goals above which the players are to be considered

awk -v age_limit="$age_limit" -v goal_criteria="$goal_criteria" -F "\"*,\"*" '$2>age_limit && $16>goal_criteria { print }' $1 >| $2

cat $2