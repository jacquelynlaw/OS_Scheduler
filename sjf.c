#include "header.h"

#define INT_MAX 2147483647
#define TERMINATED 2
#define PROCESS_IDLE -1
#define OUTPUT_FILE "processes.out"

// Functional Prototypes
int selectProcess(process **processesArray, int processCount);
void incrementTurnaroundTimes(int currentProcess, int *waitTimes, int processCount, process **processesArray);
void incrementWaitTimes(int currentProcess, int *waitTimes, int processCount, process **processesArray);

// Shortest Job First Scheduling
// Assumes processesArray is sorted by arrival time
// ### Prints process statistics in sorted order, NOT original order ###
void sjf(process **processesArray, schedparams *parameters)
{
	int processCount = parameters->processCount;
	int runTime = parameters->runTime;
	int i, t, nextArrivingProcess = 0, currentProcess = PROCESS_IDLE;
	int *waitTimes, *turnaroundTimes;

	FILE *ofp = fopen(OUTPUT_FILE, "w");

	// Track wait & turnaround times for each process
	waitTimes = calloc(processCount, sizeof(int));
	turnaroundTimes = calloc(processCount, sizeof(int));

	// Print output file headings
	fprintf(ofp, "%d processes\n", processCount);
	fprintf(ofp, "Using Shortest Job First (Pre)\n\n");

	// Increment through full run time
	for (t = 0; t < runTime; t++)
	{
		incrementTurnaroundTimes(currentProcess, turnaroundTimes, processCount, processesArray);
		incrementWaitTimes(currentProcess, waitTimes, processCount, processesArray);

		// If a process is running, decrement its burst_length and check for termination
		if (currentProcess != PROCESS_IDLE)
		{
			processesArray[currentProcess]->burst_length--;

			if (processesArray[currentProcess]->burst_length <= 0)
			{
				fprintf(ofp, "Time %d: %s finished\n", t, processesArray[currentProcess]->process_name);

				processesArray[currentProcess]->process_state = TERMINATED;
				currentProcess = PROCESS_IDLE;
			}
		}

		// Check for process arrival
		if (processesArray[nextArrivingProcess]->arrival_time == t)
		{
			// Set newly arrived process to READY
			processesArray[nextArrivingProcess]->process_state = READY;

			fprintf(ofp, "Time %d: %s arrived\n", t, processesArray[nextArrivingProcess]->process_name);

			// Assumes next process index is also next arrival
			if (nextArrivingProcess < processCount - 1)
			{
				nextArrivingProcess++;
			}

			// Select next process to run
			currentProcess = selectProcess(processesArray, processCount);
			if (currentProcess != PROCESS_IDLE)
			{
				fprintf(ofp, "Time %d: %s selected (burst %d)\n", t, processesArray[currentProcess]->process_name, processesArray[currentProcess]->burst_length);
			}
		}

		// If no process is running, select a new process or declare idle
		if (currentProcess == PROCESS_IDLE)
		{
			currentProcess = selectProcess(processesArray, processCount);

			if (currentProcess != PROCESS_IDLE)
			{
				fprintf(ofp, "Time %d: %s selected (burst %d)\n", t, processesArray[currentProcess]->process_name, processesArray[currentProcess]->burst_length);
			}
			else
			{
				fprintf(ofp, "Time %d: IDLE\n", t);
			}
		}

	}

	// Print final results
	fprintf(ofp, "Finished at time %d\n\n", t);

	for (i = 0; i < processCount; i++)
	{
		fprintf(ofp, "%s wait %d turnaround %d\n", processesArray[i]->process_name, waitTimes[i], turnaroundTimes[i]);
	}

	free(waitTimes);
	free(turnaroundTimes);

	fclose(ofp);
}

// Return index of process with smallest burst_length that is ready to run
int selectProcess(process **processesArray, int processCount)
{
	int i, selectedProcess = PROCESS_IDLE, min = INT_MAX;
	for (i = 0; i < processCount; i++)
	{
		if (processesArray[i]->process_state == READY)
		{
			if (processesArray[i]->burst_length < min)
			{
				selectedProcess = i;
				min = processesArray[i]->burst_length;
			}
		}
	}
	return selectedProcess;
}

// Increment turnaround times for each process
void incrementTurnaroundTimes(int currentProcess, int *turnaroundTimes, int processCount, process **processesArray)
{
	int i;
	for (i = 0; i < processCount; i++)
	{
		if (processesArray[i]->process_state == READY)
		{
			turnaroundTimes[i]++;
		}
	}
}

// Increment wait times for each process (except current process)
void incrementWaitTimes(int currentProcess, int *waitTimes, int processCount, process **processesArray)
{
	int i;
	for (i = 0; i < processCount; i++)
	{
		if (processesArray[i]->process_state == READY)
		{
			if (i != currentProcess)
			{
				waitTimes[i]++;
			}
		}
	}
}
