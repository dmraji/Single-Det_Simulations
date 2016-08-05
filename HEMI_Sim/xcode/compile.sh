echo "Removing old build"
rm -rf CMake* cmake* CodedAperture*

echo "Moving CMakeCache.txt"
mv ../CMakeCache.txt ../../

echo "Building..."
cmake3 -G Xcode -DGeant4_DIR=/Users/Hellfeld/GEANT4.10.2.p1/geant4-install/lib/Geant4-10.2.1/ ../

echo "Moving CMakeCache.txt back"
mv ../../CMakeCache.txt ../
