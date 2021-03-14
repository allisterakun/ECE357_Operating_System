#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

int smear(char *target, char *replacement, char *filepath);

int main(int argc, char *argv[]){

    char *target;
    char *replacement;
    if(argc < 4){
        fprintf(stderr, "Too few arguments, please try again...\n");
        fprintf(stderr, "Correct Usage: ./smear.out TARGET REPLACEMENT file1 {file2...}\n");
        exit(1);
    }
    
    target = argv[1];
    replacement = argv[2];
    if(strlen(target) != strlen(replacement)){
        fprintf(stderr, "WARNING: the target string and the replacement string have different length, please try again...\n");
        exit(1);
    }
    for(int i = 3; i < argc; i ++){
        if(smear(target, replacement, argv[i]) != 0){
            continue;
        }
    }

    return 0;
}

int smear(char *target, char *replacement, char *filepath){
    int fd;
    if((fd = open(filepath, O_RDWR)) < 0){
        fprintf(stderr, "ERROR: cannot open file \"%s: %s\n", filepath, strerror(errno));
        return -1;
    }

    struct stat statbuf;
    int err;
    if((err = fstat(fd, &statbuf)) < 0){
        fprintf(stderr, "ERROR: cannot get the file status of %s: %s\n",filepath, strerror(errno));
        return -1;
    }
    
    if(statbuf.st_size < 1){
        fprintf(stderr, "ERROR: file %s is too large to be mapped in its entirety into virtual address space", filepath);
        return -1;
    }
    
    char *ptr;
    ptr = mmap(NULL,statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
    
    if(ptr == MAP_FAILED){
        fprintf(stderr, "ERROR: cannot map file %s: %s.\n", filepath, strerror(errno));
        return -1;
    }
    
    char temp[1024];
    memset(temp,0,sizeof(temp));
    int n = 0;
    
    for(size_t i = 0; i < statbuf.st_size; i++){
        if( (ptr[i] != ' ') && (ptr[i] != 10) && (ptr[i] != 0)){
            temp[n++] = ptr[i];
        }
        else{
            if(!strcmp(target, temp)){
                for(int j = 0; j < strlen(replacement); j++){
                    ptr[i-j-1] = replacement[strlen(replacement)-j-1];
                }
            }
            memset(temp,0,strlen(temp));
            n = 0;
        }
    }
    
    if(close(fd) != 0){
        fprintf(stderr, "ERROR: cannot close file %s: %s\n", filepath, strerror(errno));
        return -1;
    }
    if(munmap(ptr, statbuf.st_size) != 0){
        fprintf(stderr, "ERROR: cannot unmap file %s: %s\n", filepath, strerror(errno));
        return -1;
    }
    
    return 0;
}
