# Short Makefile to compile "root macros"

CXX      = g++ -std=c++11 -O3
LINKER   = g++ -O3 -o

TARGETS = pu_reweight mc_background single_errors resolution cutflow trigger triggerSel treehist rootfilecreator moneyplots matching recofit forwardjetfit wjets width_comp recoplots uncer eletriggerscale compare_fc different_recocomp wtag_comp muonscalefactors wtag_unc quickAndDirty signalReco reader #scan_forwardJet fastsim uncer matchingPlots regionsratio run

SRCDIR  = src
INCDIR  = include
OBJDIR  = obj
BINDIR  = bin
PYDIR = python


SOURCES  := $(wildcard $(SRCDIR)/*.cxx)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(patsubst $(SRCDIR)%.cxx,$(OBJDIR)%.o,$(SOURCES))
MAINSRC  := $(wildcard $(SRCDIR)/*.C) 
MAINOBJ  := $(patsubst $(SRCDIR)%.C,$(OBJDIR)%.obj,$(MAINSRC))

#TARGETS = $(patsubst %.C,%"" ,$(SRCDIR)/*.C)  #pu_reweight mc_background single_errors resolution cutflow trigger triggerSel treehist rootfilecreator moneyplots matching recofit forwardjetfit wjets width_comp recoplots uncer eletriggerscale compare_fc different_recocomp wtag_comp muonscalefactors wtag_unc quickAndDirty signalReco reader #scan_forwardJet fastsim uncer matchingPlots regionsratio run


#get staff you need from your cmssw enviroment
scramtag = $(shell cd $$CMSSW_BASE; scram tool tag $(1) $(2))

ROOTLIBS :=  $(shell root-config --evelibs)  
ROOTFLAGS := $(shell root-config --cflags)
ROOTINC :=  -I$(call scramtag, root_interface, INCLUDE)

ROOFITLIBS :=  -L$(call scramtag, roofitcore, LIBDIR) -lRooFit -lRooFitCore -lMinuit
ROOFITFLAGS := 
ROOFITINC :=  -I$(call scramtag, roofitcore, INCLUDE)  

PYTHON_CFLAGS := $(shell python2.7-config --cflags) 
PYTHONLIBS := $(shell python2.7-config --ldflags)
PYTHON_INC := -I$(shell python2.7-config --includes)
PYTHON_LIBPATH := -L$(shell python2.7-config --prefix)/lib

BOOST_INC := -I$(call scramtag,boost,INCLUDE) 
BOOST_LIB := $(call scramtag,boost,LIB) $(call scramtag,boost_filesystem,LIB) $(call scramtag,boost_regex,LIB) $(call scramtag,boost_python,LIB) boost_iostreams boost_program_options boost_timer 
BOOST_LIB := $(patsubst %,-l%,$(BOOST_LIB)) -L$(call scramtag,boost,LIBDIR) -Lboost_system

USERLDFLAGS += $(BOOST_LIB) $(PYTHON_LIBPATH) $(PYTHONLIBS) $(ROOTLIBS) $(ROOFITLIBS)  
USERCXXFLAGS += -g -Wall -O2  -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=1 
USERCXXFLAGS += $(PYTHON_CFLAGS) $(ROOTFLAGS) $(ROOFITFLAGS)
USERINCFLAGS += $(BOOST_INC) $(PYTHON_INC) $(ROOTINC) $(ROOFITINC) -I$(INCDIR)
USERLASTFLAGS := -lstdc++fs
# do the work
progs : $(TARGETS)

$(TARGETS): $(OBJECTS) $(MAINOBJ)
	@mkdir -p $(BINDIR)
	@$(LINKER) $(BINDIR)/$@ $(USERLDFLAGS) $(OBJECTS) $(OBJDIR)/$@.obj $(USERLASTFLAGS)
	@echo "Linking complete for" $@"!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cxx
	@mkdir -p $(OBJDIR)	
	@$(CXX) $< $(USERINCFLAGS) $(USERCXXFLAGS) -c -o $(OBJDIR)/$(notdir $@) 
	@echo "Compiled "$<" successfully!"

$(MAINOBJ): $(OBJDIR)/%.obj : $(SRCDIR)/%.C
	@mkdir -p $(OBJDIR)	
	@$(CXX) $< $(USERINCFLAGS) $(USERCXXFLAGS) -c -o $@ 
	@echo "Compiled "$<" successfully!"


.PHONEY: clean	
clean:	
	@rm $(OBJDIR)/*
	@echo "Cleanup complete!"

.PHONEY: distclean
distclean: clean
	@rm $(BINDIR)/*
	@echo "Executable removed!"

