/*******************************************************************************
*
* CprE 308 Scheduling Lab
*
* scheduling.c
*******************************************************************************/

#define NUM_PROCESSES 20

struct process
{
  /* Values initialized for each process */
  int arrivaltime;  /* Time process arrives and wishes to start */
  int runtime;      /* Time process requires to complete job */
  int priority;     /* Priority of the process */

  /* Values algorithm may use to track processes */
  int starttime;
  int endtime;
  int flag;
  int remainingtime;
};

/* Forward declarations of Scheduling algorithms */
void first_come_first_served(struct process *proc);
void shortest_remaining_time(struct process *proc);
void round_robin(struct process *proc);
void round_robin_priority(struct process *proc);

int main()
{
  int i;
  struct process proc[NUM_PROCESSES],      /* List of processes */
                 proc_copy[NUM_PROCESSES]; /* Backup copy of processes */

  /* Seed random number generator */
  /*srand(time(0));*/  /* Use this seed to test different scenarios */
  srand(0xC0FFEE);     /* Used for test to be printed out */

  /* Initialize process structures */
  for(i=0; i<NUM_PROCESSES; i++)
  {
    proc[i].arrivaltime = rand()%100;
    proc[i].runtime = (rand()%30)+10;
    proc[i].priority = rand()%3;
    proc[i].starttime = 0;
    proc[i].endtime = 0;
    proc[i].flag = 0;
    proc[i].remainingtime = 0;
  }

  /* Show process values */
  printf("Process\tarrival\truntime\tpriority\n");
  for(i=0; i<NUM_PROCESSES; i++)
    printf("%d\t%d\t%d\t%d\n", i, proc[i].arrivaltime, proc[i].runtime,
           proc[i].priority);

  /* Run scheduling algorithms */
  printf("\n\nFirst come first served\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  first_come_first_served(proc_copy);

  printf("\n\nShortest remaining time\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  shortest_remaining_time(proc_copy);

  printf("\n\nRound Robin\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin(proc_copy);

  printf("\n\nRound Robin with priority\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin_priority(proc_copy);

  return 0;
}

void first_come_first_served(struct process *proc)
{
	/*counters*/
	int i, j;

	/*int to store running total of completion time*/
	int completiontime_running_total = 0;

	/*int to store average completion time*/
	int avg_completion_time;

	/*int to store system time*/
	int sys_time = 0;

	/*keep track of first come*/
	int first_come;

	/*loop through num processes to execute them*/
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		/*initialize first_come*/
		first_come = i;

		/*loop through and find first come*/
		for(j = 0; j < NUM_PROCESSES; j++)
		{
			/*if first_come is already complete (flag = 1) then 
 			* overwrite it*/
			if(proc[first_come].flag)
			{
				first_come = j;
			}

			/*if first_come arrival time is after current process 
 			* and current process hasn't been executed (flag = 0) 
 			* then overwrite it*/
			else if(proc[first_come].arrivaltime > 
				proc[j].arrivaltime && !proc[j].flag)
			{
				first_come = j;
			}
		}

		/*advance system time if it hasn't caught up to arrival time*/
		if(sys_time < proc[first_come].arrivaltime)
			sys_time = proc[first_come].arrivaltime;

		/*execute process*/
		/*set first_come start time*/
		proc[first_come].starttime = sys_time;

		/*advance system time*/
		sys_time += proc[first_come].runtime;

		/*set first_come end time*/
		proc[first_come].endtime = sys_time;

		/*keep track of completion time running total*/
		completiontime_running_total += (proc[first_come].endtime - 
			proc[first_come].arrivaltime);

		/*mark first_come as completed*/
		proc[first_come].flag = 1;

		/*print process star and finish*/
		printf("Process %d started at time %d\n", first_come, 
			proc[first_come].starttime);
		printf("Process %d finished at time %d\n", first_come, 
			proc[first_come].endtime);
	}

	/*calculate average completion time*/
	avg_completion_time = completiontime_running_total/NUM_PROCESSES;

	/*print out average arrival to finish time*/
	printf("Average time from arrival to completion is %d seconds\n", 
		avg_completion_time);
}

void shortest_remaining_time(struct process *proc)
{
	/*counters*/
	int i, j;

	/*int to store running total of completion time*/
	int completiontime_running_total = 0;

	/*int to store average completion time*/
	int avg_completion_time;

	/*int to store system time*/
	int sys_time = 0;

	/*keep track of first come*/
	int shortest_remaining_time;

	/*loop through num processes to execute them*/
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		/*initialize shortest_remaining_time*/
		shortest_remaining_time = -1;

		/*loop through and find shortest remaining time*/
		for(j = 0; j < NUM_PROCESSES; j++)
		{
			/*if shortest_remaining time hasn't been set, the 
			* current process has arrived (arrival time <= system 
			* time) then overwrite it, and the current process 
			* hasn't finished*/
			if(shortest_remaining_time < 0 && proc[j].arrivaltime 
				<= sys_time && !proc[j].flag)
			{
				shortest_remaining_time = j;
			}

			/*else if shortest remaining time has been set, the 
			current process has arrived, the current process 
			runtime is shorter than shortest remaining time, and 
			current process hasn't finished than overwrite it*/
			else if(shortest_remaining_time >= 0 && 
				proc[j].arrivaltime <= sys_time && 
				proc[j].runtime < 
				proc[shortest_remaining_time].runtime && 
				!proc[j].flag)
			{
				shortest_remaining_time = j;
			}
		}

		/*if no process was found advance system time and continue*/
		if(shortest_remaining_time < 0)
		{
			sys_time++;
			i--;
			continue;
		}

		/*execute process*/
		/*set shortest_remaining_time start time*/
		proc[shortest_remaining_time].starttime = sys_time;

		/*advance system time*/
		sys_time += proc[shortest_remaining_time].runtime;

		/*set shortest_remaining_time end time*/
		proc[shortest_remaining_time].endtime = sys_time;

		/*keep track of completion time running total*/
		completiontime_running_total += (
			proc[shortest_remaining_time].endtime - 
			proc[shortest_remaining_time].arrivaltime);

		/*mark shortest_remaining_time as completed*/
		proc[shortest_remaining_time].flag = 1;

		/*print process star and finish*/
		printf("Process %d started at time %d\n", 
			shortest_remaining_time, 
			proc[shortest_remaining_time].starttime);
		printf("Process %d finished at time %d\n", 
			shortest_remaining_time, 
			proc[shortest_remaining_time].endtime);
	}

	/*calculate average completion time*/
	avg_completion_time = completiontime_running_total/NUM_PROCESSES;

	/*print out average arrival to finish time*/
	printf("Average time from arrival to completion is %d seconds\n", 
		avg_completion_time);
}

void round_robin(struct process *proc)
{
	/*counters*/
	int i, j = 0;

	/*int to store which process id we searched first at current system 
	* time*/
	int start_j = 0;

	/*int to store running total of completion time*/
	int comptime_running_total = 0;

	/*int to store average completion time*/
	int avg_completion_time;

	/*int to store system time*/
	int sys_time = 0;

	/*int that is 0 until a job completes*/
	int proc_finished;

	/*loop through till all processes have completed*/
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		/*initialize proc_finished*/
		proc_finished = 0;

		while(!proc_finished)
		{
			/*if proc[j] has arrived and has not completed then run 
			* it for 1 second*/
			if(proc[j].arrivaltime <= sys_time && proc[j].flag != 2)
			{
				/*if proc[j] just started running initialize
				* it*/
				if(!proc[j].flag)
				{
					proc[j].flag = 1; /*process started*/
					proc[j].starttime = sys_time;
					proc[j].remainingtime = proc[j].runtime 
						- 1;
				}

				/*else update process*/
				else
				{
					proc[j].remainingtime--;

					/*if proc is finished update proc*/
					if(!proc[j].remainingtime)
					{
						/*process has completed*/
						proc[j].flag = 2;
						proc[j].endtime = sys_time + 1;
						proc_finished = 1;
						comptime_running_total += 
							(proc[j].endtime - 
							proc[j].arrivaltime);
						printf("Process %d started at " 
							"time %d\n", j, 
							proc[j].starttime);
						printf("Process %d finished " 
							"at time %d\n", j, 
							proc[j].endtime);
					}
				}

				/*update j and increment system time*/
				j = (j < (NUM_PROCESSES-1)) ? (j+1) : 0;
				sys_time++;
				start_j = j;
			}
			/*if proc[j] can't be ran*/
			else
			{
				/*update j*/
				j = (j < (NUM_PROCESSES-1)) ? (j+1) : 0;
				/*if j = start_j then increment system time 
				* since no process could be run at this time*/
				if(j == start_j)
					sys_time++;
			}
		}
	}

	/*calculate average completion time*/
	avg_completion_time = comptime_running_total/NUM_PROCESSES;

	/*print out average arrival to finish time*/
	printf("Average time from arrival to completion is %d seconds\n", 
		avg_completion_time);
}

void round_robin_priority(struct process *proc)
{
	/*counters*/
	int i, j = 0;

	/*int to store which process id we searched first at current system 
	* time*/
	int start_j = 0;

	/*int to store running total of completion time*/
	int comptime_running_total = 0;

	/*int to store average completion time*/
	int avg_completion_time;

	/*int to store system time*/
	int sys_time = 0;

	/*int that is 0 until a job completes*/
	int proc_finished;

	/*int that stores highest priority*/
	int high_p;

	/*int that stores last executed process*/
	int last_executed = 0;

	/*loop through till all processes have completed*/
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		/*initialize proc_finished*/
		proc_finished = 0;

		while(!proc_finished)
		{
			/*initialize high_p*/
			high_p = -1;

			do {
			/*if proc[j] has arrived, is not done, and highest 
			* priority has not been set, set highest priority*/
			if(proc[j].arrivaltime <= sys_time && high_p < 0 
				&& proc[j].flag < 2)
			{
				high_p = j;
			}

			/*if proc[j] has arrived, is not done, and is of higher 
			*priority than highest priority, update high_p*/
			else if(proc[j].arrivaltime <= sys_time && 
				proc[j].flag < 2 && proc[j].priority > 
				proc[high_p].priority)
			{
				high_p = j;
			}
			j = (j < (NUM_PROCESSES-1)) ? (j+1) : 0;
			}while(j != start_j);

			/*if highest_priority has been set execute*/
			if(high_p > -1)
			{
				last_executed = high_p;
				/*if high_p just started running 
				* initialize it*/
				if(!proc[high_p].flag)
				{
					/*process started*/
					proc[high_p].flag = 1;
					proc[high_p].starttime = 
						sys_time;
					proc[high_p].remainingtime = 
						proc[high_p].runtime - 1;
				}

				/*else update process*/
				else
				{
					proc[high_p].remainingtime--;

					/*if proc is finished update proc*/
					if(!proc[high_p].remainingtime)
					{
					/*process has completed*/
					proc[high_p].flag = 2;
					proc[high_p].endtime = sys_time + 1;
					proc_finished = 1;
					comptime_running_total += (
						proc[high_p].endtime - 
						proc[high_p].arrivaltime);
					printf("Process %d started at " 
						"time %d\n", high_p, 
						proc[high_p].starttime);
					printf("Process %d finished " 
						"at time %d\n", high_p, 
						proc[high_p].endtime);
					}
				}
			}
			/*increment system time and set j to search from current
			* highest priority job*/
			sys_time++;
			if(last_executed == NUM_PROCESSES - 1)
				start_j = j = 0;
			else
				start_j = j = (last_executed + 1);
		}
	}

	/*calculate average completion time*/
	avg_completion_time = comptime_running_total/NUM_PROCESSES;

	/*print out average arrival to finish time*/
	printf("Average time from arrival to completion is %d seconds\n", 
		avg_completion_time);
}

