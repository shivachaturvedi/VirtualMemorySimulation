#include<stdio.h>
#include<malloc.h>
int** makeTlb(int,int);	//This Method returns an integer  matrix of the dimensions passed to it
void printTlb(int **ptr,int rows,int cols);	//This is a helper function to print tlb
void printIns(int *ptr,int size);	//This is a helper function to print instructions 
void addTlb(int **tlb,int pageNo,int add,int rows,int cols);	//This method adds a page into the tlb using LFU
int getRowIndex(int **tlb,int rows,int cols);	//This is a helper function internally called by addTlb method to locate the row to perform addition.
int getLeastFrequent(int **tlb,int rows);	//This is a helper function internally called by the getIndex function to find the least frequently used page.
int checkPage(int **tlb,int rows,int pageNo);	//This is a method to check if the page is there in tlb. If not, it reuturns -1 or else the row number.

static int top = -1;

int flushTlb(int **tlb){

	int i = 0;
	int j = 0;
	
	for(i = 0; i<5; i++){
		for(j = 0; j<5; j++){
			
			tlb[i][j] = '\0';
		}
	}
return 0;
}
int checkPage(int **tlb,int rows,int pageNo){
	int i =0;
	for(i = 0;i<rows;i++){
		if(tlb[i][0]==pageNo){
			++tlb[i][2];
			return tlb[i][1];
		}
	}
return -1;
}
int getRowIndex(int **tlb,int rows,int cols){
	
	int i =0,j =0;
	for(i = 0;i<rows; i++){	// If there is space remaining in the tlb then return the value of first empty space.
			
		if(tlb[i][0]=='\0'){
			return i;
		}
	}

	if(top == -1)
		top = 0;
	else if(top < rows){
		++top;
	}
	else
		top = 0;
	
	int least = getLeastFrequent(tlb,rows);
return least;
}
int getLeastFrequent(int **tlb,int rows){	// Get the row of the least frequently used page.
	int i =0;
	int j = 0;
	int least = 10000;
	int row = -1;
	int *matches = (int*)calloc(5,sizeof(int));

	for(i =0; i<rows; i++){
		
		if(tlb[i][2]<=least){
			
			row = i;
			least = tlb[i][2];
		}
	}

	for(i = 0; i<rows; i++){
		
		if(tlb[i][2] == least){
			
			matches[j] = i;
			++j;
		}
	}

	if(j>1){
		for(i = 0; i<5; i++){
			
			if(matches[i] >= top){

				return matches[i];
			}
		}
	}


return matches[0];
}
void addTlb(int **tlb,int pageNo,int add,int rows,int cols){	
	int rowToAdd = getRowIndex(tlb,rows,cols);
	tlb[rowToAdd][0] = pageNo;
	tlb[rowToAdd][1] = add;
	
	if(tlb[rowToAdd][2]==0)	
		++tlb[rowToAdd][2];
	
		
}	//Function to add values to Tlb

void printTlb(int **ptr,int rows,int cols){
	int i=0,j=0;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("%d ",ptr[i][j]);
		}
	printf("\n");
	}
}
int** makeTlb(int row,int col){
	int **ptr=(int **)calloc(row,sizeof(int *));
	int i = 0;	
	for(i = 0;i<row; i++){
		ptr[i] = (int *)calloc(col+1,sizeof(int));
	}
	for(i =0;i<row; i++){	// Initializing the third coulmn with 0
		ptr[i][2] = 0;
	}
return ptr;
}	//End Tlb

