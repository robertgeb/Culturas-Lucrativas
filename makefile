all: culturasLucrativas

culturasLucrativas: culturasLucrativas.o
		gcc -o culturasLucrativas culturasLucrativas.o -lm -ansi -Wall -g

culturasLucrativas.o: culturasLucrativas.c
		gcc -c culturasLucrativas.c -g

clean:
		rm culturasLucrativas.o culturasLucrativas
