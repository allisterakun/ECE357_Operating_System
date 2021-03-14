#include "spinlock.h"

int main(int argc, char * argv[]) {
    if(argc!=3) {
        fprintf(stderr,"Incorrect Input. Try %s [num of process] [num of iteration]\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    long long unsigned int nProcess = atoll(argv[1]);
    long long unsigned int nIteration = atoll(argv[2]);
    fprintf (stderr, "Number of Processes= \t%llu\n", nProcess);
    fprintf (stderr, "Number of Iterations= \t%llu\n", nIteration);

    int* mappedRegionProtected = mmap (NULL,4096,PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,0,0);
    int* mappedRegionUnprotect = mmap (NULL,4096,PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,0,0);
    if(mappedRegionProtected == MAP_FAILED || mappedRegionUnprotect == MAP_FAILED) {
        fprintf(stderr,"Error with mmap: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }

    mappedRegionProtected[0] = 0;
    mappedRegionUnprotect[0] = 0;

    spinlock *lock;
    lock = (spinlock *)(mappedRegionProtected + sizeof (spinlock));
    lock -> primitive_lock = mappedRegionProtected [1];

    pid_t pids [nProcess];

    for ( int i = 0; i < nProcess; i++ ) {
        if ((pids[i] = fork()) < 0) {
            fprintf (stderr, "Error with forking process %d: %s\n", i, strerror (errno));
            return EXIT_FAILURE;
        }
        if (pids[i] == 0) {
            for (int j = 0; j < nIteration; j++) {
                mappedRegionUnprotect[0]++;
            }

            spin_lock(&lock->primitive_lock);
            for (int k = 0; k < nIteration; k++) {
                mappedRegionProtected[0]++;
            }
            spin_unlock(&lock->primitive_lock);
            exit(0);
        }
    }

    for ( int m = 0; m < nProcess; m++ ){
        if (waitpid (pids[m], NULL, 0) < 0) {
            fprintf(stderr, "Error with waitpid: %s\n", strerror (errno));
        }
    }

    fprintf(stderr,"Expected:\t\t\t%llu\n", nProcess * nIteration);
    fprintf(stderr,"With mutex protection:\t\t%d\n", mappedRegionProtected[0]);
    fprintf(stderr,"W/o  mutex protection:\t\t%d\n", mappedRegionUnprotect[0]);
}

