// Lorraine Yerger
// PA 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 300
#define NAMESIZE 50

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
	struct process* next;
} process;

process *createProcess(char* name, int arrival, int burst);
int readProcessCount(FILE *fp);
int readRunTime(FILE *fp);
char* readAlgType(FILE *fp);
int readQuantum (FILE *fp, char* schedAlg);
process* readProcess(FILE *fp);
void printProcess(process *p);

int main(void)
{    
    
    // File name to read from
    FILE *fp = fopen("set2_process.in", "r");
    if(fp==NULL)
    	printf("ERROR\n");
    
    
    // Use struct to capture input fields
    schedparams *parameters = malloc(sizeof(schedparams));
    
    parameters->processCount = readProcessCount(fp);
    parameters->runTime = readRunTime(fp);
    parameters->schedAlg = readAlgType(fp);
    parameters->quantum = readQuantum(fp, parameters->schedAlg);
    
	// Read in all processes. Fill processesArray with pointers to process structs
	process **processesArray = malloc( sizeof(process)*(parameters->processCount+1) );
	
	for(int i=0; i<parameters->processCount; i++)
	{
		processesArray[i] = readProcess(fp);
		printProcess(processesArray[i]);		
	}
	
	// TO COMPLETE - Run proper algorithm on array of processes
	if(strcmp(parameters->schedAlg, "fcfs")==0)
	{
		// RUN ALGORITHM
		printf("Running FCFS...\n");
	}
	else if(strcmp(parameters->schedAlg, "sjf")==0)
	{
		// RUN ALGORITHM
		printf("Running SJF...\n");
	}
	else if(strcmp(parameters->schedAlg, "rr")==0)
	{
		// RUN ALGORITHM
		printf("Running RR...\n");
	}
	else
		printf("Error: No algorithm recognized to run.\n");
	
	    
    return 0;
}

void printProcess(process *p)
{
	printf("Process %s: arrival = %d, burst = %d\n", p->process_name, p->arrival_time, 
				p->burst_length); 
	return;
}

int readProcessCount(FILE *fp)
{
	// Delimiter chars to tokenize input
    const char delim[4] = " \n\t";	
    char* token;
    char buffer[BUFFSIZE];
	int processCount;
    
    // Read process count
    fgets(buffer, BUFFSIZE, fp);
    // on each line, divide into words by " ", tabs, or \n
    token = strtok(buffer, delim);
    if(strcmp("processcount", token)==0)
    {
    	token = strtok(NULL, delim);
    	processCount = atoi(token);
    }
    
    printf("Process count = %d\n", processCount);
    return processCount;
}

int readRunTime(FILE *fp)
{
	// Delimiter chars to tokenize input
    const char delim[4] = " \n\t";	
    char* token;
    char buffer[BUFFSIZE];
	int runTime;
	
	fgets(buffer, BUFFSIZE, fp);
	
	token = strtok(buffer, delim);
    if(strcmp("runfor", token)==0)
    {
    	token = strtok(NULL, delim);
    	runTime = atoi(token);
    }
    printf("Run time = %d\n", runTime);
    return runTime;
}

char* readAlgType(FILE *fp)
{
    const char delim[4] = " \n\t";	
    char* token;
    char buffer[BUFFSIZE];
    char* schedAlg = malloc(6);
    if(!schedAlg)
    	exit(1);

    fgets(buffer, BUFFSIZE, fp);
    token = strtok(buffer, delim);
    if(strcmp("use", token)==0)
    {
    	token = strtok(NULL, delim);
    	strcpy(schedAlg, token);
    }
	printf("Scheduling Alg = %s\n", schedAlg);
	
	return schedAlg;
}

int readQuantum (FILE *fp, char* schedAlg)
{
	const char delim[4] = " \n\t";	
    char* token;
    char buffer[BUFFSIZE];
	int quantum = -1;
	
	fgets(buffer, BUFFSIZE, fp);

	// If it's round robin, read in quantum. Otherwise, leave quantum = -1
	if(strcmp(schedAlg, "rr")==0)
	{
		token = strtok(NULL, delim);
		quantum = atoi(token);
	}
	printf("Quantum = %d\n", quantum);
	return quantum;
}

process* readProcess(FILE *fp)
{
	// Delimiter chars to tokenize input
    const char delim[4] = " \n\t";	
    char* token;
    char buffer[BUFFSIZE];
    char pName[NAMESIZE];
    int arrival;
    int burst;
    
    fgets(buffer, BUFFSIZE, fp);
    
    token = strtok(buffer, delim);	// token = process
    token = strtok(NULL, delim);	// token = name
    if(strcmp("name", token) == 0)
    {
    	token = strtok(NULL, delim);	// token = PA1
    	strcpy(pName, token);			// pName = PA1
	}
	    
    token = strtok(NULL, delim);	// and repeat...
    if(strcmp("arrival", token)==0)
    {
    	token = strtok(NULL, delim);
    	arrival = atoi(token);
    }
    
    token = strtok(NULL, delim);
    if(strcmp("burst", token)==0)
    {
    	token = strtok(NULL, delim);
    	burst = atoi(token);
    }
    
    return createProcess(pName, arrival, burst);
}

// Create a new process struct 
process *createProcess(char* name, int arrival, int burst)
{
	process *new_process = malloc(sizeof(process));
	
	if (new_process == NULL)
	{
		fprintf(stderr, "Error: Out of memory in creating new process struct\n");
		exit(1);
	}
	
	strcpy(new_process->process_name, name);
	new_process->arrival_time = arrival;
	new_process->burst_length = burst;
	
	return new_process;
}