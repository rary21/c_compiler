mycc: main.o cc_tkn.o
	  gcc -o mycc main.o cc_tkn.o

main.o: main.c
		gcc -c main.c
cc_tkn.o: cc_tkn.c
		gcc -c cc_tkn.c