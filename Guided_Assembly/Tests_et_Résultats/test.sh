#!/bin/bash

res="scaffolds"

echo ""
echo "$res"
./generateGraphFile.sh "$1" "$2" "$3" 0 10000 0 1 "$4" > log
wc -l "graph_file_$1"
rm mmaps
./getMultimaps "filtered_alignments_$1" mmaps
cut -f 2 mmaps | sort -u > pairable
wc -l pairable
./getLinkedContigs.sh ../../sequences/reads/Ecoli/abyss.fa pairable > log 2> log
#awk '{/>/&&++a||b+=length()}END{print b/a}' ../../sequences/reads/Ecoli/abyss.fa_linked
#awk '{/>/&&++a||b+=length()}END{print b/a}' ../../sequences/reads/Ecoli/abyss.fa_unlinked
#bwa mem ../../../../../sequences/references/ADP1G/CR543861.fasta linked_shortreads.contigs.fa > sh.sam
#python3.4 /home/reads/bin/identity.py sh.sam > sh.id
#echo "STATS CONTIGS PRE TRAITEMENT"
#python3.4 ../../stats.py sh.id shortreads.contigs.fa
#samtools view -Sb sh.sam > sh.bam
#samtools sort sh.bam qry
#samtools depth -q0 -Q0 qry.bam > qry.depth
#wc -l qry.depth
./a.out "graph_file_$1" "$res.fa" ../../sequences/references/EcoliM/MAP006.fasta ../../sequences/reads/Ecoli/abyss.fa ../../sequences/reads/Ecoli/abyss.sam
wc -l "graph_file_$1"
bwa mem ../../sequences/references/EcoliM/EcoliK12.fasta "$res.fa" > "$res.sam"
python3.5 identity.py "$res.sam" > "$res.id"
echo ""
echo "STATS CONTIGS POST TRAITEMENT"
python3.5 stats.py "$res.id" "$res.fa"
samtools view -Sb "$res.sam" > "$res.bam"
samtools sort "$res.bam" qry
samtools depth -q0 -Q0 qry.bam > qry.depth
wc -l qry.depth
