#space makefile

#paths
LPATH = lib
IPATH = include

#options
MOPT = --no-print-directory -j6


#flags
LDFLAGS = -std=c++17 -Wall -Wextra -Werror -O3\
-Wno-error=unused-but-set-variable -Wno-error=unused-variable\
-Wno-error=unused-parameter $(addprefix -I,$(IPATH)) \
-Wno-error=maybe-uninitialized \
-march=native -larmadillo -lutil -lboost_iostreams -lboost_system\
-lboost_filesystem -fopenmp #-fprofile-use#-fprofile-use

#### Hier die Compiler flags ###################################################
CXXFLAGS = -g -std=c++17 -Wall -Wextra -Werror -O3\
-Wno-error=unused-but-set-variable -Wno-error=unused-variable\
-Wno-error=unused-parameter $(addprefix -I,$(IPATH)) \
-Wno-error=maybe-uninitialized \
-march=native -larmadillo -lboost_iostreams -lboost_system\
-lboost_filesystem -fopenmp #-fprofile-use

RFLAGS = -F pe-i386

#programs
#### Hier deinen Compiler eintragen ############################################
CC = gcc
CXX = g++

SP = strip
RC = windres

#directories
#### Hier landen deine .o dateien ##############################################
BUILDDIR = build
#### Pack in diesen Ordner die cpp dateien #####################################
SRCDIR = src
#### Und hier die Header #######################################################
TARDIR = bin


#### Hier gibst du einfach den namen der fertigen ausführbaren an###############
TARNAME = xps_ex


ifneq (,$(findstring Windows,$(PATH)))
	LOPT += -Wl,-subsystem,windows
	CXX := $(addsuffix .exe,$(CXX)) CC := $(addsuffix .exe,$(CC))
	TARNAME := $(addsuffix .exe,$(TARNAME))
endif

#vars
TARGET = $(TARDIR)/$(TARNAME)

#modules
CXXMODULES = $(notdir $(basename $(shell find $(SRCDIR) -type f -name '*.cpp')))
CMODULES = $(notdir $(basename $(shell find $(SRCDIR) -type f -name '*.c')))

#objects
CXXOBJECTS = $(addsuffix .o,$(addprefix $(BUILDDIR)/, $(CXXMODULES)))
COBJECTS = $(addsuffix .o,$(addprefix $(BUILDDIR)/, $(CMODULES)))

#TARGETS
#main
$(TARGET) : $(CXXOBJECTS) $(COBJECTS)
	@mkdir -p $(TARDIR)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LOPT)

#cxx prerequisites
define CXXPREREQUISITES
$(subst \ ,,$(shell $(CXX) $(CXXFLAGS) -MM -MT $(BUILDDIR)/$(o).o $(SRCDIR)/$(o).cpp))

endef

#c prerequisites
define CPREREQUISITES
$(subst \ ,,$(shell $(CC) $(CFLAGS) -MM -MT $(BUILDDIR)/$(o).o $(SRCDIR)/$(o).c))

endef

#evaluation
$(eval $(foreach o, $(CXXMODULES),$(CXXPREREQUISITES)))
$(eval $(foreach o, $(CMODULES),$(CPREREQUISITES)))

#this is the recipe for all cpp files
$(CXXOBJECTS) :

	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#this is the recipe for all c files
$(COBJECTS) :
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#COMMANDS
clean:
	$(RM) -r $(BUILDDIR)
	$(RM) $(TARGET)

# debug:
# 	@make CFLAGS="$(CFLAGS) -g" $(MOPT)
	
relink:
	@$(RM) $(TARGET)
	@make $(MOPT)
	
run:
	./$(TARGET)
	
multi:
	@make -j8
#pre:
#	make new CFLAGS="$(CFLAGS) -E -o main.pp.txt" new CFLAGS="$(CFLAGS) -E -o main.pp.txt"
