#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *permstring;

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};


char* permission_string(struct stat *statbuf) {
    	char *perm_string;
	if ((perm_string = malloc(10 * sizeof(char))) == NULL) {
        	fprintf(stderr, "Error: malloc failed. %s.\n",
                strerror(errno));
        	exit(EXIT_FAILURE);
   	}
	for (int i = 0; i < 9; i += 3) {
        	perm_string[i] = statbuf->st_mode & perms[i] ? 'r' : '-';
        	perm_string[i + 1] = statbuf->st_mode & perms[i + 1] ? 'w' : '-';
		perm_string[i + 2] = statbuf->st_mode & perms[i + 2] ? 'x' : '-';
	}
	perm_string[9] = '\0';
    	return perm_string;
}










int dir_entry(char *directory){
      int stat_count = 0;
	char path[PATH_MAX];
    	if (realpath(directory, path) == NULL) {
        	fprintf(stderr, "Error: Cannot get full path of file '%s'. %s.", directory, strerror(errno));

        	return EXIT_FAILURE;
	}
    	
	DIR *dir;
    	if ((dir = opendir(path)) == NULL) {
        	fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", path, strerror(errno));
        	return EXIT_FAILURE;
   	}

	char full_filename[PATH_MAX + 1];

	if (strcmp(path, "/")) {
		size_t copy_len = strnlen(path, PATH_MAX);
        	memcpy(full_filename, path, copy_len);
        	full_filename[copy_len] = '\0';
	
	}

	size_t pathlen = strlen(full_filename) + 1;
    	full_filename[pathlen - 1] = '/';
    	full_filename[pathlen] = '\0';

	struct dirent *entry;
    	struct stat sb;
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || 
		    strcmp(entry->d_name, "..") == 0) {continue; }
	

		strncpy(full_filename + pathlen, entry->d_name, PATH_MAX - pathlen);
	
		if (stat_count == 0){
                    if (stat(full_filename, &sb) < 0) {
                        fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n",
                                full_filename, strerror(errno));
                        stat_count++;
                        continue;
                    }
             }else{
                    if (lstat(full_filename, &sb) < 0) {
                        fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n",
                                full_filename, strerror(errno));
                        continue;
                    }
             }

		if (S_ISDIR(sb.st_mode)) {
			dir_entry(full_filename);









        	} else {
            		//check the permission string, print.
			//printf("PERM STR: %s \n", permission_string(&sb));//test
			char *file_permission = permission_string(&sb);
						
			//printf("%s  ", full_filename);//test

			if (strcmp(file_permission, permstring) == 0){
				printf("%s\n", full_filename);
				
			}			
        		free(file_permission);
	
		}

	}
	 	closedir(dir);
    		return EXIT_SUCCESS;


}





void usage_message(){

	printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");


}




int main (int argc, char ** argv){
    	int opt;
	char *directory;
/*	
	if (argc !=5){
		fprintf(stderr, "Error: incorrect number of arguments.\n");
		usage_message();
		return EXIT_FAILURE;

		
	}

*/

	int d_flag = 0;
	int p_flag = 0;


	opterr = 0;
    	while ((opt = getopt(argc, argv, "d:p:h")) != -1) {
            	switch (opt) {
                	case 'd':
				directory = optarg;
				d_flag = 1;
                        	break;
                	case 'p':
				permstring = optarg;
				p_flag = 1;
                        	break;
			case 'h':
				printf("\n");
				usage_message();
				return EXIT_SUCCESS;
                	case '?':
				if (optopt == 'd'){
					fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
					return EXIT_FAILURE;
				
				}else if (optopt == 'p'){
					fprintf(stderr, "Error: Required argument -p <permissions string> not found.\n");
					return EXIT_FAILURE;
				} else {
				
                        		fprintf(stderr, "Error: Unknown option '%c' received.\n", optopt);// invalid flag
					
					usage_message();
                        		return EXIT_FAILURE;
				}
                	default:
                        	break;

                }
    }
	if (d_flag == 0){
		fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
		return EXIT_FAILURE;
	} else if (p_flag == 0){
		fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
                return EXIT_FAILURE;
	}
	
	//str is length 9
	if (strlen(permstring) != 9){
		fprintf(stderr, "Error: Permission string '%s' is of invalid size.\n", permstring);
		return EXIT_FAILURE;	
	}
	//element check
	for (int i=0; i<9; i++){
		char ele = permstring[i];
		if (i == 0 || i == 3 || i == 6){
			if (ele == 'r'|| ele == '-'){continue;}
		} else if ( i == 1 || i == 4 || i == 7){
			if (ele == 'w'|| ele == '-'){continue;}
		}else if (i == 2 || i == 5 || i == 8){
			if (ele == 'x'|| ele == '-'){continue;}
		
		}
		
		fprintf(stderr, "Error: Permission string contains invalid character: '%c'at index %d. \n", ele, i);
		return EXIT_FAILURE;	
		
	}//for loop
	
	
	
	
	
	
	dir_entry(directory);


	return 1;











}//end of main 


