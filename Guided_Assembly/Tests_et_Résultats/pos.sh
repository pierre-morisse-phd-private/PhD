#!/bin/bash

pos=$(grep "_$1_" ../../sequences/reads/ADP1/withRC.sam | cut -f 4)
fla=$(grep "_$1_" ../../sequences/reads/ADP1/withRC.sam | cut -f 2)

echo "$pos"
echo "$fla"
