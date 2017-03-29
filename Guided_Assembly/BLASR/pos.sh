#!/bin/bash

pos=$(grep "scaffold$1|" scaffolds.sam | cut -f 4)
fla=$(grep "scaffold$1|" scaffolds.sam | cut -f 2)

echo "$pos"
echo "$fla"
