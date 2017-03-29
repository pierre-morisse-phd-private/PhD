import sys
import re
import csv

f = open(sys.argv[1])

linkingLRS = 0
contigs = 0

line = f.readline()
t = line.split('\t')
while line != '':
	c1 = t[0]
	lr = t[1]
	c2 = t[4]
	n = 1
	line = f.readline();
	t = line.split('\t')
	while line != '' and t[0] == c1:
		contigs = contigs + 1
		while line != '' and t[0] == c1 and t[4] == c2:
			if t[2] != lr:
				n = n + 1
			lr = t[1]
			line = f.readline()
			t = line.split('\t')
		linkingLRS = linkingLRS + n
		if line != '':
			lr = t[1]
			c2 = t[4]
		n = 1
		if t[0] == c1:
			line = f.readline()
			t = line.split('\t')
		else:
			n = 0
	linkingLRS = linkingLRS + n
	contigs = contigs + n

print(linkingLRS / contigs)
