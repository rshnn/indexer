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


/* 
	TESTING FUNCTIONALITY OF TOKENIZER.
	SHOULD SPIT OUT INPUT FILE'S ALPHANUM CHARS IN LOWERCASE 
	Usage:  ./index whatever gastrobot.txt
*/
int main(int argc, char **argv){

	argCheck(argc, argv);

	FILE *input_file 		= fopen(argv[2],"r");
	TokenizerT* tokenizer 	= TKCreate(input_file);


	char* tok;
	while((tok = TKGetNextToken(tokenizer)) != 0){
		printf("%s ",tok);
	}

	TKDestroy(tokenizer);


	return 0;
}
