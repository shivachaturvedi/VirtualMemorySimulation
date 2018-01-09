#include<stdio.h>
#include<malloc.h>
#include<unistd.h>

int** makeMemory(int memorySize);
void printMemory(int **mainMemory,int memorySize);

int countFree(int **memory,int memorySize){

	int count = 0;
	int i = 0;
	for(i = 0; i<memorySize; i++){
		
		if(memory[i][2] == 0)
			++count;
	}
return count;
}
int** changeSize(int oldSize,int newSize,int **oldMemory){
	
	int **newMemory = makeMemory(newSize);
	int i = 0;
	for(i = 0; i<oldSize; i++){
		
		newMemory[i][1] = oldMemory[i][1];
		newMemory[i][2] = oldMemory[i][2];
	}
printf("\n\t\t\t Change Size Called\n\n");
printMemory(newMemory,newSize);

return newMemory;
}
void claimMemory(int pID,int **memory,int memorySize){

	int i = 0;
	for(i = 0; i<memorySize; i++){
		
		if(memory[i][1] == pID){
			
			memory[i][1] = -1;
			memory[i][2] = 0;
		}
	}
}
long long int genAdd(int frameNo){
	
	return frameNo*1024;
}
int** makeMemory(int memorySize){
	int **mainMemory = NULL;
	int i = 0;
	int j = 0;
	mainMemory = (int **)calloc(memorySize,sizeof(int*));
	for(i=0; i<memorySize; i++){
		mainMemory[i] = (int*)calloc(4,sizeof(long long int));
	}
	for(i=0; i<memorySize; i++){
		mainMemory[i][0] = genAdd(i);
		mainMemory[i][1] = -1;
		mainMemory[i][2] = 0;
	}

// Code to ensure if Memory is made

printf("\n  Frame Add\tAlloted Process\t  Occupied bit\n\n");
//sleep(3);
for(i=0; i<memorySize; i++){
	if(i==0){
		printf("      %d\t\t    %d\t\t      %d\n",mainMemory[i][0],mainMemory[i][1],mainMemory[i][2]);
	}else{
		printf("    %d\t    %d\t\t      %d\n",mainMemory[i][0],mainMemory[i][1],mainMemory[i][2]);
	}
	
}
return mainMemory;
}
void printMemory(int **mainMemory,int memorySize){
	int i = 0;
	int j = 0;
	printf("\n\t  Inspecting Main Memory ....\n\n");
	//sleep(2);
	printf("\t   Printing Main Memory ....\n\n");
	//sleep(2);	
	printf("\n  Frame Add\tAlloted Process\t  Occupied bit\n\n");
	//sleep(2);
	for(i = 0; i<memorySize; i++){
		
		if(i==0){
			printf("      %d\t\t    %d\t\t      %d\n",mainMemory[i][0],mainMemory[i][1],mainMemory[i][2]);
		}else{
			printf("    %d\t    %d\t\t      %d\n",mainMemory[i][0],mainMemory[i][1],mainMemory[i][2]);
		}
	}
printf("\n\n");
}
int findFree(int **memory,int memorySize){
	int i = 0;
	for(i = 0; i<memorySize; i++){
		
		if(memory[i][2] == 0)
			return i;
	}
return -1;
}
int allocateMemory(int pId,int **memory,int memorySize){
	int freeRow = 0;
	if((freeRow = findFree(memory,memorySize))!=-1){
		memory[freeRow][1] = pId;
		memory[freeRow][2] = 1;
	return memory[freeRow][0];
	}
return -1;
}
int findAdd(int add,int **memory,int memorySize){
	int i = 0;
	for(i = 0; i<memorySize; i++){
	
		if(memory[i][0]==add)
			return i;
	}
return -1;
}
int reAllocate(int add,int pId,int **memory,int memorySize){
	int rowToReplace = findAdd(add,memory,memorySize);
	if(rowToReplace!=-1){
		memory[rowToReplace][1] = pId;
	return 0;
	}
return -1;
}

