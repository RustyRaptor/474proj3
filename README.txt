Angel Camacho Brandly Ross Ziad Arafat Luis Mendoza
CS474
Project 3

We have a makefile in our project that will compile and run the code. 
When you download and extract the code run these commands. 

make

make run


alternatively you can compile and run manually

gcc baboons.c -lpthread -lrt -Wall -Wextra  -o baboons
./baboons input.dat 3