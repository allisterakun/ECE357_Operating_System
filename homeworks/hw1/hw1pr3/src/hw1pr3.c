#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<ctype.h>
#include<stdbool.h>
extern int errno;

int concatenate(int inputFd, int outputFd, char *inputFilename);

char buf[4096];
int totalReads, totalWrites, totalBytes = 0;

int main(int argc, char *argv[]){
    char *outputFilename;
    int opt, j,k,l;
    int input = 1;          //by default, the first argument is the first inputfile name (assuming no output file specified).
    int inputFd = 0;        //input fd is set to 0 (standard input) by default.
    int outputFd = 1;       // output fd is set to 1 (standard output) by default.
    
    while ((opt = getopt(argc, argv, "o")) != -1){
        switch(opt){
            case 'o':
                outputFilename = argv[2]; outputFd = open(outputFilename, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                if(argc != 3) input = 3;
                break;
            default:
                break;
        }
    }
    if(argc == 1 || ((input == 1) && (argc == 3) && (strcmp(argv[1], "-o") == 0))){
            j = concatenate(inputFd, outputFd, "standard input");
        goto bp;
    }
    if(outputFd < 0){
        fprintf(stderr, "ERROR: %s when opening %s.\n", strerror(errno), outputFilename);
        return -1;
    }
    for( ; input < argc; input++){
        if(strcmp(argv[input], "-") == 0){
            inputFd = 0;                        //setting input fd to 0 (standard input) if "-" is passed in
            argv[input] = "standard input";     //setting the input file name to "standard input"
        }
        else{
            inputFd = open(argv[input], O_RDONLY, 0666);
            if(inputFd < 0){
                fprintf(stderr, "ERROR: %s when opening %s.\n", strerror(errno), argv[input]);
                return -1;
            }
        }
        j = concatenate(inputFd, outputFd, argv[input]);
        if(inputFd != 0){
            k = close(inputFd);
            if(k < 0){
                fprintf(stderr, "ERROR: %s when closing %s.\n", strerror(errno), argv[input]);
                return -1;
            }
        }
        
        if(j<0){
            return -1;
        }
    }
bp:
    if(outputFd != 1){
        l = close(outputFd);
        if(l < 0){
            fprintf(stderr, "ERROR: %s when closing %s.\n", strerror(errno), outputFilename);
            return -1;
        }
    }
    
    fprintf(stderr, "A total of %d bytes are transferred to the output file.\n A total of %d read system calls and %d write system calls are made.\n", totalBytes, totalReads, totalWrites);
    return 0;
}

int concatenate(int inputFd, int outputFd, char *inputFilename){
    int m, n;
    int flag = 1;
    int reads =0; int writes = 0; int bytes = 0;
    while((m = read(inputFd, buf, sizeof(buf))) > 0){
        reads++; totalReads++;
        for (int i = 0; i < m; i++) {
            if ( (!(isprint(buf[i]) || isspace(buf[i]) )) && flag) {
                flag = 0;
                printf("WARNING: The file '%s' is observed to be a binary file or had binary elements in it!\n", inputFilename);
            }
        }
        
        n = write(outputFd, buf, m);
        writes++; totalWrites++;
        if (n == m){
            // all the inputs read are written into the output file with no error occurred.
            bytes += n; totalBytes += n;
        }
        else if(n <0) {
            fprintf(stderr, "ERROR: %s when writing %s.\n", strerror(errno), inputFilename);
            return -1;
        }
        else{
            fprintf(stderr, "ERROR: Not all inputs read from '%s' are transferred to the output file.\n", inputFilename);
            return -1;
        }
    }
    if (m < 0) {
        fprintf(stderr, "ERROR: %s when reading %s.\n", strerror(errno), inputFilename);
        return -1;
    }
    fprintf(stderr, "\n%d bytes are transferred from %s to the output file.\n %d read system calls and %d write system calls are made.\n", bytes, inputFilename, reads, writes);
    return 0;
}
