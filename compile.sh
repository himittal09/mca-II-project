#!/bin/sh

if [ -d "Build" ]; then
	echo "Build directory already exists!"
	exit
fi

echo "Please wait while build process finishes..."

mkdir Build

for f in $(find . -maxdepth 5 -type f -name '*.cpp')
do
	fuck=${f##*/}
	fuck=${fuck%.cpp}
	g++ -std=c++17 -c $f -o ./Build/$fuck.o
done

echo "Compilation finished, starting to link compiled files"

# notice the two options needed to link with postgreSQL C++ library
g++ ./Build/*.o -lpqxx -lpq -o ProductivityTracker.out

rm -r Build

echo "Build Finished..."