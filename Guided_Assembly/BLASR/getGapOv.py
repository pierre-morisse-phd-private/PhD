import sys
import re
import csv

f = open(sys.argv[1]);
f.readline();
f.readline();
f.readline();
line = f.readline();
prev = 0;
while line != '':
	t = line.split("\t");
	tmp = [int(i) for i in (re.findall('\d+', t[0]))];
	len = tmp[1];
	pos = int(t[2]);
	print(pos - prev)
	end = pos + len - 1;
	prev = end;
	line = f.readline();
