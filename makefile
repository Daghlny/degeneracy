
GCCFLAGS = -O3 -std=c++11 #-Wall
OPENMP = -fopenmp

clean:
	rm -rf ./bin

checkbin:
	mkdir -p bin

bz: src/BZ.cpp src/BZ.hpp src/inputbuffer.cpp src/inputbuffer.hpp
	make checkbin;
	g++ $(GCCFLAGS) ./src/BZ.cpp ./src/inputbuffer.cpp -o ./bin/bz.out


normal: src/degeneracy.cpp
	make checkbin;
	g++ $(GCCFLAGS) ./src/degeneracy.cpp -o ./bin/normal.out


parallel: src/parallel_degeneracy.cpp
	make checkbin;
	g++ $(GCCFLAGS) $(OPENMP) ./src/parallel_degeneracy.cpp -o ./bin/parallel.out


