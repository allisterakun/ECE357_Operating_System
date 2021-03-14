//Steven LEE, Amy LEONG, Allister LIU
//ECE357 hw3 pr3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdbool.h>

#define ARRAY_SIZE 4096 

int mypwd();
int mycd (char* path);
void myexit();
int readInput(FILE* infile);
int children(char *input[], int length);
int exitstat = 0;

int main(int argc, char* argv[]){
    FILE *infile;
    if (argc > 2){
        fprintf(stderr,"Wrong format, please check your input format.\n");
    } 
    else if (argc == 2){
        if ((infile = fopen(argv[1],"r")) == NULL){
            fprintf(stderr, "Cannot open file to read: %s.\n", strerror(errno));
        }
        else{
            readInput(infile);
        } 
    }
    else if (argc == 1){
        readInput(stdin);
    }
    return 0;
}

int readInput(FILE* infile){
    int filenum = -1;
    int file_mode; 
    char *input;
    size_t size = ARRAY_SIZE;

    while(1){
        int length= 1;
        int nRedirect = 0;
        int fd;
        
        // allocating memory for input array
        input = (char *)malloc(ARRAY_SIZE * sizeof(char));
        if( input == NULL){
            perror("Unable to allocate buffer");
        }

        // get a line of input
        getline(&input, &size, infile);
        // get the number of arguments = length
        for (int m = 0; input[m] != '\0';m++){
            if (input[m] == ' ' && input[m+1] != ' ' && input[m+1] != '\n' && input[m] != '\n'){
                length++;
            }
        }
        // end case
        if(input[0] == EOF || input[0] == 64 || input[0] == 0) {
            return 0;
        }

        // comments or empty line
        if((input[0] == '#') ||(input[0] == '\n')){ 
            continue;
        }


        char *str[length];
        for (int i = 0; i < length; i++){
            str[i] = (char *)malloc(ARRAY_SIZE * sizeof(char)); 
        }
        
        char *input2 = strdup(input);

        // separate input 
        strcpy(str[0], strtok(input2, " \n"));
        for(int n = 1; n < length; n++){ 
            strcpy(str[n], strtok(NULL, " \n"));
        }
        // pwd, cd, exit special cases
        if(!strcmp(str[0], "pwd")){
            mypwd();
        }
        else if(!strcmp(str[0], "cd")){
            if(length == 1){
                str[1] = NULL;
            }
            mycd(str[1]);
        }
        else if(!strcmp(str[0], "exit")){
            if(length == 1){
                str[1] = NULL;
            }
            myexit(str[1]);
        }

        // all other commands
        else{
            pid_t pid;
            int status; 
            int fdold;
            float realtime;
            
            struct rusage ru;
            struct timeval start;
            struct timeval end;

            if (gettimeofday( &start, NULL) <0){
                fprintf( stderr, "Error with gettimeofday() system call: %s \n", strerror (errno));
            } 

            switch (pid = fork()){

                case 0:
                    exitstat = children(str, length);
                    break;

                case -1: 
                    fprintf(stderr, "Error when forking command %s: %s\n", str[0], strerror(errno));
                    return EXIT_FAILURE;
                
                default:
                    if (wait3(&status,0,&ru ) == -1){
                        fprintf(stderr, "Error with wait: %s \n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    else {
                        if (gettimeofday( &end, NULL) <0){
                            fprintf( stderr, "Error with gettimeofday() system call: %s \n", strerror (errno));
                        } 
                        realtime= end.tv_usec - start.tv_usec; 
                        realtime= (double) realtime/1000000;

                        fprintf( stderr, "User time:  %f seconds\n", (double) ru.ru_utime.tv_usec /1000000);
                        fprintf( stderr, "System time: %f seconds\n", (double) ru.ru_stime.tv_usec/1000000);
                        fprintf(stderr, "Real time: %f seconds\n", realtime); 
                    }

                    if (WIFEXITED(status)){
                        exitstat=WEXITSTATUS(status);
                        fprintf(stderr, "Exit status was %d\n",exitstat);
                    }
                    break;

            }
        }
    }
}


int children(char *str[], int length){
    int nRedirect = 0;
    int fdold;
    char *filename;
    for(int i = 0; i < length && nRedirect <=3 ; i++){
        if(str[i][0] == '<'){
            //redirect stdin
            filename = str[i] + 1;
            if ((fdold=open(filename, O_RDONLY,0666))<0){
                fprintf(stderr, "Cannot open %s : %s.\n", filename, strerror(errno));
                return EXIT_FAILURE;
            }
            if (dup2(fdold,0) <0) {
                fprintf(stderr, "Error with dup2: %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            if (close (fdold)!= 0) {
                fprintf(stderr,"Error while closing %s : %s\n", filename, strerror(errno));
                exit(EXIT_FAILURE);
            }
            nRedirect++;
        }
        
        else if((str[i][0]) == '>'){
            if((str[i][1]) == '>'){
                //redirect stdout with O_WRONLY|O_CREAT|O_APPEND
                filename = str[i] + 2;
                if ((fdold=open(filename, O_WRONLY|O_CREAT|O_APPEND,0666))<0){
                    fprintf(stderr, "Cannot open %s : %s.\n", filename, strerror(errno));
                    return EXIT_FAILURE;
                }
                if (dup2(fdold,1) <0) {
                    fprintf(stderr, "Error with dup2: %s.\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                if (close (fdold)!= 0) {
                    fprintf(stderr,"Error while closing %s : %s\n", filename, strerror(errno));
                    exit(EXIT_FAILURE);
                }
                nRedirect++;
            }
            else{
                //redirect stdout with O_WRONLY|O_CREAT|O_TRUNC
                filename = str[i] + 1;
                if ((fdold=open(filename, O_WRONLY|O_CREAT|O_TRUNC,0666))<0){
                    fprintf(stderr, "Cannot open %s : %s.\n", filename, strerror(errno));
                    return EXIT_FAILURE;
                }
                if(dup2(fdold,1) <0) {
                    fprintf(stderr, "Error with dup2: %s.\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                if (close (fdold)!= 0) {
                    fprintf(stderr,"Error while closing %s : %s\n", filename, strerror(errno));
                    exit(EXIT_FAILURE);
                }
                nRedirect++;
            }
        }
        else if(str[i][0] == '2' && str[i][1] == '>'){
            if((str[i][2]) == '>'){
                //redirect stderr with O_WRONLY|O_CREAT|O_APPEND
                filename = str[i] + 2;
                if ((fdold=open(filename, O_WRONLY|O_CREAT|O_APPEND,0666))<0){
                    fprintf(stderr, "Cannot open %s : %s.\n", filename, strerror(errno));
                    return EXIT_FAILURE;
                }
                if (dup2(fdold,2) <0) {
                    fprintf(stderr, "Error with dup2: %s.\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                if (close (fdold)!= 0) {
                        fprintf(stderr,"Error while closing %s : %s\n", filename, strerror(errno));
                        exit(EXIT_FAILURE);
                }
                nRedirect++;
            }
            else{
                //redirect stderr with O_WRONLY|O_CREAT|O_TRUNC
                filename = str[i] + 3;
                if ((fdold=open(filename, O_WRONLY|O_CREAT|O_TRUNC,0666))<0){
                    fprintf(stderr, "Cannot open %s : %s.\n", filename, strerror(errno));
                    return EXIT_FAILURE;
                }
                if (dup2(fdold,2) <0) {
                    fprintf(stderr, "Error with dup2: %s.\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                if (close (fdold)!= 0) {
                        fprintf(stderr,"Error while closing %s : %s\n", filename, strerror(errno));
                        exit(EXIT_FAILURE);
                }
                nRedirect++;
            }
        }

        // reaching the last input argument                
        if(i == length - 1) {
            int length2 = length - nRedirect;
            char *inputs[length2]; 
            for (int i = 0; i < length2; i++){
                inputs[i] = (char *)malloc(ARRAY_SIZE * sizeof(char)); 
            }
            for(int p = 0; p < length2; p++){
                strcpy(inputs[p], str[p]);
            }
            inputs[length2] = NULL;
            execvp(inputs[0], inputs);
            fprintf(stderr,"ERROR: fail to exec %s because %s\n", inputs[0], strerror(errno));
            exit(127);
        }
        
    }
    return -1;      //shouldnt reach here
}

int mycd (char* path){
    char* location;
    if (path == NULL){
        // default target location to to the environment variable HOME.
        location = getenv("HOME");
    }
    else
    {
        location = path;
    }
    if (chdir(location) < 0){
        fprintf(stderr, "mycd: cant change to destination %s: %s\n", location, strerror(errno));
    }
    return 0;
} 

int mypwd(){
    char s[ARRAY_SIZE];
    if(getcwd(s, ARRAY_SIZE) != NULL){
        printf("%s\n", s);
        return 0;
    }
    else{
        fprintf(stderr, "ERROR: %s when calling pwd.\n", strerror(errno));
        return -1;
    }
}

void myexit(char* path){
     if (path == NULL){
        //add code
        exit(exitstat);
     }
     else{
         exit(atoi(path));
     }
}
