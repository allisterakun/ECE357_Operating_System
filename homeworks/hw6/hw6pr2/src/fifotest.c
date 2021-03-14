

#define N_PROC 64
#define MAX 100


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include "fifo.h"

extern int my_procnum;
pid_t pidTable[N_PROC];

int main() {
	for(int i = 0; i < N_PROC; i++){
		pidTable[i] = 0;
	}

    struct fifo *f;

	f = mmap(NULL, sizeof(*f), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	if(f == MAP_FAILED) {
		fprintf(stderr, "Mmap failed: %s\n", strerror(errno));
		return -1;
	}

	fifo_init(f);
	int a;
	for(my_procnum = 0; my_procnum < N_PROC; my_procnum++) {
		a = fork();
		
		if(a == 0){
			pidTable[my_procnum] = getpid();
			if(my_procnum == 0) {
				unsigned long d;
				for(int j = 0; j < ((N_PROC - 1) * MAX); j++) {
					d = fifo_rd(f);
					fprintf(stderr, "READ %lu \n", d);
				}
				
				return 0;
			}
			else {
				
				for (int i = 0; i < MAX; i++) {
					unsigned long d;
					d = ((unsigned long)my_procnum<< 32) + i;
					fifo_wr(f, ((unsigned long)my_procnum<< 32) + i);
					fprintf(stderr, "WRITE %lu by PID %d\n", d, pidTable[my_procnum]);
				}		
				return 0;
			}
		}
		else if(a == -1){
			fprintf(stderr, "Fork failed: %s\n", strerror(errno));
			return -1;
		}
	}
	for (int i = 0; i < N_PROC; i++)
    {
        if (waitpid(pidTable[i], NULL, 0) < 0){
            fprintf(stderr, "Error: Unable to wait for child process to complete\n");
		}
    }
	printf("%d processes successfully ended!\n", N_PROC);
	return 0;
}
