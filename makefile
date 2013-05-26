stegobmp: args.c hide.c stenography.c crypto.c
	gcc -o stegobmp args.c hide.c stenography.c crypto.c -lm -lmcrypt -largtable2

