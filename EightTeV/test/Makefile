CC = g++
CFLAGS = -Wall -c -g

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I$(ROOTSYS)/include  -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src -I$(CMSSW_BASE)/src/UserCode/pandolf/CommonTools -I$(CMSSW_BASE)/src/UserCode/pandolf/

ROOTSYS  ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs` 

EXTRALIBS  :=  -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -lHtml -lMathCore -lGenVector -lMinuit -lEG -lRooFitCore -lRooFit `root-config --libs` -lTMVA


provaSyst: provaSyst.cpp QGSyst.o
	$(CC) -Wall $(INCLUDES) -o provaSyst provaSyst.cpp QGSyst.o $(ROOTFLAG) $(EXTRALIBS)


QGSyst.o: $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGSyst.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGSyst.cc $(ROOTFLAG)


clean:
	rm *.o