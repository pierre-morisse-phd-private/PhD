#!/bin/bash

linked="$1_linked"
unlinked="$1_unlinked"
rm $linked $unlinked
lines=$(wc -l "$1")
nb=0
while read line
do
	((nb++))
	echo "$nb / $lines"
	req=$(echo "$line" | tail -c +2 | head -c -1)
	echo "$req"
	if grep -q $req "$2"
	then
		echo "$line" >> $linked
		read line
		echo "$line" >> $linked
	else
		echo "$line" >> $unlinked
		read line
		echo "$line" >> $unlinked
	fi
done < "$1"

