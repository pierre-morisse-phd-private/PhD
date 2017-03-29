import sys
import re
import csv

forward = 0
reverse = 0
f = open(sys.argv[1])

line = f.readline()
while line != '':
	#t =  line.split('\t')
	t = line.split(' ')
	b = t[6]
	e = t[7]
	o = int(t[3])
	#print(o)
	if (o == 0):
		forward = forward + 1;
	else:
		reverse = reverse + 1;
	line = f.readline()

print("Forward : ", forward)
print("Reverse : ", reverse)
