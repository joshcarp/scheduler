#include <stdio.h>
#define LT -1
#define GT 1
#define EQ 0

#define MAXBUFF 258
#define MAXFIELD 128
#define MAXFIELDNUM 5

// Typedef the key so it could theoreticallybe changed 
typedef int dictKey_t;

/* datat is a linked list and has a key, and a 'location', which is the
amount of bytes of data from the start of the input file. When needing to print this value will 
be used to offset the start of the file (ie pointer arithmetic for files)*/
typedef struct datat{
	int key;
	int arrival;
	int procid;
	int memsize;
	int jobtime;
	int remaining;
	struct datat * next;
}datat;


dictKey_t *NewKey();
datat *newData (char *entry);
datat *blankData();
int cmp(int, int);
void printData(datat*);
void printKey(dictKey_t);
void printFData(datat*, FILE*);
void printFAttr(datat *d, FILE* outFile, char* Attr);
// char* getField(datat *d, int fieldNum, char* dest);
void fieldFromStr(char *row, char dest[MAXFIELDNUM][MAXFIELD]);
datat* linkData(datat*, datat*);
int searchData(datat *,int, FILE*, char *);
void freeData(datat*);
// char *fgets_noNL(char *, int, FILE *);
