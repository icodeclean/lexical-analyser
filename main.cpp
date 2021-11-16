/* main.c - a lexical analyzer system for simple
 arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
int goodTokens[1000];
char nextChar;
int lexLen;
int tokenLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int getNextToken(void);
void error(char ch);

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define DOT 3

/* Token codes */
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define IF_CODE 31
#define ELSE_CODE 32
#define FOR_CODE 30
#define WHILE_CODE 33
#define ELSE_CODE 32
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37

/******************************************************/
/* main driver */
int main(void) {
	/* Open the input data file and process its contents */
	 if ((in_fp = fopen("front.in", "r")) == NULL)
	 	printf("ERROR - cannot open front.in \n");
	 else {
	 	getChar();
	 do {
	 	getNextToken();
	 } while (nextToken != EOF);
	 }
	 /* Print list of proper tokens */
	for(int i = 0; i < 1000; i++)
		if (goodTokens[i] != 0) 
      		printf("%d ", goodTokens[i]);
	 return 0;
}
/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 nextToken = DIV_OP;
			 break;
		 default:
			 addChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
		else if (nextChar == '.')
		 	charClass = DOT;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
}


/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}

/******************************************************/
/* getNextToken - a simple lexical analyzer for arithmetic
 expressions */
int getNextToken(void) {
	 lexLen = 0;
	 getNonBlank();
	 switch (charClass) {
		/* Identifiers */
		 case LETTER:
			 addChar();
			 getChar();
			 while (charClass == LETTER || charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			 if (strcmp(lexeme,"for") == 0) {
			 goodTokens[tokenLen++] = FOR_CODE;
			 nextToken = FOR_CODE;
			 break;
			 }
			if (strcmp(lexeme,"if") == 0) {
			 goodTokens[tokenLen++] = IF_CODE;
			 nextToken = IF_CODE;
			 break;
			 }
			if (strcmp(lexeme,"else") == 0) {
			 goodTokens[tokenLen++] = ELSE_CODE;
			 nextToken = ELSE_CODE;
			 break;
			 }
			if (strcmp(lexeme,"while") == 0) {
			 goodTokens[tokenLen++] = WHILE_CODE;
			 nextToken = WHILE_CODE;
			 break;
			 }
			if (strcmp(lexeme,"do") == 0) {
			 goodTokens[tokenLen++] = DO_CODE;
			 nextToken = DO_CODE;
			 break;
			 }
			if (strcmp(lexeme,"switch") == 0) {
			 goodTokens[tokenLen++] = SWITCH_CODE;
			 nextToken = SWITCH_CODE;
			 break;
			 }
			 goodTokens[tokenLen++] = IDENT;
			 nextToken = IDENT;
			 break;
		/* Integer literals */
		 case DIGIT:
			 addChar();
			 getChar();
			 while (charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			if (charClass == DOT) {
				addChar();
				getChar();
				while (charClass == DIGIT) {
				 	addChar();
				 	getChar();
			 	}
				goodTokens[tokenLen++] = FLOAT_CODE;
				nextToken = FLOAT_CODE;
				break;
			}
			goodTokens[tokenLen++] = INT_CODE;
			nextToken = INT_CODE;
		 	break;
		/* Parentheses and operators */
		case UNKNOWN:
			 lookup(nextChar);
			 getChar();
			 break;
		/* DOT */
		case DOT:
		    printf("Error: . by itself is an error!");
    		exit(0);

		/* EOF */
		case EOF:
			 goodTokens[tokenLen++] = EOF;
			 nextToken = EOF;
			 lexeme[0] = 'E';
			 lexeme[1] = 'O';
			 lexeme[2] = 'F';
			 lexeme[3] = '\0';
		 	break;
	 } /* End of switch */
	 printf("Next token is: %d, Next lexeme is %s\n",
	 nextToken, lexeme);
	 return nextToken;
} /* End of function getNextToken */

void error(char ch) { 
    printf("Expected token % is missing\n", ch);
    exit(0);
}

