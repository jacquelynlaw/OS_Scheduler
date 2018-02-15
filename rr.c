#include "header.h"
#define OUTPUT_FILE "processes.out"

// Node struct for a linked list to implement queue
typedef struct node {
	process *proc_node;
	struct node *next;
} node;

// Global variables
node *front = NULL;
node *rear = NULL;
FILE *ofp = NULL;

// For queue implementation: creates a node that linkes processes
node* createNode(process *proc)
{
	proc->process_state = READY;
	node *new_node = malloc(sizeof(node));
	new_node->proc_node = proc;
	new_node->next = NULL;
	return new_node;
}

// Enqueue a process
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

// Dequeue a process
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

// For debugging
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

// Searches for all processes that have arrived
void checkArrivals(int time, process **processesArray, schedparams *parameters)
{	
	for (int i = 0; i < parameters->processCount; i++)
	{
		if (processesArray[i]->arrival_time == time)
		{
			printArrival(time, processesArray[i]->process_name);
			processesArray[i]->checkin_time = time;
			processesArray[i]->end_time = processesArray[i]->burst_length;
			// Enqueue process
			enqueue(processesArray[i]);
		}
	}
}

// Selects process at front of the queue and prints corresponding message
process* selectProcess(int time)
{
 	process* selected_proc = dequeue();
 	
 	if (selected_proc == NULL)
 	{
 		return NULL;
 	}
 	
 	// Set state of process to running
 	selected_proc->process_state = RUNNING;
 	
 	// Calculate wait time using the current time and the time it was last enqueued
 	selected_proc->wait_time += (time - selected_proc->checkin_time);
 	fprintf(ofp, "Time %d: %s selected (burst %d)\n", time, selected_proc->process_name, selected_proc->burst_length);
 	return selected_proc;
}

void processFinished(int time, process *proc)
{
	fprintf(ofp, "Time %d: %s finished\n", time, proc->process_name);
}

void printWaitTimes(process **processesArray, int numProcesses)
{
	for (int i = 0; i < numProcesses; i++)
	{
		fprintf(ofp, "%s wait %d turnaround %d\n", processesArray[i]->process_name, processesArray[i]->wait_time, processesArray[i]->end_time);
	}
}

// Frees dynamically allocated linked list
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
			if (current_process != NULL)
			{
				// Decrease corresponding process' burst length
				current_process->burst_length -= 1;
				time++;
				
				// Check for arriving processes
				checkArrivals(time, processesArray, parameters);
			}
		
			// If current process is null, then no processes are ready to run, so scheduler is idling
			// End quantum
			else
			{	
				fprintf(ofp, "Time %d: IDLE\n", time);
				
				time++;
				// Check for arriving processes
				checkArrivals(time, processesArray, parameters);
				interval = quantum;
			}
			
			// If process finishes before quantum finishes, then end quantum
			if (current_process != NULL && current_process->burst_length == 0)
			{
				processFinished(time, current_process);
				current_process->end_time += current_process->wait_time;
				interval = quantum;
			}
		}
		
		// If process is unfinished, then enqueue and mark check in time
		if (current_process != NULL && current_process->burst_length > 0)
		{
			current_process->checkin_time = time;
			enqueue(current_process);
		}
	} while (time < parameters->runTime);
	
	fprintf(ofp, "Finished at time %d\n\n", time);
	
	// Print waiting and turnaround times
	printWaitTimes(processesArray, parameters->processCount);

	freeList();
	fclose(ofp);
}