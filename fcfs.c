#include "header.h"

void fcfs(process **processesArray, schedparams *parameters)
{
	printf("Running FCFS from fcfs.c...\n\n");
	
	FILE *fp = fopen("processes.out", "w");
	if(fp==NULL)
		printf("Error: file not found.\n");
	
	fprintf(fp, "%d Processes\n", parameters->processCount);
	fprintf(fp, "Using First Come First Served\n\n");
	
	int time;
	int indexOfNextArriving=0;
	int allProcessesArrived = 0;
	int indexOfProcessToRun = 0; // bc processessArray is sorted by arrival time
	int processStartTime = 0;
	
	for(time=0; time<parameters->runTime; time++)
	{
		// --- CHECK FOR ARRIVING PROCESSES ---
		// Are there processes left to arrive?
		if(!allProcessesArrived)
		{
			// Did any process just arrive?
			if(processesArray[indexOfNextArriving]->arrival_time == time)
			{
				// Write to Output
				fprintf(fp, "Time %d: %s arrived\n", time, processesArray[indexOfNextArriving]->process_name);
				processesArray[indexOfNextArriving]->process_state = READY;
				indexOfNextArriving++;

				
				// Was this the last process to arrive? 
				if(indexOfNextArriving==parameters->processCount)
					allProcessesArrived = 1;
			}
		}
		
		// --- CHECK WHICH PROCESS TO RUN ---
		// Are we currently running a process?
		if(processesArray[indexOfProcessToRun]->process_state == RUNNING)
		{
			// Did the process just finish?
			if( (time-processStartTime) == (processesArray[indexOfProcessToRun]->burst_length) )
			{
				// Print finished message, set state to READY, increase indexOfProcessToRun
				fprintf(fp, "Time %d: %s finished\n", time, processesArray[indexOfProcessToRun]->process_name);
				processesArray[indexOfProcessToRun]->process_state = READY;
				
				// Capture the time it finished
				processesArray[indexOfProcessToRun]->end_time = time;

				indexOfProcessToRun++;
			}
			
			// Or, is process still running? If so, do nothing.
			else
				;
		}
			;
		// Else, if we need to start a new process, has p[indexOfProcessToRun] arrived?
		if(processesArray[indexOfProcessToRun]->process_state == READY)
		{
			// Start running this process
			processesArray[indexOfProcessToRun]->process_state = RUNNING;
			processStartTime=time;
			fprintf(fp, "Time %d: %s selected (burst %d)\n", time, processesArray[indexOfProcessToRun]->process_name,
							processesArray[indexOfProcessToRun]->burst_length);
			// Capture how long that process was waiting before running
			processesArray[indexOfProcessToRun]->wait_time = time - processesArray[indexOfProcessToRun]->arrival_time;
		}
		
	}
	// Check if last process running finished at end of runTime
	if( (time-processStartTime) == (processesArray[indexOfProcessToRun]->burst_length) )
	{
		fprintf(fp, "Time %d: %s finished\n", time, processesArray[indexOfProcessToRun]->process_name);
		processesArray[indexOfProcessToRun]->end_time = time;
	}
	
	fprintf(fp, "Finished at time %d\n\n", time);
	
	// Print wait times and turnaround times
	for(int i=0; i<parameters->processCount; i++)
	{
		fprintf(fp, "%s wait %d turnaround %d\n", processesArray[i]->process_name, 
					processesArray[i]->wait_time, 
					processesArray[i]->end_time - processesArray[i]->arrival_time);
	}
	
	return;
}