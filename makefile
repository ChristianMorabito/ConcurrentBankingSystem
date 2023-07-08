all: account.o main.o dynamic_list.o test
main.o: main.c account.h
account.o: account.c account.h dynamic_list.h
dynamic_list.o: dynamic_list.h
test:
	gcc -Wall -g -o test account.o main.o dynamic_list.o -lpthread
