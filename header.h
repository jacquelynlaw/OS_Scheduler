#ifndef header_h
#define header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 300
#define NAMESIZE 50
// For the state of the process
#define WAITING = 0
#define READY = 1

// Struct declarations
typedef struct schedparams {
	int processCount;
	int runTime;
	char* schedAlg;
	int quantum;

} schedparams;

typedef struct process {
	char process_name[BUFFSIZE];
	int arrival_time;
	int burst_length;
	
} process;

// Function declarations: main.c
process *createProcess(char* name, int arrival, int burst);
int readProcessCount(FILE *fp);
int readRunTime(FILE *fp);
char* readAlgType(FILE *fp);
int readQuantum (FILE *fp, char* schedAlg);
process* readProcess(FILE *fp);
void printProcess(process *p);

// Function declarations: SJF.c
void fcfs(process **processesArray);
void sjf(process **processesArray);
void rr(process **processesArray);

#endif