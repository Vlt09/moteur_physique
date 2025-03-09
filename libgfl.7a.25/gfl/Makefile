#==================================================
# version 6e (2023/2024) -
#==================================================
VERS   := 6e
TARG   := gfl

MODULS := gfl_error.o  gfl_mtrack.o gfl_tools.o gfl_colors.o gfl_capture.o gfl_pixmap.o gfl_colsyst.o gfl_window.o 
2DSPEC := gfl_geom2d.o gfl_transfo2d.o gfl_draw2d.o
3DSPEC := gfl_geom3d.o gfl_transfo3d.o gfl_particule3d.o

SRC    := src/
INC    := include/
BIN    := bin/

PFLAGS := -I/usr/include -I./$(INC) -D_USE_GL_IDLE_
CFLAGS := -Wpointer-arith #-Wall
LFLAGS := -lm -L/usr/lib/x86_64-linux-gnu/ -lGL -lGLU -lglut

ifeq ($(DIM),3)
	PFLAGS += -D_GFL3D_
	MODULS += $(3DSPEC)
else
	PFLAGS += -D_GFL2D_
	MODULS += $(2DSPEC)
endif

ifeq ($(CPP),1)
	# clang++ n'accepte pas les réels 1.0 sous type <float> :=> IL VEUT 1.0f :=> génère une erreur
	# g++ n'aime pas non plus, mais se contente d'un warning
	CC   := g++
	STD  := -std=c++11
	cpp  :=++
else
	CC   := gcc
	STD  := -std=c17
endif

ifeq ($(GDB),1)
  CFLAGS += -g
	ext    :=.gdb
else
  CFLAGS += -O2
endif

COMPOPT = $(cpp)$(ext)

all : $(TARG)

# compilation generique d'un module %.o a partir de src/%.c
%.o : $(SRC)%.c
	@echo "module $@"
	@$(CC) -fpic $(CFLAGS) $(PFLAGS) -c $< -o $@
	@echo "------------------------"

# compilation de la lib
$(TARG) : $(MODULS)
	@echo "$(EXT)"
	@echo "assemblage bibliotheque dynamique lib$@$(DIM)d$(COMPOPT).so"
	@$(CC) -fpic -shared $^ $(LFLAGS) -o $(BIN)lib$@$(DIM)d$(COMPOPT).so
	@echo "assemblage bibliotheque d'archive lib$@$(DIM)d$(COMPOPT).a"
	@ar r $(BIN)lib$@$(DIM)d$(COMPOPT).a $^
	@echo "------------------------                                "

# les règles 'PHONY'
.PHONY : clean cleanall arch ?

# nettoyage
clean :
	rm -f *.o
cleanall :
	rm -f *.o $(BIN)lib$(TARG)*

# informations sur les paramètres de compilation
? :
	@echo "---------info compil----------"
	@echo "  compilateur : $(CC)    "
	@echo "  standard    : $(STD)   "
	@echo "  PFLAGS      : $(PFLAGS)"
	@echo "  CFLAGS      : $(CFLAGS)"
	@echo "  LFLAGS      : $(LFLAGS)"

# archivage des sources
DATE := $(shell date +%y%m%d-%H%M)
NAME := $(shell basename $(PWD))

arch : Make* include/*.h src/*.c
	@echo "archivage sources > $(NAME).$(DATE).tgz"
	@tar -zcf $(NAME).$(DATE).tgz $^

