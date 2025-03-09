
CFLAGS := -Wpointer-arith -Wall
# changer GFL3 en GFL2 pour version 2D
PFLAGS := $(incGFL3) -I./include
LFLAGS := $(libGFL3)

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

ALL = gfl3_squelette gfl3_REFERENCE gfl3_demo_scene gfl3_distdroites

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
