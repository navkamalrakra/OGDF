ReadME

The test cases must be in a text file named "data.txt" which should be in the same directory as executable. 

Test Cases :

A X Y Z
X P Q 
B Y P

A X Y Z
X P Q 
B Y P 
Z L O
P Z



Compilation : 

$ g++ -I /home/r00t3r/OGDF/include/ -L /home/r00t3r/OGDF/_release/ HW1.cpp -lOGDF -lCOIN -o HW1 -pthread

Execute :
./HW1

Assumptions:
Number of input elements should be less than 16.
 
