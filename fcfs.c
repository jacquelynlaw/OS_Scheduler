#include "header.h"

void fcfs(process **processesArray, schedparams *parameters)
{
	printf("Running FCFS from fcfs.c...!\n\n");
	
	FILE *fp = fopen("processes.out", "w");
	if(fp==NULL)
		printf("Error: file not found.\n");
	
	fputs("hello!\n", fp);
	
	return;
}