
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

int IOredir(int fdold, int fdnew){
	if (dup2 (fdold, fdnew) < 0) {
		fprintf(stderr, "ERROR when redirecting to target file discripter %d using dup2(): %s\n", fdnew, strerror (errno));
	    exit(EXIT_FAILURE);
	}
	if (close(fdold)<0){
		fprintf (stderr,"ERROR closing file discripter %d: %s [dangling file discripter]\n",fdold, strerror (errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
void CLOSE(int fdtodel){
	if (close(fdtodel)<0){
		fprintf (stderr, "ERROR closing file discripter %d: %s [dangling file discripter]\n",fdtodel, strerror (errno));
		exit(EXIT_FAILURE);
	}
}
int main(int argc, char **argv){
    int pipefd1[2];
    int pipefd2[2];
    int waitStatus;
    pid_t child;
	pid_t grandchild;
	pid_t greatgrandchild;
	if (argc >2) {
	    fprintf(stderr, "Input Format: %s <number>[optional]\n", argv[0]);
	    exit(EXIT_FAILURE);
	}
    if (pipe(pipefd1) == -1) {
        perror("Pipe");
        exit(EXIT_FAILURE);
    }
    child = fork();
	if (child == 0) {
        IOredir(pipefd1[1],1);
        CLOSE(pipefd1[0]);
		if(argc==1){
			char* argv1[2];
			argv1[0]="./wordgen.out";
			argv1[1]=NULL;
			if (execvp (argv1[0],argv1)==-1) {
				fprintf (stderr, "ERROR executing wordgen.out using execvp(): %s\n", strerror (errno));
				exit(EXIT_FAILURE);
			}
		}else{
			char* argv1[3];
			argv1[0]="./wordgen.out";
			argv1[1]=argv[1];
			argv1[2]=NULL;
			if (execvp (argv1[0],argv1)==-1) {
				fprintf (stderr, "ERROR executing wordgen.out using execvp(): %s\n", strerror (errno));
				exit(EXIT_FAILURE);
			}
		}
		exit(EXIT_FAILURE);
	} else if (child < 0) {
		fprintf (stderr, "ERROR forking for wordgen.out: %s\n", strerror (errno));
		return EXIT_FAILURE;
	} else {
		if (pipe(pipefd2) == -1) {
			perror("Pipe");
			exit(EXIT_FAILURE);
		}
		grandchild=fork();
		if (grandchild == 0) {
			CLOSE(pipefd1[1]);
			CLOSE(pipefd2[0]);
			IOredir(pipefd1[0],0);
			IOredir(pipefd2[1],1);
			char *a[3];
			a[0]="./wordsearch.out";
			a[1]="words.txt";
			a[2]=NULL;
			if (execvp ("./wordsearch.out",a)==-1) {
						fprintf (stderr, "ERROR executing wordsearch.out using execvp(): %s\n", strerror (errno));
						exit(EXIT_FAILURE);
			}
		} else if(grandchild<0){
			fprintf (stderr, "ERROR forking for wordsearch.out: %s\n", strerror (errno));
			return EXIT_FAILURE;
		}else{
			greatgrandchild=fork();
			if (greatgrandchild == 0) {
				CLOSE(pipefd1[0]);
				CLOSE(pipefd1[1]);
				CLOSE(pipefd2[1]);
				IOredir(pipefd2[0],0);
				char* argv2[2];
				argv2[0]="./pager.out";
				argv2[1]=NULL;
				if (execvp ("./pager.out",argv2)==-1) {
							fprintf (stderr, "ERROR executing pager.out using execvp(): %s\n", strerror (errno));
							exit(EXIT_FAILURE);
				}
			}else if(greatgrandchild<0){
						fprintf (stderr, "ERROR forking for pager: %s\n", strerror (errno));
						exit(EXIT_FAILURE);
			}else{
                CLOSE(pipefd1[0]);
				CLOSE(pipefd1[1]);
				CLOSE(pipefd2[0]);
				CLOSE(pipefd2[1]);
				pid_t children;
				while((children = waitpid(-1,&waitStatus,0))>0 && children!=1){
					if (WIFEXITED(waitStatus)) {
						fprintf(stderr,"child %d exited, with %d\n",children,WEXITSTATUS(waitStatus));
					}else if (WIFSIGNALED(waitStatus)) {
                        fprintf(stderr,"child %d exited - killed by signal %d\n", children, WTERMSIG(waitStatus));
					}else if (WIFSTOPPED(waitStatus)) {
						fprintf(stderr,"pid %d exited - stopped by signal %d\n",children, WSTOPSIG(waitStatus));
					}
				}
				if(errno == EINTR && children<0){
					fprintf(stderr,"waitpid failed because of a signal interuption by calling process: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}
