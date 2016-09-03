# degeneracy
A quick algorithm implementation of degeneracy computing.

use commands below to compile the sequential program:
```
g++ -std=c++11 -O3 ./src/degeneracy.cpp -o degeneracy.out
```

use command below to compile the parallel version:
```
g++ -std=c++11 -O3 -fopenmp ./src/parallel_degeneracy.cpp -o parallel.out
```

the memory space required is almost $O(m+n)$
