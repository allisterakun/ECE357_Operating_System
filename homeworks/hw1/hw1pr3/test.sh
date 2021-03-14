#!/bin/bash
echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output1.txt ./inputs/short.txt ./inputs/long.txt"
./Concatenate.out -o ./outputs/output1.txt ./inputs/short.txt ./inputs/long.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output2.txt ./inputs/long.txt ./inputs/short_binary.txt"
./Concatenate.out -o ./outputs/output2.txt ./inputs/long.txt ./inputs/short_binary.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output3.txt ./inputs/long.txt ./inputs/long_binary.txt"
./Concatenate.out -o ./outputs/output3.txt ./inputs/long.txt ./inputs/long_binary.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output4.txt ./inputs/short.txt ./inputs/long_binary.txt"
./Concatenate.out -o ./outputs/output4.txt ./inputs/short.txt ./inputs/long_binary.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output5.txt ./inputs/short.txt ./inputs/long.txt ./inputs/long_binary.txt ./inputs/short_binary.txt"
./Concatenate.out -o ./outputs/output5.txt ./inputs/short.txt ./inputs/long.txt ./inputs/long_binary.txt ./inputs/short_binary.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output6.txt ./inputs/long_binary.txt ./inputs/short.txt ./inputs/short.txt ./inputs/short.txt"
./Concatenate.out -o ./outputs/output6.txt ./inputs/long_binary.txt ./inputs/short.txt ./inputs/short.txt ./inputs/short.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out ./inputs/short_binary.txt ./inputs/long.txt"
./Concatenate.out ./inputs/short_binary.txt ./inputs/long.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out ./inputs/short.txt ./inputs/long.txt"
./Concatenate.out ./inputs/short.txt ./inputs/long.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output9.txt ./inputs/long.txt -"
./Concatenate.out -o ./outputs/output9.txt ./inputs/long.txt -

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output10.txt - ./inputs/long_binary.txt"
./Concatenate.out -o ./outputs/output10.txt - ./inputs/long_binary.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output11.txt - ./inputs/short.txt -"
./Concatenate.out -o ./outputs/output11.txt - ./inputs/short.txt -

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output12.txt ./inputs/short_binary.txt - - ./inputs/short.txt"
./Concatenate.out -o ./outputs/output12.txt ./inputs/short_binary.txt - - ./inputs/short.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output13.txt"
./Concatenate.out -o ./outputs/output13.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out"
./Concatenate.out

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output15.txt someRandomFilename.txt ./inputs/long.txt"
./Concatenate.out -o ./outputs/output15.txt someRandomFilename.txt ./inputs/long.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output16.txt ./inputs/short.txt someRandomFilename.txt"
./Concatenate.out -o ./outputs/output16.txt ./inputs/short.txt someRandomFilename.txt

echo "Allisters-MacBook-Pro:test allisterliu$ touch ./outputs/output17.txt ./outputs/output18.txt"
touch ./outputs/output17.txt ./outputs/output18.txt
echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output17.txt ./inputs/short.txt"
./Concatenate.out -o ./outputs/output17.txt ./inputs/short.txt

echo "Allisters-MacBook-Pro:test allisterliu$ chmod 400 ./outputs/output18.txt"
chmod 400 ./outputs/output18.txt
echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output18.txt ./inputs/long.txt"
./Concatenate.out -o ./outputs/output18.txt ./inputs/long.txt

echo "Allisters-MacBook-Pro:test allisterliu$ ./Concatenate.out -o ./outputs/output19.txt -o -o -o"
./Concatenate.out -o ./outputs/output19.txt -o -o -o

echo "ls"
ls
