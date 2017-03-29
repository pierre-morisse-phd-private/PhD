#!/bin/bash

while read line
do
	grep "$line" filtered_alignments_RRC_ADP1_Alignments > "mwe"
	echo "$line"
	python3.5 getOrientation.py "mwe"
	echo ""
done < "$1"
