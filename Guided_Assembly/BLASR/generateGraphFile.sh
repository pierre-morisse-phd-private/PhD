#!/bin/bash

tmp="tmp_"$$

echo "Filtering insignificant alignments..."
echo "$1"
./filterAlignments "$1" "$2" "$3" "$8" > $tmp
sort -k1,1 -k10,10n -k2,2 -k7,7n $tmp > "filtered_alignments_$1"
rm "$tmp"
echo "... done"
#echo "Searching long reads mapped multiple times..."
#rm "multimaps_$1"
#getMultimaps.sh "filtered_alignments_$1" "tmp_"$$
#sort -k1,1 -k7,7n -k9,9n "tmp"_$$ > "multimaps_$1"
#echo "... done"
echo "Filtering big gaps and overlaps..."
rm "graph_file_$1"
res="graph_file_$$"
./filterGaps "filtered_alignments_$1" "$4" "$5" "$6" "$7" "$res"
#sort -k1,1 -k5,5 -k3,3nr -k2,2 "$res" > "graph_file_$1"
cat "$res" > "graph_file_$1"
rm $res
echo "... done"
echo "Graph file generated: graph_file_$1"
