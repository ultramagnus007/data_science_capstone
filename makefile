IDIR =./
CC=g++
CFLAGS="-I$(IDIR)" "-std=c++11"

ODIR=./obj
LDIR =./

_DEPS = TST.cpp Process.cpp Model.cpp Main.cpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = TST.o Process.o Model.o Main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Main: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
