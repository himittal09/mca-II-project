# if [ -d "Build" ]; then
# 	echo "Build directory already exists!"
# 	exit
# fi

echo "Please wait while build process finishes..."

# mkdir Build

while IFS= read -r -d '' -u 9
do
	f=$(basename $REPLY)
	g++ -std=c++17 -c $REPLY -o ./Build/${f%.cpp}.o
done 9< <( find . -maxdepth 5 -type f -name '*.cpp' -print0 )

# g++ -std=c++17 -c main.cpp -o ./Build/main.o

echo "Compilation finished, starting to link compiled files"

# notice the two options needed to link with postgreSQL C++ library
g++ ./Build/*.o -lpqxx -lpq -o ProductivityTracker.out

# rm -r Build

echo "Build Finished..."