#include "header.h"
#define OUTPUT_FILE "processes.out"

// Struct declarations
/*
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
	int process_state;
} process;
*/
	
// Node struct for a linked list to implement queue
typedef struct node {
	process *proc_node;
	struct node *next;
} node;

// Global variables
node *front = NULL;
node *rear = NULL;
FILE *ofp = NULL;

node* createNode(process *proc)
{
	proc->process_state = READY;
	node *new_node = malloc(sizeof(node));
	new_node->proc_node = proc;
	new_node->next = NULL;
	return new_node;
}

// Enqueue
void enqueue(process *proc)
{
	node *new_node = createNode(proc);
	
	if (front == NULL && rear == NULL)
	{
		front = rear = new_node;
		return;
	}
	
	rear->next = new_node;
	rear = new_node;
}

// Dequeue
process* dequeue(void)
{
	node *temp = front;
	process *proc = NULL;
	if (front == NULL)
	{
		return NULL;
	}
	if (front == rear)
	{
		front = rear = NULL;
	}
	
	else
	{
		front = front->next;
	}

	proc = temp->proc_node;
	free(temp);
	return proc;
}

void printQueue()
{
	printf("==== Q U E U E ====\n");
	node *temp = front;
	while (temp != NULL)
	{
		printf("Process Name: %s\n", temp->proc_node->process_name);
		printf("\tBurst: %d\n", temp->proc_node->burst_length);
		temp = temp->next;
	}
	printf("===================\n");
}

void printArrival(int time, char* process_name)
{
	fprintf(ofp, "Time %d: %s arrived\n", time, process_name);
}

void checkArrivals(int time, process **processesArray, schedparams *parameters)
{	
	for (int i = 0; i < parameters->processCount; i++)
	{
		if (processesArray[i]->arrival_time == time)
		{
			printArrival(time, processesArray[i]->process_name);
			// Enqueue process
			enqueue(processesArray[i]);
		}
	}
}

process* selectProcess(int time)
{
 	process* selected_proc = dequeue();
 	
 	if (selected_proc == NULL)
 	{
 		return NULL;
 	}
 	
 	selected_proc->process_state = RUNNING;
 	fprintf(ofp, "Time %d: %s selected (burst %d)\n", time, selected_proc->process_name, selected_proc->burst_length);
 	return selected_proc;
}

void processFinished(int time, process *proc)
{
	fprintf(ofp, "Time %d: %s finished\n", time, proc->process_name);
}

void freeList(void)
{
	node *temp;
	
	// If it is already empty
	if (front == NULL)
		return;
		
	while (front != NULL)
	{
		temp = front->next;
		free(front);
		front = temp;
	}
	
	free(front);
	
	if (rear != NULL)
		free(rear);
}

void rr(process **processesArray, schedparams *parameters)
{
	printf("Running RR from rr.c...\n");	
	int quantum = parameters->quantum;
	process *current_process;
	int interval = 0;
	int time = 0;
	
	ofp = fopen(OUTPUT_FILE, "w");
	if(ofp == NULL)
	{
		printf("Error: file not found.\n");
	}
	
	fprintf(ofp, "%d Processes\n", parameters->processCount);
	fprintf(ofp, "Using Round-Robin\n");
	fprintf(ofp, "Quantum %d\n\n", quantum);
	
	// Check if any processes have arrived, if so enqueue
	checkArrivals(time + interval, processesArray, parameters);
		
	do {
		// Select a process to run
		current_process = selectProcess(time);
		
		// Run that process for a quantum
		for (interval = 0; interval < quantum; interval++)
		{
			if (current_process == NULL)
			{
				fprintf(ofp, "Time %d: IDLE\n", time);
				// Check for arriving processes
				checkArrivals(time, processesArray, parameters);
				time++;
				interval = quantum;
			}
		
			else
			{
				current_process->burst_length -= 1;
				time++;
				// Check for arriving processes
				checkArrivals(time, processesArray, parameters);
			}
			
			// If process finishes before quantum finishes, then end quantum
			if (current_process != NULL && current_process->burst_length == 0)
			{
				processFinished(time, current_process);
				interval = quantum;
			}
		}
		
		// If process is unfinished, then enqueue
		if (current_process != NULL && current_process->burst_length > 0)
		{
			enqueue(current_process);
		}
		// Update time
		// time += interval;
	} while (time < parameters->runTime);
	
	fprintf(ofp, "Finished at time %d\n", time);
	/*
	// Check if any processes have arrived, if so, enqueue
	checkArrivals(time, processesArray, parameters);
	// D E B U G G I N G
	printQueue();
	// Select a process to run
	// current_process = selectProcess(interval + time);
	// printf("Current process: %s\n", current_process->process_name);
	
	
	while (time <= (parameters->runTime) - quantum)
	{
		// Check if any processes have arrived, if so, enqueue
		// checkArrivals(time, processesArray, parameters);
		// D E B U G G I N G
		// printQueue();
		// Select a process to run
		current_process = selectProcess(interval + time);
		printf("\nCurrent process: %s\n", current_process->process_name);
		
		// Run process for a time slice
		for (interval = 0; interval < quantum; interval++)
		{
			printf("~~~~~~~~~~~~~~T I M E~~~~~~~~~~~~~~~: %d\n", time + interval);
			
			// Check for any processes' arrivals during time slice
			if (interval != 0)
			{
				checkArrivals(time + interval, processesArray, parameters);
				current_process->burst_length -= 1;
				// printQueue();
			}
			
			// If process is finished, end time slice
			if (current_process->burst_length == 0)
			{
				processFinished(time + interval, current_process);
				interval = quantum;
			}
		}
		
		// Enqueue process if it is unfinished after time slice
		printf("\n******************\nCurrent process: %s\n", current_process->process_name);
		if (current_process->burst_length > 0)
		{
			printf("Remaining burstlength: %d\n", current_process->burst_length);
			enqueue(current_process);
		}
		printf("******************\n\n");
		printQueue();
		time = time + interval;
	}
	
	*/
	/*
	// Loop for all of the runtime
	for (int time = 0; time < parameters->runTime; time++)
	{
		// D E B U G G I N G
		// fprintf(ofp, "\n\t BUG IN checkArrivals\n");
		
		// See if any processes have arrived at the current time
		checkArrivals(time, processesArray, parameters);
		
		// D E B U G G I N G
		// fprintf(ofp, "\n\t BUG IN selectProcess\n");
		// D E B U G G I N G 
		fprintf(ofp, "\t T I M E: %d\n", time);
		
		// Select a process, and print
		
		// D E B U G G I N G 
		// fprintf(ofp, "\tTHIS one\n");
		current_process = selectProcess(time);
		
		interval = 1;
		
		// Starts at 1, starting at 0 would be the redundant
		for (interval = 1; interval <= quantum; interval++)
		{
			// D E B U G G I N G 
			fprintf(ofp, "\t ~T I M E: %d\n", time + interval);
			
			// Decrease burst length
			current_process->burst_length -= 1;
			
			// Check if process is finished
			if (current_process->burst_length == 0)
			{
				processFinished(time + interval, current_process);
				interval = quantum;
			}
			// Check for any arriving processes 
			checkArrivals(time + interval, processesArray, parameters);
			
		}
		// Enqueue process if it is unfinished
		if (current_process->burst_length > 0)
		{
			enqueue(current_process);
		}
		
		// Change current time according to amount of interval passed
		time = time + interval;
	}
	*/

	freeList();
	fclose(ofp);
}