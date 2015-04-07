#include "ADT/LinkedList.h"

#define MEMORY_SIZE 128
#define MAX_USES 3

void firstFit(LinkedList *processHead);
void bestFit(LinkedList *processHead);
void nextFit(LinkedList *processHead);
void worstFit(LinkedList *processHead);
LinkedList * parse(LinkedList *processHead, char* fname);
Boolean isNumber(char *number);

int main(int argc, char **argv)
{
	LinkedList *processHead = NULL;
	char input;

	if(argc != 2)
	{
		printf("Invalid arguement size (should be only one, the file name)\n");
		exit(1);
	}
	processHead = parse(processHead, argv[1]);


	do
	{
		printf("\nMenu:\n");
		printf("Enter 1 to use first fit\n");
		printf("Enter 2 to use best fit\n");
		printf("Enter 3 to use next fit\n");
		printf("Enter 4 to use worst fit\n");
		printf("Enter q or Q to exit\n");

		input = getchar();
		printf("\n");

		if(getchar() != '\n')
		{
			while(getchar() != '\n') {}
			printf("Invalid input\n");
		}
		else if((input != 'q' && input != 'Q') && ((int)input > 52 || (int)input < 49))
		{
			printf("Invalid input\n");
		}
		else if(input == '1')
		{
			firstFit(processHead);
		}
		else if(input == '2')
		{
			bestFit(processHead);
		}
		else if(input == '3')
		{
			nextFit(processHead);
		}
		else if(input == '4')
		{
			worstFit(processHead);
		}
	} while(input != 'q' && input != 'Q');

	LL_deleteList(processHead);
	return 0;
}

void firstFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;

	int holeFlag = 0;
	int done = 0;

	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;

	int currentMem = 0;
	float avgMem = 0;

	int numOfHoles = 0;
	int avgHoles = 0;

	printf("First Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 0;
		done = 0;
		spaces = 0;

		if(readyProcesses->p->uses >= MAX_USES)
		{
			break;
		}

		for(i = 0; i < MEMORY_SIZE; i++)
		{
			if(memory[i] == 0)
			{
				spaces++;
			}
			else
			{
				spaces = 0;
			}

			if(!done)
			{
				if(spaces >= readyProcesses->p->size)
				{
					//space found, insert process
					readyProcesses->p->location = i + 1 - spaces;
					readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
					readyProcesses->p->uses++;

					numOfProcesses++;
					totalLoads++;
					currentMem += readyProcesses->p->size;
					
					for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
					{
						memory[i] = 1;
					}
					holeFlag = 0;
					for(i = 0; i < MEMORY_SIZE; i++)
					{
						if(memory[i] == 0)
						{
							if(holeFlag == 1)
							{
								//free mem started
								holeFlag = 2;
							}
						}
						else
						{
							if(holeFlag == 0)
							{
								//mem taken
								holeFlag = 1;
							}
							else if(holeFlag == 2)
							{
								//mem taken with gap
								numOfHoles++;
								holeFlag = 0;
							}
						}
					}

					processTemp = readyProcesses->next;
					readyProcesses->next = NULL;
					runningHead = LL_addToBack(runningHead, readyProcesses);
					spaces = 0;
					done = 1;
					break;
				}

				if(i == 127)
				{				
					//space not found, remove one process
					numOfProcesses--;
					currentMem -= runningHead->p->size;

					for(i = runningHead->p->location; i < runningHead->p->end; i++)
					{
						memory[i] = 0;
					}

					processTemp2 = runningHead;
					runningHead = runningHead->next;
					processTemp2->next = NULL;
					readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					i = -1;
					spaces = 0;
				}
			}
		}
		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		printf("Pid loaded, #processes = %d, #holes = %d, %%memusage = %.2f, cumulative %%mem = %.2f\n"
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads: %d, average #processes: %.2f, average #holes: %.2f, cumulative %%mem: %.2f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads: 0, average #processes: 0, average #holes: 0, cumulative %%mem: 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

void bestFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	int bestSpace = MEMORY_SIZE;
	int bestLoc = 0;

	int holeFlag = 0;
	int done = 0;

	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;

	int currentMem = 0;
	float avgMem = 0;

	int numOfHoles = 0;
	int avgHoles = 0;

	printf("Best Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 0;
		done = 0;
		spaces = 0;
		bestSpace = MEMORY_SIZE;
		bestLoc = 0;

		if(readyProcesses->p->uses >= MAX_USES)
		{
			break;
		}

		for(i = 0; i < MEMORY_SIZE; i++)
		{

			if(memory[i] == 0)
			{
				spaces++;
				if(i == 127)
				{
					if(spaces >= readyProcesses->p->size)
					{
						if(spaces < bestSpace)
						{
							bestSpace = spaces;
							bestLoc = i + 1 - spaces;
						}
						done = 1;
						spaces = 0;
					}
				}
			}
			else
			{
				if(spaces >= readyProcesses->p->size)
				{
					if(spaces < bestSpace)
					{
						bestSpace = spaces;
						bestLoc = i - spaces;
					}
					done = 1;
				}
				spaces = 0;
			}

			if(!done)
			{
				if(i == 127)
				{				
					//space not found, remove one process
					numOfProcesses--;
					currentMem -= runningHead->p->size;

					for(i = runningHead->p->location; i < runningHead->p->end; i++)
					{
						memory[i] = 0;
					}

					processTemp2 = runningHead;
					runningHead = runningHead->next;
					processTemp2->next = NULL;
					readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					i = -1;
					spaces = 0;
				}
			}
		}

		//space found, insert process
		readyProcesses->p->location = bestLoc;
		readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
		readyProcesses->p->uses++;

		numOfProcesses++;
		totalLoads++;
		currentMem += readyProcesses->p->size;
		
		for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
		{
			memory[i] = 1;
		}
		holeFlag = 0;
		for(i = 0; i < MEMORY_SIZE; i++)
		{
			if(memory[i] == 0)
			{
				if(holeFlag == 1)
				{
					//free mem started
					holeFlag = 2;
				}
			}
			else
			{
				if(holeFlag == 0)
				{
					//mem taken
					holeFlag = 1;
				}
				else if(holeFlag == 2)
				{
					//mem taken with gap
					numOfHoles++;
					holeFlag = 0;
				}
			}
		}
		processTemp = readyProcesses->next;
		readyProcesses->next = NULL;
		runningHead = LL_addToBack(runningHead, readyProcesses);

		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		printf("Pid loaded, #processes = %d, #holes = %d, %%memusage = %.2f, cumulative %%mem = %.2f\n"
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads: %d, average #processes: %.2f, average #holes: %.2f, cumulative %%mem: %.2f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads: 0, average #processes: 0, average #holes: 0, cumulative %%mem: 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

void nextFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	int lastPos = 0;

	int holeFlag = 0;
	int done = 0;
	int looped = 0;

	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;

	int currentMem = 0;
	float avgMem = 0;

	int numOfHoles = 0;
	int avgHoles = 0;

	printf("Next Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}
	i = 0;

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 0;
		done = 0;
		spaces = 0;

		if(readyProcesses->p->uses >= MAX_USES)
		{
			break;
		}

		i = lastPos;
		if(i == MEMORY_SIZE)
		{
			i = 0;
		}
		for(i = i; i < MEMORY_SIZE; i++)
		{
			if(i == 0)
			{
				looped = 1;
			}


			if(memory[i] == 0)
			{
				spaces++;
			}
			else
			{
				spaces = 0;
			}

			if(!done)
			{
				if(spaces >= readyProcesses->p->size)
				{
					//space found, insert process
					readyProcesses->p->location = i + 1 - spaces;
					readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
					lastPos = i;
					readyProcesses->p->uses++;

					numOfProcesses++;
					totalLoads++;
					currentMem += readyProcesses->p->size;
					
					for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
					{
						memory[i] = 1;
					}
					holeFlag = 0;
					for(i = 0; i < MEMORY_SIZE; i++)
					{
						if(memory[i] == 0)
						{
							if(holeFlag == 1)
							{
								//free mem started
								holeFlag = 2;
							}
						}
						else
						{
							if(holeFlag == 0)
							{
								//mem taken
								holeFlag = 1;
							}
							else if(holeFlag == 2)
							{
								//mem taken with gap
								numOfHoles++;
								holeFlag = 0;
							}
						}
					}

					processTemp = readyProcesses->next;
					readyProcesses->next = NULL;
					runningHead = LL_addToBack(runningHead, readyProcesses);
					spaces = 0;
					done = 1;
					break;
				}

				if(i == 127)
				{
					if(looped)
					{		
						//space not found, remove one process
						numOfProcesses--;
						currentMem -= runningHead->p->size;

						for(i = runningHead->p->location; i < runningHead->p->end; i++)
						{
							memory[i] = 0;
						}

						processTemp2 = runningHead;
						runningHead = runningHead->next;
						processTemp2->next = NULL;
						readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					}
					i = -1;
					spaces = 0;
				}
			}
		}
		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		printf("Pid loaded, #processes = %d, #holes = %d, %%memusage = %.2f, cumulative %%mem = %.2f\n"
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads: %d, average #processes: %.2f, average #holes: %.2f, cumulative %%mem: %.2f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads: 0, average #processes: 0, average #holes: 0, cumulative %%mem: 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

void worstFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	int worstSpace = 0;
	int worstLoc = 0;

	int holeFlag = 0;
	int done = 0;

	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;

	int currentMem = 0;
	float avgMem = 0;

	int numOfHoles = 0;
	int avgHoles = 0;

	printf("Worst Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 0;
		done = 0;
		spaces = 0;
		worstSpace = 0;
		worstLoc = 0;

		if(readyProcesses->p->uses >= MAX_USES)
		{
			break;
		}

		for(i = 0; i < MEMORY_SIZE; i++)
		{

			if(memory[i] == 0)
			{
				spaces++;
				if(i == 127)
				{
					if(spaces >= readyProcesses->p->size)
					{
						if(spaces > worstSpace)
						{
							worstSpace = spaces;
							worstLoc = i + 1 - spaces;
						}
						done = 1;
						spaces = 0;
					}
				}
			}
			else
			{
				if(spaces >= readyProcesses->p->size)
				{
					if(spaces > worstSpace)
					{
						worstSpace = spaces;
						worstLoc = i - spaces;
					}
					done = 1;
				}
				spaces = 0;
			}

			if(!done)
			{
				if(i == 127)
				{				
					//space not found, remove one process
					numOfProcesses--;
					currentMem -= runningHead->p->size;

					for(i = runningHead->p->location; i < runningHead->p->end; i++)
					{
						memory[i] = 0;
					}

					processTemp2 = runningHead;
					runningHead = runningHead->next;
					processTemp2->next = NULL;
					readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					i = -1;
					spaces = 0;
				}
			}
		}

		//space found, insert process
		readyProcesses->p->location = worstLoc;
		readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
		readyProcesses->p->uses++;

		numOfProcesses++;
		totalLoads++;
		currentMem += readyProcesses->p->size;
		
		for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
		{
			memory[i] = 1;
		}
		holeFlag = 0;
		for(i = 0; i < MEMORY_SIZE; i++)
		{
			if(memory[i] == 0)
			{	
				if(holeFlag == 1)
				{
					//free mem started
					holeFlag = 2;
				}
			}
			else
			{
				if(holeFlag == 0)
				{
					//mem taken
					holeFlag = 1;
				}
				else if(holeFlag == 2)
				{
					//mem taken with gap
					numOfHoles++;
					holeFlag = 0;
				}
			}
		}
		processTemp = readyProcesses->next;
		readyProcesses->next = NULL;
		runningHead = LL_addToBack(runningHead, readyProcesses);

		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		printf("Pid loaded, #processes = %d, #holes = %d, %%memusage = %.2f, cumulative %%mem = %.2f\n"
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads: %d, average #processes: %.2f, average #holes: %.2f, cumulative %%mem: %.2f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads: 0, average #processes: 0, average #holes: 0, cumulative %%mem: 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

LinkedList * parse(LinkedList *processHead, char* fname)
{
	char* line = NULL;
	char* token = NULL;
	FILE *fp = NULL;
	LinkedList *processTemp = NULL;

	fp = fopen(fname, "r");
	if(!fp)
	{
		printf("Error opening file\n");
		exit(1);
	}

	line = malloc(sizeof(char)*MEMORY_SIZE);

	while((fgets(line, MEMORY_SIZE, fp)) != NULL)
	{
		token = strtok(line, " ");
		if(token == NULL)
		{
			printf("Parse error\n");
			fclose(fp);
			free(line);
			exit(1);
		}
		processTemp = LL_create(token[0]);
		processHead = LL_addToBack(processHead, processTemp);

		token = strtok(NULL, "\n");
		if(token == NULL)
		{
			printf("Parse error\n");
			LL_deleteList(processHead);
			fclose(fp);
			free(line);
			exit(1);
		}
		if(!isNumber(token))
		{
			printf("Invalid number\n");
			LL_deleteList(processHead);
			fclose(fp);
			free(line);
			exit(1);
		}
		processTemp->p->size = atoi(token);
		if(processTemp->p->size < 1 || processTemp->p->size > MEMORY_SIZE)
		{
			printf("Invalid number\n");
			LL_deleteList(processHead);
			fclose(fp);
			free(line);
			exit(1);
		}

		processTemp->p->uses = 0;
		processTemp->p->location = -1;
		processTemp->p->end = -1;
	}

	fclose(fp);
	free(line);
	return processHead;
}

Boolean isNumber(char *number)
{
	int i = 0;

	for(i = 0; i < strlen(number); i++)
	{
		if(!isdigit(number[i]))
		{
			return false;
		}
	}
	return true;
}