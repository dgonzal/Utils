# Short Makefile to compile "root macros"

CXX      = g++ -std=c++11
LINKER   = g++ -o
TARGET = run 

SRCDIR  = src
INCDIR  = include
OBJDIR  = obj
BINDIR  = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cxx) 
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(patsubst $(SRCDIR)%.cxx,$(OBJDIR)%.o,$(SOURCES))


scramtag = $(shell cd $$CMSSW_BASE; scram tool tag $(1) $(2))

ROOTLIBS :=  $(shell root-config --evelibs)  
ROOTFLAGS := $(shell root-config --cflags)
ROOTINC :=  -I$(call scramtag, root_interface, INCLUDE)

ROOFITLIBS :=  -L$(call scramtag, roofitcore, LIBDIR) -lRooFit -lRooFitCore -lMinuit 
ROOFITFLAGS := 
ROOFITINC :=  -I$(call scramtag, roofitcore, INCLUDE)  

USERLDFLAGS += $(ROOTLIBS) $(ROOFITLIBS)
USERCXXFLAGS += -g -Wall -O2 
USERCXXFLAGS += $(ROOTFLAGS) $(ROOFITFLAGS)
USERINCFLAGS += $(ROOTINC) $(ROOFITINC) -I$(INCDIR)


$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)	
	$(LINKER) $@ $(USERLDFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cxx
	@mkdir -p $(OBJDIR)	
	$(CXX) $< $(USERINCFLAGS) $(USERCXXFLAGS) -c -o $(OBJDIR)/$(notdir $@) 
	@echo "Compiled "$<" successfully!"


.PHONEY: clean
clean:
	@rm $(OBJDIR)/*
	@echo "Cleanup complete!"

.PHONEY: distclean
distclean: clean
	@rm $(BINDIR)/*
	@echo "Executable removed!"

