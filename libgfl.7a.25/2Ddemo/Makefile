
CFLAGS := -Wpointer-arith -Wall
# changer GFL2 en GFL3 pour version 3D
PFLAGS := $(incGFL2) -I./include
LFLAGS := $(libGFL2)

# C ou C++
ifeq ($(CPP),1)
	# clang++ n'accepte pas les réels 1.0 sous type <float> :=> IL VEUT 1.0f :=> génère une erreur
	# g++ n'aime pas non plus, mais se contente d'un warning
	CC	   := g++
	STD	   := -std:=c++11
	# version C++ de la libgfl
	LFLAGS +=++
else
	CC	 := gcc
	STD	:= -std:=c17
endif

# optim ou debug
ifeq ($(GDB),1)
	CFLAGS += -g
	# version gdb de la libgfl
	LFLAGS +=.gdb
else
	CFLAGS += -O2
endif

SRC = src/

ALL = gfl2_cercle		  gfl2_HSVflower 	gfl2_HSVcolor gfl2_HSVmire\
			gfl2_balle		  gfl2_bubblesort gfl2_gauss    gfl2_cardioide\
			gfl2_REFERENCE  gfl2_squelette 

# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(SRC)%.c
	@echo "module $@"
	@$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@
	@echo "------------------------"

# règle générique de création de l'executable sans dépendance
% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(LFLAGS) -o $@
	@echo "------------------------"

all : $(ALL)

.PHONY : clean cleanall ?

# informations sur les paramètres de compilation
? :
	@echo "---------compilation informations----------"
	@echo "	compilateur : $(CC)		 "
	@echo "	standard		: $(STD)	 "
	@echo "	PFLAGS			: $(PFLAGS)"
	@echo "	CFLAGS			: $(CFLAGS)"
	@echo "	LFLAGS			: $(LFLAGS)"

clean :
	@rm -f *.o
	
cleanall :
	@rm -f *.o $(ALL)
