#include <stdlib.h>
#include "tokenizer.h"


/* 
	Agument validity checking.  Includes -help option.
	Returns error message on invalid input.
*/
void argCheck(int argc, char** argv){

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


int parseFile(FILE* file){

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
	TESTING FUNCTIONALITY OF TOKENIZER.
	SHOULD SPIT OUT INPUT FILE'S ALPHANUM CHARS IN LOWERCASE 
	Usage:  ./index whatever gastrobot.txt
*/
int main(int argc, char **argv){

	argCheck(argc, argv);

	FILE *input_file 		= fopen(argv[2],"r");
	
	parseFile(input_file);


	return 0;
}
