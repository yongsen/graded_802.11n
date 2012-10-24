#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define MAX 20480
#define MIN 8
#define WINDOW 10
#define LEN 64
#define N 100
#define P "p"
#define X "x"
#define OUT "result"
#define TOTAL 20000
typedef struct
{
	unsigned x[MAX];
	float p[MAX];
}status;
typedef struct
{
	unsigned Ws[MAX];
	float Ps[MAX];
	float Error[MAX];
}DSWA;
