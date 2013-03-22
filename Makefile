# (C) 2009 DJG: University of Cambridge, SoCDAM.
# $Id: $  toyclass4: tlm/esl Makefile
#


include ../Makefile.def

CLASSIP=$(SOCDAM)/toyclasses/



CXXFLAGS=-Wall -g -Wno-deprecated -I$(SYSCDIR)/include -I$(SOCDAM)/TLM-2008-06-09/include/tlm -I$(CLASSIP)


all:
	g++ $(CXXFLAGS) tlmcore.cpp $(LDFLAGS)
	./a.out


	#echo "Please enter make target where target is one of single, dual or nonsysc"

single:rtl-onecpucore.cpp 
	g++ $(CXXFLAGS) tlm-onecpucore.cpp $(LDFLAGS)
	./a.out

dual:rtl-twocpucores.cpp 
	g++ $(CXXFLAGS) rtl-twocpucores.cpp $(LDFLAGS)
	./a.out

#eof
