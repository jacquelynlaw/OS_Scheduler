///////////////////////
// COP4600: Spring 2018
// Group 48:
// Brandon Cuevas
// Jacquelyn Law
// Lorraine Yerger
///////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 500

typedef struct process {
	char process_name[MAX_NAME_LENGTH];
	int arrival_time;
	int burst_length;
	struct process* next;
} process;

typedef struct LinkedList {
	process *head;
	process *tail;
} LinkedList;

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
	// new_process->process_name = name;
	new_process->arrival_time = arrival;
	new_process->burst_length = burst;
	
	return new_process;
}

// Create a new linked list
LinkedList *createList(void)
{
	LinkedList *list = malloc(sizeof(LinkedList));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

// Free a linked list
LinkedList *freeList(LinkedList *list)
{
	process *temp;
	
	// If it is already empty
	if (list == NULL)
		return NULL;
		
	while (list->head != NULL)
	{
		temp = list->head->next;
		free(list->head);
		list->head = temp;
	}
	
	free(list);
	
	return NULL;
}

// Add a process struct to the end of the linked list of processes
void addProcess(LinkedList *list, process* new_process)
{
	// If the list is empty, then set the new process equal to its head and tail
	if (list->head == NULL)
	{
		list->head = new_process;
		list->tail = new_process;
		return;
	}
	
	list->tail->next = new_process;
	list->tail = list->tail->next;
}

int main(void)
{	
	// fflush(stdin);
	return 0;
}