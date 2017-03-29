#!/bin/bash

./generateGraphFile.sh "$1" "$2" "$3" 0 1000000 -9000 1 "$4"
wc -l "graph_file_$1"
./assembleContigs "graph_file_$1" > tmp
rm orderedContigs

while read line
do
	src=$(cut -f 1 <<< "$line")
	dst=$(cut -f 2 <<< "$line")
	cov=$(cut -f 3 <<< "$line")
	id=$(cut -f 4 <<< "$line")
	./pos.sh "$src" > src_pos
	./pos.sh "$dst" > dst_pos
	echo -e "$src\t$dst\t$cov\t$id"
	while read msrc
	#while 0
	do
		while read mdst
		do
			r="+"
			if [[ msrc -gt mdst ]]
			then
				r="-"
			fi
			echo -e "$msrc\t$mdst\t$r"
		done < dst_pos
	done < src_pos
	#echo "$src"
	echo ""
	echo "$src" >> "orderedContigs"
	echo "$dst" >> "orderedContigs"
done < ""

sort -u "orderedContigs" | wc -l
