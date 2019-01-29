#
#  Makefile for compile isamon project
#
#  created by: Vladimir Jerabek (xjerab21@stud.fit.vut.cz)
#  This file is part of the ISA project 'isamon' -> monitoring of network
#

NAME=isamon
OBJFILES=IsamonArgs.o IsamonARP.o \
IsamonICMP.o IsamonInterfaces.o \
IsamonInterfc.o IsamonIP.o \
IsamonLiveVect.o IsamonMAC.o \
IsamonNetAddr.o IsamonTCP.o  \
IsamonHost.o IsamonLiveScan.o \
IsamonPcap.o IsamonTCPscan.o \
IsamonUDP.o IsamonUDPscan.o \
main.o


CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic -g -lpcap -lpthread
#-lm

# univerzalni pravidlo pro generovani vsech objektovych souboru
%.o : %.c
	$(CX) $(CXXFLAGS) -c $<

# Startovaci pravidlo
all: $(NAME)

# Slinkovani vsech objektovych souboru do jednoho spustitelneho programu .
$(NAME): $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@


# Pravidlo pro vycisteni adresare, kde jsou zdrojove kody
clean:
	rm -f $(OBJFILES) $(NAME)

run:
	./$(NAME) -n 192.168.1.0/28
