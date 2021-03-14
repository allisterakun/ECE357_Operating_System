ALL: spintest fifotest

spintest: spintest.c  spinlock.h spinlock.c tas64.s
	gcc -o spintest spinlock.c spintest.c tas64.s

fifotest: fifotest.o fifo.o sem.o spinlock.o tas.o
	gcc -o fifotest fifotest.c fifo.c sem.c spinlock.c tas64.S

fifotest.o:
	gcc -c fifotest.c

fifo.o:
	gcc -c fifo.c

sem.o:
	gcc -c sem.c

spinlock.o:
	gcc -c spinlock.c

tas.o:
	gcc -c tas64.S

clean:
	rm -f fifotest spintest *.o *.stackdump