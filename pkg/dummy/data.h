#include <stdio.h>
#define LT -1
#define GT 1
#define EQ 0

#define MAXBUFF 258
#define MAXFIELD 128

// Typedef the key so it could theoreticallybe changed 
typedef char dictKey_t[20];

/* datat is a linked list and has a key, and a 'location', which is the
amount of bytes of data from the start of the input file. When needing to print this value will 
be used to offset the start of the file (ie pointer arithmetic for files)*/
typedef struct datat{
	dictKey_t key;
	int location;
	struct datat * next;
	FILE *file;
}datat;


dictKey_t *NewKey();
datat *newData(dictKey_t, int, FILE*);
datat *blankData();
int cmp(dictKey_t, dictKey_t);
void printData(datat*);
void printKey(dictKey_t);
void printFData(datat*, FILE*);
void printFAttr(datat *d, FILE* outFile, char* Attr);
char* getField(datat *d, int fieldNum, char* dest);
char* fieldFromStr(char *row, int fieldNum, char* dest);
void linkData(datat*, datat*);
int searchData(datat *,int, FILE*, char *);
void freeData(datat*);
char *fgets_noNL(char *, int, FILE *);
