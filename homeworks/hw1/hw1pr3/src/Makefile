Concatenate.out: OS_hw1.o
	gcc -o Concatenate.out OS_hw1.o
	mkdir -p ./outputs

OS_hw1.o: OS_hw1.c
	gcc -c OS_hw1.c

debug:
	gcc -g -o Concatenate.out OS_hw1.c

clean:
	rm -f *.out *.o *.stackdump *~ output*.txt *.exe
	rm -f ./outputs/output*.txt

backup:
	test -d backups || mkdir backups
	cp *.c backups
	cp Makefile backups