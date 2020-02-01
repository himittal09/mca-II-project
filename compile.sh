if [ -d "Build" ]; then
	echo "Build directory already exists!"
	exit
fi

echo "Please wait while build process finishes..."

mkdir Build

while IFS= read -r -d '' -u 9
do
	f=$(basename $REPLY)
	g++ -std=c++14 -c $REPLY -o ./Build/${f%.cpp}.o
done 9< <( find . -type f -name '*.cpp' -print0 )

g++ ./Build/*.o -o ProductivityTracker.exe

rm -r Build

echo "Build Finished..."