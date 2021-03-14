#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>




int main (int argc, char *argv[]) {
	int numwords;
	int wordcount=0;
   	srand(time(0));
	int endless=0;  

	if ((argc == 2 && strcmp(argv[1],"0")==0) || (argc ==1)) {             //check the conditions for an endless loop
//		printf("this is when you input ./a.out 0 or ./a.out only\n");
		endless=1;
	
	}


	else if (argc ==2) {
//		printf("this is when you input ./a.out (number) \n");
		int len=strlen(argv[1]);
		char num[len+1];
		strcpy(num,argv[1]);

		for (int i=0; i<len; i++) {                    //check if argv[1] is a number
			if (isdigit(num[i])==0) {
				fprintf(stderr,"Invalid input: Please make sure you specified a number after the executable name\n");
				return -1;

			}
		}
		numwords= atoi(argv[1]);
//		printf("numwords is %d\n",numwords);

	}
	else {
		fprintf(stderr,"Invalid input: Please make sure you specified a number after the executable name\n");
		return -1;
	}		

	while (wordcount <numwords || endless==1) {

		int length= (rand() %13) +3;               //randomly generate the length of each word to be between 3 and 15 inclusive
//		printf("length is %d\n",length);
		char word[length];
   		for(int i = 0 ; i < length ; i++ ) {       
			word[i]= 'A' +rand() %26;

  	 	}	

		word[length]='\0';
		printf("%s\n",word);

		wordcount++;



	}


	return 0;
}




