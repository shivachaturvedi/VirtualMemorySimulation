#include<stdio.h>
#include<unistd.h>
#include<malloc.h>



static int lastMatch = -1;

void printPageTable(int rows,int **pageTable);
int getLowestLRU(int **pageTable,int maxPages);

int getReplacementAddress(int **pageTable,int maxPages,int algoFlag){
	
	int i = 0;
	int checkFlag = 0;
	for(i = 0; i<maxPages; i++){
		
		if(pageTable[i][0] == '\0'){
			checkFlag = -1;
			return i;
		}
	}

	if(pageTable[0][3] == -1 && pageTable[1][3] == -1){
		pageTable[0][3] = 0;
		pageTable[1][3] = 0;
	}
	else if(pageTable[0][3] < maxPages-1){
		
		++pageTable[0][3]; 
		++pageTable[1][3];
	}
	else{
		pageTable[0][3] = 0;
		pageTable[1][3] = 0;
	}

	if(checkFlag == 0 && algoFlag == 1){
		
		return getLFU(pageTable,maxPages);	

	}
	else if(checkFlag == 0 && algoFlag == 2){
		
		return pageTable[0][3];
	}
	else if(checkFlag == 0 && algoFlag == 3){

		return getLowestLRU(pageTable,maxPages);
	}


}
int getLowestLRU(int **pageTable, int maxPages){

	int i = 0;
	int lowest = 9999;
	int rowToReplace = -1;

	for(i = 0; i<maxPages; i++){
		
		if(pageTable[i][4]<lowest){
			lowest = pageTable[i][4];
			rowToReplace = i;
		}
		
	}

return rowToReplace;
}
int getLFU(int **pageTable,int maxPages){

	int check = 9999;
	int row = -1;
	int i = 0;
	int j = 0;
	

	int *matches = (int*)calloc(maxPages,sizeof(int));

	for(i = 0; i<maxPages; i++){
		
		if(pageTable[i][2] < check){
			
			check = pageTable[i][2];
			row = i;
		}
	}

	for(i = 0; i<maxPages; i++){
		
		if(pageTable[i][2] == check){
			
			matches[j] = i;
			++j;
		}
	}

	if(j>1){
		for(i = 0; i<5; i++){
			
			if(matches[i] >= pageTable[0][3]){
					pageTable[0][3] = matches[i];
					return matches[i];
			}
		}
	}
	/*else{
		return matches[j-1];
	}*/


return matches[0];

}

int** makePageTable(int maxPages){
	int i = 0;
	int j = 0;
	static int k = 1;

	int rows = maxPages;	//	Make page table of the rows = Maximum Pages in the input for the process


	printf("\n\t  Creating Page Table for process %d \n\n",k++);
	//sleep(3);
	int **pageTable = (int**)calloc(rows,sizeof(int*));
	
 		for(i = 0; i<rows; i++){
			pageTable[i] = (int*)calloc(7,sizeof(int));
		}
return pageTable;
}
int checkHighestLRU(int **pageTable,int maxPages){

	int i = 0;
	int highest = -1;

	for(i = 0; i<maxPages; i++){
		if(pageTable[i][4] > highest)
			highest = pageTable[i][4];
	}
return highest;
}
int loadPages(int **pageTable,int **instTable,int pId,int **memory){	//Three Unique pages must be loaded in the memory.
	int i = 0;
	int j = 0;
	static int f = 1;
	int allocatedMemory = 0;
	printf("\n\t  Loading three unique pages in the memory ..\n\n");
	//sleep(3);
	printf("\t  Examining respective instruction tables ..\n\n");
	//sleep(3);
	printf("\t  Determining unique pages ... \n\n");
	//sleep(3);
	printf("\t  Loading pages in Page Map Table %d\n\n",f++);
	for(i = 0,j = 0; i<3; j++){
		if(checkUnique(pageTable,instTable[j][0])!=-1){
			pageTable[i][0] = instTable[j][0];	//Indexing page in memory
			
			if((allocatedMemory = allocateMemory(pId,memory))!=-1){	//Allocating memory to the pages
				pageTable[i][1] = allocatedMemory;
			}
			else{
				printf("No more processes can be loaded\n Memory is full!!\n");
					
					return -1;
			}
			pageTable[i][5] = instTable[j][1];	//Indexing Modify bit 
			pageTable[i][3] = 0;
			pageTable[i][2] = 0;
			pageTable[i][4] = 0;
			++i;
		}
	}
	pageTable[0][3] = -1;
	pageTable[1][3] = -1;
	printf("\t  Page Map Table loaded successfully ....\n\n");
//printf("Printing Page Table ...................\n\n");
return 0;
}
void printPageTable(int rows,int **pageTable){
	int i = 0;
	printf("\n\n\t  Page\t  Frame Address\t  LFU Counter\t  LRU Counter\t  Modify Bit\n\n");
	//sleep(3);
	int check = 0;
	for(i = 0; i<rows; i++){
	if(pageTable[i][0]=='\0')
		continue;

		if(pageTable[0][1] == 0){	
			printf("\t    %d\t        %d\t      %d\t		%d\t	%d\n",pageTable[i][0],pageTable[i][1],pageTable[i][2],pageTable[i][4],pageTable[i][5]);
			++check;
		}else{
			printf("\t    %d\t     %d\t      %d\t		%d\t	  %d\n",pageTable[i][0],pageTable[i][1],pageTable[i][2],pageTable[i][4],pageTable[i][5]);
		}
	}
}
int checkPMT(int **pageTable,int page,int size){

	int i = -1, j = 0;
	
	for(j = 0; j<size; j++){
		if(pageTable[j][0] == page){	//PMT Hit
			
			++pageTable[j][2];	//Incrementing LFU Counter
			pageTable[j][4] = checkHighestLRU(pageTable,size) + 1;	//Incrementing LRU Counter
			
			
		return pageTable[j][1];
		}
			
	}

return i;
}
int checkUnique(int **pageTable,int value){
	int i = 0;
	for(i = 0; i<3; i++){
	
		if(pageTable[i][0] == value)
			return -1;
	}
return 0;
}
int getIndex(int rows,int **pmt){
	int i = 0;
	int j = 0;
}

