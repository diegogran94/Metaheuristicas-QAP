CC=g++
CFLAGS=-I. -g -Wall
DEPS = funciones.h inout.h bus_loc.h random_ppio.h bmb.h ils.h AGG.h och.h
OBJ = funciones.o inout.o main.o bus_loc.o random_ppio.o bmb.o ils.o AGG.o och.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

.PHONY : clean

clean:
	rm -f *.o
