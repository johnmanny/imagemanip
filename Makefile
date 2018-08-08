# ---original makefile given from assignment
# ---modifications are the addition of the targets
#	all, png, try, orgDirectories, clean, and prefix specifications for directories

SRC=main.C image.C sink.C source.C PNMreader.C PNMwriter.C filter.C logging.C
OBJ=$(SRC:.C=.o)
OBJBIN=$(addprefix bin/, $(OBJ))

prog: orgDirectories $(OBJ)
	g++ -g -I./include $(OBJBIN) -o main

.C.o: $< $@
	g++ -g -c -I./include $< -o $(addprefix bin/, $@)

all: prog try png

png:
	pnmtopng < outimage.pnm > outimage.png
try:
	./main inimage.pnm outimage.pnm

orgDirectories:
	mkdir include
	mkdir bin
	mv image.h include/image.h
	mv sink.h include/sink.h
	mv source.h include/source.h
	mv PNMreader.h include/PNMreader.h
	mv PNMwriter.h include/PNMwriter.h
	mv filter.h include/filter.h
	mv logging.h include/logging.h

clean:
	mv include/image.h image.h
	mv include/sink.h sink.h
	mv include/source.h source.h
	mv include/PNMreader.h PNMreader.h
	mv include/PNMwriter.h PNMwriter.h
	mv include/filter.h filter.h
	mv include/logging.h logging.h
	rm -r include
	rm -r bin
	rm main outimage.pnm outimage.png logger.txt my_checksum
