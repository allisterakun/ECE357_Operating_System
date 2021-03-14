#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

typedef char* string;

#define MAX 10000

int FIFO, FCHR, FDIR, FBLK, FREG, FLNK, FSOCK = 0;
unsigned long totalSize, totalBlock, nlink1, nsymlink, problematic = 0;

int ls(char *pathname);
int stats(char *pathnames);

bool counted = false;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *pathname = argv[1];
    ls(pathname);
    printf("# of block device inodes:           %d.\n", FBLK);
    printf("# of character device inodes:       %d.\n", FCHR);
    printf("# of directory inodes:              %d.\n", FDIR);
    printf("# of FIFO/pipe inodes:              %d.\n", FIFO);
    printf("# of symlink inodes:                %d.\n", FLNK);
    printf("# of regular file inodes:           %d.\n", FREG);
    printf("# of sock inodes:                   %d.\n", FSOCK);
    printf("Total size of all regular files:     %lu.\n", totalSize);
    printf("Total disk blocks allocated:         %lu.\n", totalBlock);
    printf("# of inodes with nlink > 1:         %lu.\n", nlink1);
    printf("# of invalid symlinks:                %lu.\n", nsymlink);
	printf("# of \'problematic\' directory entries %lu.\n", problematic);

    exit(EXIT_SUCCESS);
}

int stats(char *pathname) {
	int returnval = 0;	
    struct stat sb;
    if (lstat(pathname, &sb) == -1) {
        perror("lstat");
    }
  
    //printf("File type:                ");

    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:
            //printf("block device\n");
            FBLK++;
            if(sb.st_nlink>1) {
                nlink1++;
            }
            break;

        case S_IFCHR:
            //printf("character device\n");
            FCHR++;
            if(sb.st_nlink>1) {
                nlink1++;
            }
            break;

        case S_IFDIR:
            //printf("directory\n");
	        returnval=0;
	        returnval=access(pathname, X_OK);
            if (returnval!=0) {                       //there is an error if you are trying to traverse through a directory without the execute permission
                if (errno == ENOENT) {
                    fprintf(stderr, "Cannot open directory %s : %s \n",pathname,strerror(errno));
                }
                if (errno ==EACCES) {

                    fprintf(stderr, "Denied execution access for directory %s : %s \n",pathname, strerror(errno));
                }
            }
            FDIR++;
            ls(pathname);
            break;

        case S_IFIFO:
            //printf("FIFO/pipe\n");
            FIFO++;
            if(sb.st_nlink>1) {
                nlink1++;
            }
            break;

        case S_IFLNK:
            //printf("symlink\n");
            FLNK++;
            if(sb.st_nlink>1) {
                nlink1++;
            }
            if(access(pathname, F_OK) != 0) {
                nsymlink++;
                printf("Invalid symbolic link with path: %s\n", pathname);
            }
            break;

        case S_IFREG:
            //printf("regular file\n");
            FREG++;
            totalSize += sb.st_size;
            totalBlock += sb.st_blocks;
            if(sb.st_nlink>1) {
                nlink1++;
            }
            break;

        case S_IFSOCK:
            //printf("socket\n");
            FSOCK++;
            if(sb.st_nlink>1) {
                nlink1++;
            }
            break;

        default:
            //printf("unknown?\n");
            break;

    }

    return 0;
}


int ls(char *pathname){
    int i = 0, j = 0, h = 0, k = 0, n = 0;

    char *pathnames[MAX];
    for(j = 0; j < MAX; j++) {
        pathnames[j] = NULL;
    }
    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(pathname);

    if (dr == NULL) { // opendir returns NULL if couldn't open directory
        fprintf(stderr, "Could not open current directory %s: %s\n",pathname, strerror(errno) );
        return 0;
    }

    i = 0;
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    //
    errno=0;
    while ((de = readdir(dr)) != NULL) {
        char* proc = "proc";
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") || !strcmp(de->d_name, "proc")){
            goto jump;
        }
        
        
        char *buffer, *buffer2;
		buffer = (char*) malloc(255 * sizeof(char));
        buffer2 = (char*) malloc(255 * sizeof(char));
        memset(buffer, 0, 255 * sizeof(char));
        memset(buffer2, 0, 255 * sizeof(char));

		if (buffer==NULL || buffer2 == NULL) {
			fprintf(stderr, "Failed memory allocation\n");
			return 0;
		}

		/* Copy original path into larger buffer */
		strcpy(buffer, pathname);
        strcpy(buffer2, de->d_name);
		/* Concatenate a / for separation */
		strcat(buffer, "/");

        /* Concatenate the new directory */
        pathnames[i] = strcat(buffer, de->d_name);
        //printf("pathnames[%d] = %s\n", i, pathnames[i]);
        i++;
        for(n = 0; n < strlen(buffer2) + 1; n++) {
            if(((42 < buffer2[n] && buffer2[n] < 59) || (buffer[n] == 47) || (63 < buffer2[n] && buffer2[n] < 91) || (93 < buffer2[n] && buffer2[n] < 96) || (96 < buffer2[n] && buffer2[n] < 127) || buffer2[n] == 0)) {
                continue;
            } else {
                struct stat sbs;
                lstat(buffer, &sbs);
                if ((sbs.st_mode & S_IFMT) == S_IFDIR){
                    fprintf(stderr, "Not Pathname: \'%s\'\n %s\n", buffer, buffer2);
                    problematic++;
                    break;
                }
            }
        }
        jump:
            ;

    }

    if (errno) {
	    fprintf(stderr, "Error reading directory %s : %s \n",pathname, strerror(errno));
    }

    if (closedir(dr)<0) {
	    fprintf(stderr, "Error closing directory %s : %s \n", pathname, strerror(errno));
    }


    for (k = 0; pathnames[k] != NULL; k++) {
        //printf("\n%d\n", k);
        //printf("name: %s\n", pathnames[k]);
        stats(pathnames[k]);
    }

    return 0;
}
