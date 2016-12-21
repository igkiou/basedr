#!/bin/bash

g++ -DUSE_THREADED -DUSE_DOUBLE_PRECISION -DUSE_NO_OPENEXR -DUSE_PARALLEL_CONCAT -Wall -Wextra -g -W -pedantic -march=native -O3 -ffast-math -fopenmp -pthread -I /usr/include/boost/ -I ../include/ test_bsdf.cpp ../src/bsdf.cpp  ../src/sampler.cpp -o test_bsdf

#g++ -DUSE_THREADED -DUSE_DOUBLE_PRECISION -DUSE_NO_OPENEXR -DUSE_PARALLEL_CONCAT -Wall -Wextra -g -W -pedantic -march=native -O3 -ffast-math -fopenmp -pthread -I /usr/include/boost/ -I ../include/ test1.cpp ../src/image.cpp ../src/bsdf.cpp  ../src/sampler.cpp -o test1

g++ -DUSE_THREADED -DUSE_DOUBLE_PRECISION -DUSE_NO_OPENEXR -DUSE_PARALLEL_CONCAT -Wall -Wextra -g -W -pedantic -march=native -O3 -ffast-math -fopenmp -pthread -I /usr/include/boost/ -I ../include/ test2.cpp ../src/image.cpp ../src/bsdf.cpp  ../src/sampler.cpp -o test2
