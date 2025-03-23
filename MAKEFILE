CC = g++
CFLAGS = -Wall -std=c++11 -lssl -lcrypto

all: secure_file_management

secure_file_management: main.o authentication.o encryption.o file_operations.o
	$(CC) main.o authentication.o encryption.o file_operations.o -o secure_file_management $(CFLAGS)

main.o: main.cpp authentication.h encryption.h file_operations.h
	$(CC) -c main.cpp

authentication.o: authentication.cpp authentication.h
	$(CC) -c authentication.cpp

encryption.o: encryption.cpp encryption.h
	$(CC) -c encryption.cpp $(CFLAGS)

file_operations.o: file_operations.cpp file_operations.h
	$(CC) -c file_operations.cpp

clean:
	rm -f *.o secure_file_management
