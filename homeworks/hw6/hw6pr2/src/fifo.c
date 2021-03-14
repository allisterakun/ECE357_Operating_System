#include "fifo.h"

void fifo_init(struct fifo *f){
    f->read = 0;
    f->write = 0;
    sem_init(&f->empty, 0);
    sem_init(&f->full, MYFIFO_BUFSIZ);
    sem_init(&f->mutex, 1);

}
void fifo_wr(struct fifo *f,unsigned long d){
    while (1) {
		sem_wait (&f->full);
		if (sem_try (&f->mutex)) {
			f->buff[f->write] = d;
			f->write++;
			f->write %= MYFIFO_BUFSIZ;
			sem_inc (&f->mutex);
			sem_inc (&f->empty);
			break;
		} else {
			sem_inc (&f->full);
		}
	}

}
unsigned long fifo_rd(struct fifo *f){
    unsigned long d;
	while (1) {
        
		sem_wait (&f->empty);
        
		if (sem_try (&f->mutex)) {
            
			d = f->buff[f->read];
			f->read++;
			f->read %= MYFIFO_BUFSIZ;
			sem_inc (&f->mutex);
			sem_inc (&f->full);
			break;
		} else {
            
			sem_inc (&f->empty);
		}
	}
    return d;
}