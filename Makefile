#
#  Makefile for compile isamon project
#
#  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
#  This file is part of the ISA project 'isamon' -> monitoring of network
#

# compile source code
all:
	cd src; make; cp isamon ../isamon; cd ..;

# clean working dir
clean:
	rm -rf isamon;cd src; make clean; cd ..;
