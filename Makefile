mycc: main.o cc_tkn.o cc_pars.o cc_print.o cc_eval.o
	  gcc -o mycc main.o cc_tkn.o cc_pars.o cc_print.o cc_eval.o

main.o: main.c
		gcc -c main.c
cc_tkn.o: cc_tkn.c
		gcc -c cc_tkn.c
cc_pars.o: cc_pars.c
		gcc -c cc_pars.c
cc_print.o: cc_print.c
		gcc -c cc_print.c
cc_eval.o: cc_eval.c
		gcc -c cc_eval.c