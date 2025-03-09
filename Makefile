
CFLAGS := -Wpointer-arith -Wall
# changer GFL2 en GFL3 pour version 3D
PFLAGS := $(incGFL2) -I./include
LFLAGS := $(libGFL2)

CPP = 1

# C ou C++
ifeq ($(CPP),1)
	# clang++ n'accepte pas les réels 1.0 sous type <float> :=> IL VEUT 1.0f :=> génère une erreur
	# g++ n'aime pas non plus, mais se contente d'un warning
	CC	   = g++
	STD	   = -std=c++17
	# version C++ de la libgfl
else
	CC	 = gcc
	STD	= -std=c17
endif

# optim ou debug
ifeq ($(GDB),1)
	CFLAGS += -g
	# version gdb de la libgfl
	LFLAGS +=.gdb
else
	CFLAGS += -O2
endif

EXEC := tp1

SRC := src/main.cpp src/PMat.cpp

OBJ := $(SRC:.cpp=.o)

CXXFLAGS := $(STD)

all : $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LFLAGS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(PFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY : clean cleanall ?

# informations sur les paramètres de compilation
? :
	@echo "---------compilation informations----------"
	@echo "	compilateur : $(CC)		 "
	@echo "	standard		: $(STD)	 "
	@echo "	PFLAGS			: $(PFLAGS)"
	@echo "	CFLAGS			: $(CFLAGS)"
	@echo "	LFLAGS			: $(LFLAGS)"
	
cleanall :
	@rm -f *.o $(ALL)
