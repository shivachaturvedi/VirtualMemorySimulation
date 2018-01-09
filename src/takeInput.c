#include<stdio.h>	
#include<stdlib.h>
#include<unistd.h>
#include"pageTable.c"
#include"MainMemory.c"
#include"pcb.c"
#include"tlb.c"

int** makeInstTable(int rows,int cols,FILE *reader);	//Function to read instructions from a file and return an instruction table
mypcb* initialize(int **memory);    //Method to initialize the page table and load it with 3 unique pages and return the pcbs of each process
struct readBuffer{		// Global Decleration of a structure to represent a process
	int maxPages;
	int noInst;
	int **instructionTable;
};
/*********************************** Global Variables ***********************************/

int totalProc = 0;	//Global variable to contain maximum number of processes
int listSize = 0;
int **memory = NULL;
int memorySize = 64;
int algo_flag = 1;
int tlbHit = 0;
int tlbMiss = 0;
int pmtHit = 0;
int pmtMiss = 0;
int **outputTime = NULL;

typedef struct readBuffer myBuffer;
myBuffer** readInput();	//Method to return an array of Structure pointers which contain the data of the Input file.
struct pcbNodes{	//Declaring a node of the linklist in the Input Queue 
	mypcb *procPcb;

	int timeTaken;
	int tlbHit;
	int tlbMiss;
	int pmtHit;
	int pmtMiss;

	struct pcbNodes *next;
};
typedef struct pcbNodes pcbNode;
pcbNode *head = NULL;	//Global Variable pointer declaration for storing the head of the link list.
pcbNode* addNode(mypcb *thisProcPcb);	//Method prototype declaration 
pcbNode* makeInputQueue(mypcb *procPcbs){
	int i = 0;
	
	mypcb *sendPcb = procPcbs;
	for(i = 0; i<totalProc; i++){
		
		addNode(sendPcb);
	sendPcb++;
	}
	pcbNode *navigator = head;
	while(navigator->next!=NULL){
		navigator = navigator->next;
	}
	navigator->next = head;		//Making the link list circular
return head;
}
void welcomeScreen(){

	system("clear");
	printf("\n\n\t\t\t ***************** Welcome to Virtual Memory simulation using simple paging technique ************\n\n");
	printf("\n\t\t\t\t1. Least Frequently Used\n\t\t\t\t2. First in First Out\n\t\t\t\t3. Least Recently Used\n");
	printf("\n\t\t\tEnter a value: ");
	scanf("%d",&algo_flag);
}
pcbNode* addNode(mypcb *thisProcPcb){
	
	if(listSize == 0){
		
		pcbNode *temp = (pcbNode*)calloc(1,sizeof(pcbNode));
		temp->procPcb = thisProcPcb;
		temp->next = NULL;
		temp->timeTaken = 0;
		temp->tlbHit = 0;
		temp->tlbMiss = 0;
		temp->pmtHit = 0;
		temp->pmtMiss = 0;
	head = temp;
	++listSize;
	}else{
		pcbNode *temp = (pcbNode*)calloc(1,sizeof(pcbNode));
			
			temp->procPcb = thisProcPcb;
			temp->next = NULL;
			temp->timeTaken = 0;
			temp->tlbHit = 0;
			temp->tlbMiss = 0;
			temp->pmtHit = 0;
			temp->pmtMiss = 0;

		pcbNode *navigator = head;
		
		while(navigator->next!=NULL){
			navigator = navigator->next;
		}
		navigator->next = temp;		
	++listSize;
	}
return head;
}
pcbNode* removeNode(pcbNode *thisProcPcbNode){

	claimMemory(thisProcPcbNode->procPcb->pID,memory,memorySize);


	pcbNode *temp = thisProcPcbNode;
	pcbNode *navigator = head;
	pcbNode *follower = NULL;
	pcbNode *oldHead = NULL;
	

	if(thisProcPcbNode == head){

		head = head->next;
		navigator->next = NULL;
		oldHead = navigator;
		navigator = head;
		while(navigator->next!=oldHead){
		
			navigator = navigator->next;
		}
		navigator->next = head;
		
	//	free(oldHead);

	--listSize;
	}else{
		while(navigator!=temp){
			follower = navigator;
			navigator = navigator->next;
		}
		follower->next = navigator->next;
	//	free(temp);
	--listSize;
	}
return head;
}
void execute(pcbNode *currentNode,int **tlb){	//Execute Method
	
	int i = 0;
	int frameAddress = -1;
	int rowToReplace = -1;
	static int outputRow = 0;

	while(listSize!=0){
	
	  if(currentNode->procPcb->lastInst != currentNode->procPcb->noOfInst-1){	// All Instruction have been executed
	
		if((frameAddress = checkPage(tlb,5,currentNode->procPcb->instTable[i][0]))!=-1){		//Tlb Hit
		
			printf("\n\t  Frame Address : %d\n",frameAddress);	
			++(currentNode->timeTaken);
			currentNode->procPcb->lastInst = i;
			++tlbHit;
			++currentNode->tlbHit;
			++i;

		//Retrieve Frame address
		//Increment LFU Counter
		}
		else if((frameAddress =checkPMT(currentNode->procPcb->pageTable,currentNode->procPcb->instTable[i][0],currentNode->procPcb->maxPages))!=-1){	//PMT Hit
			++tlbMiss;
			++currentNode->tlbMiss;
			++pmtHit;
			++currentNode->pmtHit;

			currentNode->timeTaken = currentNode->timeTaken + 2;

			printf("\n\tPage Table of process : %d\n",currentNode->procPcb->pID);
			printPageTable(currentNode->procPcb->maxPages,currentNode->procPcb->pageTable);
			currentNode->procPcb->lastInst = i;
			addTlb(tlb,currentNode->procPcb->instTable[i][0],frameAddress,5,5);
			++i;

			//Retrieve Frame Address
			//Increment PMT counters
			//Update Tlb
		} 
		else{
		
			if(countFree(memory,memorySize)<6){
			
				memory = changeSize(memorySize,memorySize*2,memory);

				memorySize = memorySize*2;
			}
			
			if((frameAddress = allocateMemory(currentNode->procPcb->pID,memory,memorySize))!=-1){
			
			++pmtMiss;
			++tlbMiss;
			++currentNode->tlbMiss;
			++currentNode->pmtMiss;
			

			currentNode->procPcb->pageTable[(rowToReplace = getReplacementAddress(currentNode->procPcb->pageTable,currentNode->procPcb->maxPages,algo_flag))][1] = frameAddress;
				if(currentNode->procPcb->pageTable[rowToReplace][5] == 0)
					currentNode->timeTaken = currentNode->timeTaken + 4;
				else
					currentNode->timeTaken = currentNode->timeTaken + 6;

				currentNode->procPcb->pageTable[rowToReplace][0] = currentNode->procPcb->instTable[i][0];
				currentNode->procPcb->pageTable[rowToReplace][2] = 1;
				currentNode->procPcb->pageTable[rowToReplace][4] = checkHighestLRU(currentNode->procPcb->pageTable,currentNode->procPcb->maxPages) + 1;
				currentNode->procPcb->lastInst = i;
				addTlb(tlb,currentNode->procPcb->instTable[i][0],frameAddress,5,5);

			printf("\n\tPage Table of process : %d\n",currentNode->procPcb->pID);
			printPageTable(currentNode->procPcb->maxPages,currentNode->procPcb->pageTable);
				
					currentNode = currentNode->next;
					if(currentNode->procPcb->lastInst!=-1)
						i = currentNode->procPcb->lastInst + 1;
					else
						i = 0;
		
			flushTlb(tlb);
			printf("\n\t Context Switch performed... \n");
			}
			else{
				reAllocate(currentNode->procPcb->pageTable[getReplacementAddress(currentNode->procPcb->pageTable,currentNode->procPcb->maxPages,1)][1],currentNode->procPcb->pID,memory,memorySize);
				
			currentNode->procPcb->pageTable[(rowToReplace = getReplacementAddress(currentNode->procPcb->pageTable,currentNode->procPcb->maxPages,1))][0] = 	currentNode->procPcb->instTable[i][0];

				//currentNode->procPcb->pageTable[rowToReplace][0] = currentNode->procPcb->instTable[i][0];
				currentNode->procPcb->lastInst = i;
				addTlb(tlb,currentNode->procPcb->instTable[i][0],frameAddress,5,5);

					currentNode = currentNode->next;
					if(currentNode->procPcb->lastInst!=-1)
						i = currentNode->procPcb->lastInst + 1;
					else
						i = 0;
			flushTlb(tlb);	
			printf("\n\t Context Switch performed... \n");
			}
			//Bring Page to main Memeory
			//Update Page Table
			//Update Tlb
			//Assign Last Inst value to pcb 
			//Move to next Node/Process 
			//Set i to 0;
		}
	  }else{	
			pcbNode *removingNode = currentNode;
			currentNode = currentNode->next;	

			outputTime[outputRow][0] = removingNode->procPcb->pID;	
			outputTime[outputRow][1] = removingNode->timeTaken;
			outputTime[outputRow][2] = removingNode->tlbHit;
			outputTime[outputRow][3] = removingNode->tlbMiss;
			outputTime[outputRow][4] = removingNode->pmtHit;
			outputTime[outputRow][5] = removingNode->pmtMiss;
		
				++outputRow;

			printf("\n\t\t Time taken for process %d is %d ms.\n",removingNode->procPcb->pID,removingNode->timeTaken);

			if(listSize != 1)
				removeNode(removingNode);
			else
				--listSize;
			
			if(currentNode->procPcb->lastInst!=-1)
				i = currentNode->procPcb->lastInst + 1;
			else
				i = 0;

			printf("\n\t Node of Process %d removed \n",removingNode->procPcb->pID);	
			printf("\n\t The Process ID of the current node is %d \n",currentNode->procPcb->pID);
			flushTlb(tlb);
			//Remove Node
			//Set i to 0
			//Move to next Node
	}

	}



}
void printOutput(int **outputTime,int listLength){
	
	int i = 0;
	printf("\n\t\tProcess ID\t\t\tCPU Time Taken\t\tTlb Hit\t\tTlb Miss\t\tPmt Hit\t\tPmt Miss\n");
	for(i = 0; i<listLength; i++){
		
		printf("\n\t\t    %d\t\t\t\t      %d\t\t   %d\t\t   %d\t\t\t   %d\t\t   %d\n",outputTime[i][0],outputTime[i][1],outputTime[i][2],outputTime[i][3],outputTime[i][4],outputTime[i][5]);
	}
	printf("\n\n");
}
int** makeTimeOutput(int listSize){
	
	int **ptr = (int**)calloc(listSize,sizeof(int*));
	int i = 0;
		for(i = 0; i<listSize; i++){
			ptr[i] = (int*)calloc(6,sizeof(int));
		}
return ptr;
}
int main(int argv,char **argc){
	
	system("clear");
	welcomeScreen();
	int listLength = 0;

	printf("\n********  Main Memory is being initialized  *************\n\n");

	pcbNode *head = makeInputQueue(initialize(memory=makeMemory(memorySize)));	
	listLength = listSize;
	int **tlb = makeTlb(5,5);	//Making Tlb
	outputTime = makeTimeOutput(listSize);
	
	execute(head,tlb);
	claimMemory(head->procPcb->pID,memory,memorySize);

	printMemory(memory,memorySize);
	printf("\n%d\n\n",memorySize);

	printOutput(outputTime,listLength);
	printf("**** Total Stats ****\n\nTlb Hit = %d\nTlb Miss = %d\nPMT Hit = %d\nPMT Miss = %d\n",tlbHit,tlbMiss,pmtHit,pmtMiss);
return 0;
}
mypcb* initialize(int **memoryDummy){
	myBuffer **processes = readInput();	//Storing pointer to an array of pointers to structures of type readBuffer


	int i = 0, j = 0;
	int maxProc = totalProc;
	mypcb *procPcbs = (mypcb*)calloc(maxProc,sizeof(mypcb));	//Making an array of pcb to hold each process
	//system("clear");
	
	for(i = 0; i<maxProc; i++){
	int **tempPmt = makePageTable(processes[i]->maxPages);	//Making page table for each process
	
			if(countFree(memory,memorySize)<6){
			
				memory = changeSize(memorySize,memorySize*2,memory);

				memorySize = memorySize*2;
			}
	if(loadPages(tempPmt,processes[i]->instructionTable,i+1,memory) != -1){
		procPcbs[i].pID = i+1;
		procPcbs[i].pageTable = tempPmt;
		procPcbs[i].lastInst = -1;
		procPcbs[i].noOfInst = processes[i]->noInst;
		procPcbs[i].instTable = processes[i]->instructionTable;
		procPcbs[i].maxPages = processes[i]->maxPages;
	}
	else{
		printf("\n\t Memory is full after Loading Process %d\n\t  Their Process table will not be loaded!!",i);
		break;
	}
	printf("\n\nPrinting Page Table .......\n");
	printPageTable(3,tempPmt);
	printf("\n\n");
	}
	printMemory(memory,memorySize);
return procPcbs;
}
myBuffer** readInput(){
	FILE *reader = NULL;
	int maxProc = 0;
	int maxPages = 0;
	int noInst = 0;
	int i =0;
	int **instTable = NULL;
	char *fileName = (char*)calloc(20,sizeof(char));	// Initializing a character array of size 20 to store the filename
	myBuffer *newBuffer = NULL;
	myBuffer **procList = NULL;
	//system("clear");
	printf("\n\n\n  Enter the name of the Test file : ");
	scanf("%s",fileName);
	reader = fopen(fileName,"r");
	if(reader == NULL){
		printf("\n\n  File is not available or you do not have sufficient rights to access it!!\n\n");
		exit(-1);
	}else{
		printf("\n\t  File opened .... \n\t  Examining file contents..... \n");
		printf("\n\t  Reading Input from the Test File .....\n\n");
		
		//sleep(3);
		printf("\n\t  Storing file data in the structure [readBuffer] ....\n\n");
		//sleep(2);

		fscanf(reader, "%d", &maxProc);		totalProc = maxProc;
		procList = (myBuffer**)calloc(maxProc,sizeof(myBuffer*));
	for(i = 0; i<maxProc; i++){
		
		newBuffer = (myBuffer*)calloc(1,sizeof(myBuffer));	// Allocating memory for myBuffer for each process
		
		fscanf(reader, "%d", &maxPages);
		fscanf(reader, "%d", &noInst);
		
	//	printf("%d\t%d\t%d\n",maxProc,maxPages,noInst);
		
		instTable = makeInstTable(noInst,2,reader);
		
		newBuffer->maxPages = maxPages;
		newBuffer->noInst = noInst;
		newBuffer->instructionTable = instTable;
	
	procList[i] = newBuffer;	// Storing each process in the array of structure pointers
	}	//End for
	printf("\n\t  Storing data in an array of structure pointers .....\n\n");
	//sleep(3);
	printf("\t  Returning the double pointer of array of structure pointers \n\n");
	//sleep(3);
	printf("\t  Each structure in the process list array contains\n\n");
	//sleep(4);
	printf("\t\t  1) Maximum Pages of Each process.\n\n\t\t");
	//sleep(2);
	printf("  2) Maximum number of Instructions.\n\n\t\t");
	//sleep(2);
	printf("  3) A double pointer to the created instruction Table.\n\n\t\t");
	//sleep(5);
	}	//End else
return procList;
	
}	//End Method
int** makeInstTable(int rows,int cols,FILE *temp){
  
	int i = 0;
	int j = 0;
	int inst = 0;
	static int procID = 1;
	char operation = 'n';
	int **ptr = NULL;
	int instruction = 0;
	int division = 0;
	printf("\n\t  Making Instruction table ...... \n");
	//sleep(3);
	ptr = (int**)calloc(rows, sizeof(int*));
	for(i = 0; i<rows; i++){
		ptr[i] = (int *)calloc(rows, sizeof(int));
	}
	
	for(i = 0; i<rows; i++){
		fscanf(temp, "%d", &division);
		ptr[i][0] = division/1024;

		fscanf(temp, "%c", &operation);
		fscanf(temp, "%c", &operation);
		
		if(operation == 'w'){
			ptr[i][1] = 1;
		}
		else{
			ptr[i][1] = 0;
							
		}
	}
	printf("\n\t  Instruction table created successfully .... \n");
	//sleep(1);
	printf("\n\t  Printing Instruction table for Process %d .... \n",procID++);
	//sleep(2);
	printf("\n\t  Page\t\tModify Bit\n\n");

	for(i = 0; i<rows; i++){
		printf("\t    %d\t\t    %d\n\n",ptr[i][0],ptr[i][1]);
	
	}

return ptr;
  
}

