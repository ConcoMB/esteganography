stegobmp: main.c stenography.c crypto.c
	gcc -o stegobmp main.c stenography.c crypto.c -lm -lmcrypt

