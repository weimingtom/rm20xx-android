#ifndef WCVOCAB_H
#define WCVOCAB_H

/* Defines */
#define VOCAB_NEW_GAME 0x72
#define VOCAB_CONTINUE 0x73
#define VOCAB_SHUTDOWN 0x75

/* Types */
typedef struct
{
	int id; /* ID code for the string */
	char *str; /* Actual string */
}vocabulary;

/*
	Loads all vocab
*/
extern void wcvocab_load_all(FILE *f,int len);

/*
	Releases all vocab
*/
extern void wcvocab_free_all();

/*
	Gets a vocabulary word
*/
extern char *wcvocab_get(int code);

#endif