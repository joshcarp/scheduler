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
typedef struct process{
	int key;
	int arrival;
	int procid;
	int memsize;
	int jobtime;
	int remaining;
	struct process * llNext;
	struct process * queueNext;
	page **memory;
	int memsize;
	int last_execution_time;
}process;
typedef struct page{
	process* parent;
	int last_used;
	int finished;
	int allocated;
	int id;
}page;

typedef struct mem{
	page **memory;
	int len;
}mem;
typedef struct queue{
	struct process *front, *rear;
}queue;

dictKey_t *NewKey();
process *newData (char *entry);
process *blankData();
int cmp(int, int);
void printData(process*);
void printKey(dictKey_t);
void printFData(process*, FILE*);
void printFAttr(process *d, FILE* outFile, char* Attr);
// char* getField(datat *d, int fieldNum, char* dest);
void fieldFromStr(char *row, char dest[MAXFIELDNUM][MAXFIELD]);
process* linkData(process*, process*);
int searchData(process *,int, FILE*, char *);
void freeData(process*);
// char *fgets_noNL(char *, int, FILE *);


queue *NewQueue ();

void addToQueue (queue *q, process *d);

process* getFromQueue (queue *q);