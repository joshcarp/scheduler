#include <stdio.h>
#include <stdbool.h>
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

typedef struct page page;
struct datat{
	int key;
	int arrival;
	int procid;
	int memsize;
	int jobtime;
	int loadtime;
	int remaining;
	int finishingtime;
	struct datat *llNext;
	struct datat *queueNext;
	struct datat *queuePrev;
	page **memory;
	int memunits;
	int last_execution_time;
};

typedef struct datat datat;

struct page{
	datat* parent;
	bool allocated;
	int id;
	int size;
};



struct mem{
	page **memory;
	page **recently_evicted;
	int num_recently_evicted;
	int cap;
	int len;
};

typedef struct mem mem;
struct queue{
	datat *front, *rear;
};

typedef struct queue queue;

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


queue *NewQueue ();

void addToQueue (queue *q, datat *d);

datat* getFromQueue (queue *q);