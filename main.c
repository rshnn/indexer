#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "tokenizer.h"

/* 
	Agument validity checking.  Includes -help option.
	Returns error message on invalid input.
*/
void argCheck(int argc, char** argv)
{

	char* argNumErr = "Incorrect number of arguments given.  Use \"index -help\" for proper usage infomation.\n";  
	switch(argc){
		case 2:
			if ( strcmp(argv[1], "-help") == 0 ){
				fprintf(stderr, "\nProper usage:  ./index \"<inverted-index file name> <directory or file name>\"\n");
			}
			exit(0);

		case 3:
			break;
	
		default: 
			fprintf(stderr, "%s\n", argNumErr);
			exit(0);
	}
}

/* 
	Creates and parses through an input file using a tokenizer object.
	Caller is responsible for opening and closing the input file.
	Tokenizer object is intialized and destroyed by this function.
*/
int parseFile(FILE* file)
{

	TokenizerT* tk 	= TKCreate(file);

	/* Currently only prints out file contents.  To be changed later */
	char* token;
	while((token = TKGetNextToken(tk)) != 0){
		printf("%s ",token);
	}

	TKDestroy(tk);
	return 0;
}

/*
	exploreDirectories travels through directories given a char* inputname.
	Recursive calls are initiated on file pointers until Files are reached.
	The base case handles files by calling parseFile.
*/
int exploreDirectories(char* inputname)
{

	/* enum value will keep record of current type */
	enum{File, Directory} type 		= Directory;
	
	void *fileptr					= opendir(inputname);
	struct dirent *entry;

	if(fileptr == NULL){
		type 		= File;
		fileptr 	= fopen(inputname, "r");
		
		if(fileptr == NULL){
			perror(fileptr);
			return 0;
		}
	}

	/* Current recursive iteration is a directory */
	if(type == Directory){

		DIR *directory 	= (DIR *)fileptr;
		for(;;){
			entry 	= readdir(directory);

			/* Empty directory */
			if(entry == NULL){
				break;
			}

			/* Protection against recursing backwards */
			if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){
				continue;
			}

			/* Save filepath as string */
			char* next 	= (char *)malloc(strlen(inputname) + strlen(entry->d_name) + 2);
			strcpy(next, inputname);
			strcat(next, "/");
			strcat(next, entry->d_name);

			/* Recursive call */
			printf("Current directory:\t%s\n",next);
			exploreDirectories(next);
			free(next);
		}
		closedir(directory);

	}
	/* Current recursive iteration is a file (Base case) */
	else{
		printf(">>Exploring file:\t%s \n", inputname);
		FILE *file = (FILE *)fileptr;
		parseFile(file);
		printf("\n\n");
		fclose(file);
	}

	return 0;
}


/* 
	TESTING FUNCTIONALITY OF TOKENIZER.
	SHOULD SPIT OUT INPUT FILE'S ALPHANUM CHARS IN LOWERCASE 
	Usage:  ./index whatever gastrobot.txt
*/
int main(int argc, char **argv)
{
	argCheck(argc, argv);
	exploreDirectories(argv[2]);

	// FILE *input_file 		= fopen(argv[2],"r");
	// parseFile(input_file);


	return 0;
}
