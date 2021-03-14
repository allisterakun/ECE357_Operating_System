#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

int matchcount=0;

void sig_handler(int signo){
	fprintf(stderr,"Total Matched Words: %d\n", matchcount);
	signal(SIGPIPE, SIG_DFL);
	raise(SIGPIPE);
	fprintf(stderr, "Received signal %d\n",signo);
	exit(0);
//	fprintf(stderr
}

int main (int argc, char *argv[]) {
	char * buf [470000];
	char *buf2[470000];
	FILE *in;
//	int matchcount=0;
	int line=0;
	size_t bufsize=470000;
	size_t bufsize2=470000;
	signal(SIGPIPE, sig_handler);

	if (argc==2) {
		in=fopen(argv[1],"r");
		if (in ==NULL) {
			fprintf(stderr,"Error opening %s: %s\n",argv[1],strerror(errno));
			return -1;
		}		

	}
	else {
		
	        fprintf(stderr, "Too little or too many arguments\n");
		return -1;
	}

	while (getline(&buf[line],&bufsize, in) != -1) {
	//	printf("buf[line] is %s\n",buf[line]);
		for (int i=0; i< strlen(buf[line]);i++) {
			buf[line][i] = toupper (buf[line][i]);		
	//		printf("%c\n",buf[line][i]);
		}

		line++;
	}
//	printf("line is %d\n",line);
	fclose (in);

	while (getline(&buf2[0], &bufsize2, stdin) != -1) {
//		printf("buf is %s\n",*buf2);


		for (int i=0; i<strlen(buf2[0]);i++) {

			buf2[0][i]=toupper(buf2[0][i]);
//			printf("buf2[0][i] is %c\n",buf2[0][i]);

//			printf("buf2[0] is %s \n",buf2[0]);



		}


		for (int i=0; i<=line; i++) {

			if (strcmp(buf2[0], buf[i]) == 0) {
				printf("%s \n",buf[i]);
				matchcount++;
			}

		}


	}

	fprintf(stderr, "Matched %d words\n",matchcount);




	return 0;
}





