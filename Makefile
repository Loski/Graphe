graphe: main.o algorithme.o lecture.o 
	gcc -o graphe main.o algorithme.o lecture.o
main.o: main.c lecture.h algorithme.h
	gcc -c main.c -o main.o -W -Wall -Wextra
algorithme.o: algorithme.c algorithme.h
	gcc -c algorithme.c -o algorithme.o -W -Wall -Wextra
lecture.o: lecture.c lecture.h algorithme.h
	gcc -c lecture.c -o lecture.o -W -Wall -Wextra
	
