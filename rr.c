#include "header.h"
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

void printArrival(int time, char* process_name)
{
	fprintf("Time %d: %s arrived", time, process_name);
}

void checkArrivals(int time, process **processesArray, schedparams *parameters)
{
	// Look to see if any processes have arrived at the current time
	printf("Process Count = %d", parameters->processCount);
	
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
 	fprintf("Time %d: %s selected (burst %d)", time, selected_proc->process_name, selected_proc->burst_length);
 	return selected_proc;
}

void processFinished(int time, process *proc)
{
	fprintf("Time %d: %s finished", time, proc->process_name);
}

void rr(process **processesArray, schedparams *parameters)
{
	printf("Running RR from rr.c...\n");	
	
	int quantum = parameters->quantum;
	process *current_process;
	int interval = 1;
	
	// Loop for all of the runtime
	for (int time = 0; time < parameters->runTime; time++)
	{
		// D E B U G G I N G
		printf("\n\t BUG IN checkArrivals\n");
		
		// See if any processes have arrived at the current time
		checkArrivals(time, processesArray, parameters);
		
		// D E B U G G I N G
		printf("\n\t BUG IN selectProcess\n");
		
		
		// Select a process, and print
		current_process = selectProcess(time);
		
		interval = 1;
		
		// Starts at 1, starting at 0 would be the redundant
		for (interval = 1; interval < quantum; interval++)
		{
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
}