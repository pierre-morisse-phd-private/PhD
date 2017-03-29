SRC=DocALgoStyle

all:
	latex $(SRC)
	latex $(SRC)
	dvips $(SRC) -o $(SRC).ps
	ps2pdf $(SRC).ps
	make clean

clean:
	rm *.toc *.aux *.log *.dvi
