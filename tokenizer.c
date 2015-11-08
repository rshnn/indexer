#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOKENSIZE 1000

/*
 * Tokenizer type.  Simplified for indexer.
 */
typedef struct {
	FILE *file;
} TokenizerT;



/*
	TKCreate creates a new TokenizerT object for a given input file.
	Returns a pointer to the intialized tokenizer structure.
*/
TokenizerT *TKCreate(FILE *file) {
	TokenizerT *tk = (TokenizerT *)malloc(sizeof(TokenizerT));
	if(tk == NULL) {
		return NULL;
	}
	tk->file = file;
	return tk;
}



/*
	TKDestroy destroys a TokenizerT object. 
*/
void TKDestroy( TokenizerT * tk ) {
	free(tk);
}




/*
 	TKGetNextToken returns the next token from the token stream as a char*.
 	Space is dynamically allocated an the caller is responsible for freeing
 	the space once it is no longer needed.
 */

char *TKGetNextToken( TokenizerT * tk ) {

	char *buffer = (char*)malloc(sizeof(char)*TOKENSIZE);

	if(buffer==NULL){
		printf("Error:  Failure allocating memory for tokenizer structure.\n");
		return NULL;
	}

	char curr		= fgetc(tk->file);
	int index 	 	= 0;

	/* Populate buffer with alphanumeric characters.  Terminate on non-alnum. */
	while(curr != EOF && index < TOKENSIZE){

		curr 	= tolower(curr);

		if(index == 0 && isalpha(curr)){
			buffer[index] 	= curr;
			index++;
			curr	= fgetc(tk->file);
			continue;

		}

		if(index!=0 && isalnum(curr)){
			buffer[index]	 = curr;
			index++;
		}else{
			if(index > 0)
				break;
		}
		curr	= fgetc(tk->file);
	}

	/* EOF reached with an empty buffer. */
	if(index == 0){
		return NULL;
	}

	/* Preparing output string from buffer. */
	buffer[index]	= '\0';
	buffer  		= (char * )realloc(buffer, sizeof(char)*(index+1));

	return buffer;
}































