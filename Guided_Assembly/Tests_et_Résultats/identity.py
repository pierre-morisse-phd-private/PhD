import sys
import re
import csv

def compute_identity(f_name):
	f = open(f_name);
	f.readline();
	f.readline();
	line = f.readline();
	while line != '':
		t = line.split("\t");
		if t[1] != "256" and t[1] != "2048" and t[1] != "272" and t[1] != "2064":
			pos = t[3];
			l = len(t[9]);
			q = t[12];
			print (t[0]);
			print(pos);
			print("Longueur : ", l);
			nbs = [int(i) for i in (re.findall('\d+', q))]
			print("Identité : ", sum(nbs) / l * 100);
		line = f.readline();
compute_identity(sys.argv[1]);
