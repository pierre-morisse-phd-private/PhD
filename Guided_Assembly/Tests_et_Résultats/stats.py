import sys
import re
import csv

def get_template_len(id, f_name):
	f = open(f_name);
	for line in f:
#		if id in line and not ('0' <= line[len(id)+1] or line[len(id)+1] <= '9'):
		if id in line:
			s = f.readline();
			return len(s) - 1;

def compute_identity(f_name):
	f = open(f_name);
	nbReads = 0;
	nbContigs = 0;
	avLen = 0;
	avCov = 0;
	avQual = 0;
	maxLen = 0;
	maxLenQ = 0;
	goodQ = 0;
	badQ = 0;
	nbGoodCov = 0;
	nbUn = 0;
	nbGoodCovUn = 0;
	s = f.readline();
	while s != '':
		nbReads = nbReads + 1;
		tmpAvLen = 0;
		t = [];
		t.append(s);
		s = f.readline();
		t.append(s);
		s = f.readline();
		t.append(s);
		s = f.readline();
		t.append(s);
		s = f.readline();
		n = int(len(t) / 4);
		nbContigs = nbContigs + n;
		for i in range(n):
			tl = int(re.findall('\d+', t[4 * i + 2])[0]);
			tmpAvLen = tmpAvLen + tl;
			tq = int(re.findall('\d+', t[4 * i + 3])[0]);
			avQual = avQual + tq;
			if tl > maxLen and tq > 85:
                        	maxLen = tl;
                        	maxLenQ = tq;
			if tq >= 85:
				goodQ = goodQ + 1;
			if tq <= 70:
				badQ = badQ + 1;
		if n == 1:
			nbUn = nbUn + 1;
		avLen = avLen + tmpAvLen;
		#print(t[0]);
		#str = re.sub('.*_.*\n?', '', t[0]);
		str = t[0];
		#print("Je cherche : ", str);
		tlen = get_template_len(str, sys.argv[2]);
		#print("lens : ", tmpAvLen, tlen);
		avCov = avCov + (tmpAvLen / tlen * 100);
		if (tmpAvLen / tlen * 100) >= 90:
			nbGoodCov = nbGoodCov + 1;
			if n == 1:
				nbGoodCovUn = nbGoodCovUn + 1;	
	print("Nombre de reads : ", nbReads);
	print("Nombre de cotigs uniques : ", nbUn);
	print("Nombre moyen de contigs : ", nbContigs / nbReads);
	print("Cumulative size : ", avLen);
	print("Longueur moyenne d'un coting : ", avLen / nbContigs);
	print("Qualité moyenne d'un conting : ", avQual / nbContigs);
	print("Contig le plus long et qualite :", maxLen, maxLenQ);
	print("Template couvert en moyenne : ", avCov / nbReads);
	print("Nombre de templates bien couverts : ", nbGoodCov);
	print("Nombre de templates bien couverts par un unique contig : ", nbGoodCovUn);
	print("Nombre de contigs :", nbContigs);
	print("Nombre de contigs de bonne qualite (>= 85 %) :", goodQ, "(", goodQ / nbContigs * 100, "%)");
	print("Nombre de contigs de mauvaise qualite (<= 70%) :", badQ, "(", badQ / nbContigs * 100, "%)");
	print("&", nbReads, "&", nbContigs / nbReads, "&", avLen  / nbContigs, "&", avQual / nbContigs, "&", maxLen, "(", maxLenQ, "%) &", avCov / nbReads, "&", nbGoodCovUn);
compute_identity(sys.argv[1]);
