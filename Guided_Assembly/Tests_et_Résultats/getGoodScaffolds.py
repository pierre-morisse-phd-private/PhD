import sys
import re
import csv

scaf = open(sys.argv[1]);
stat = open(sys.argv[2]);

s = scaf.readline();
while s != '':
	seq = scaf.readline();
	stat.readline();
	stat.readline();
	stat.readline();
	id = stat.readline();
	#print("Lala : ", id, s)
	id = int(re.findall('\d+', id)[0]);
	if id > int(sys.argv[3]):
		print(s[:-1]);
		print(seq[:-1]);
	s = scaf.readline();
