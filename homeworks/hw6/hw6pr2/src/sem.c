#include "sem.h"

void handler() {}

void sem_init(struct sem *s, int count){
    s->count = count;
    for(int i = 0; i < N_PROC; i++){
        s->pidtable[i] = 0;
        s->proc_stat[i] = 0;
    }
    s->lock = 0;
    

}


int sem_try(struct sem *s){
    spin_lock(&s->lock);

    if (s->count > 0) {
        s->count--;  

        spin_unlock(&s->lock);

        return 1;
    }
    else {
        spin_unlock(&s->lock);
        return 0;
    }
}


void sem_wait(struct sem *s){
    sigset_t mask, oldmask;
    s->pidtable[my_procnum] = getpid();
    while (1){  
        spin_lock(&s->lock);
        signal(SIGUSR1, handler);

        sigemptyset(&mask);
        sigemptyset(&oldmask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);

        if(s->count > 0){
            s->count--;
            s->proc_stat[my_procnum] = 0;
            spin_unlock(&s->lock);

            return;
        }
        spin_unlock(&s->lock);
        s->proc_stat[my_procnum] = 1;
        sigsuspend(&oldmask);
    }
}


void sem_inc(struct sem *s){
    spin_lock (&s-> lock);
    s-> count++;
    for (int i =0; i < N_PROC; i++){
        if (s -> proc_stat[i]){
            s -> proc_stat[i] = 0; 
            if (kill(s-> pidtable[i], SIGUSR1) < 0){
                fprintf(stderr, "Failed to send signal to pid %d: %s\n", s->pidtable[i], strerror(errno));
                exit(EXIT_FAILURE);
            }
        } 
    }
    spin_unlock(&s-> lock);
}
