// OS Group 48: Lorraine Yerger, Jackie Law, Brandon Cuevas
// PA 1

#include "header.h"

int main(void)
{

    // File name to read from
    FILE *fp = fopen("set3_process.in", "r");

    if(fp==NULL)
    	printf("ERROR: File pointer is null\n");

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
		//printProcess(processesArray[i]);
	}

	// Sort array of processes by arrival time
	processesArray = sortProcesses(processesArray, parameters);
  
	// Run proper algorithm on array of processes (each contained in own .c file)
	if(strcmp(parameters->schedAlg, "fcfs")==0)
	{
		// RUN ALGORITHM
		fcfs(processesArray, parameters);
	}
	else if(strcmp(parameters->schedAlg, "sjf")==0)
	{
		// RUN ALGORITHM
		sjf(processesArray, parameters);
	}
	else if(strcmp(parameters->schedAlg, "rr")==0)
	{
		// RUN ALGORITHM
		rr(processesArray, parameters);
	}
	else
		printf("Error: No algorithm recognized to run.\n");


    return 0;
}

// For debugging purposes
void printProcess(process *p)
{
	printf("Process %s: arrival = %d, burst = %d\n", p->process_name, p->arrival_time,
				p->burst_length);
	return;
}

// Read line containing process count from input, parse and return the integer value
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

    return processCount;
}

// Read line containing run time from input, parse and return the integer value
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
    return runTime;
}

// Read line containing Alg Type from input, parse and return a string
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

	return schedAlg;
}

// If RR, read line containing quantum from input, parse and return the integer value
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
	return quantum;
}

// Read one line containing process name and description, parse data values
// Return a process struct with filled parameters
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
	new_process->process_state = NOTARRIVED;

	new_process->wait_time = 0;
	new_process->end_time = 0;
	
	return new_process;
}

void swap (process *a, process *b)
{
	process temp = *a;
	*a = *b;
	*b = temp;
}

process **sortProcesses(process **processesArray, schedparams *parameters)
{
	int i, j;
	for (i = 0; i < (parameters->processCount) - 1; i++)
	{
		for (j = 0; j < (parameters->processCount - i) - 1; j++)
		{
			if (processesArray[j]->arrival_time > processesArray[j+1]->arrival_time)
				swap(processesArray[j], processesArray[j+1]);
		}
	}

	return processesArray;
}
