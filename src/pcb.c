#include<stdio.h>
#include<malloc.h>
struct pcb{		//Global Declaration of a structure
                int pID;
                int **pageTable;
		int **instTable;
                int lastInst;
		int noOfInst;
		int maxPages;
        };
	typedef struct pcb mypcb;
mypcb* stateSave(int pID,int **pageTable,int lastInst,int **instTable);
/*int main(int argv, char **argc){
		
	
return 0;
}*/
mypcb* stateSave(int pID,int **pageTable,int lastInst,int **instTable){
	mypcb *pcbProc = (mypcb*)calloc(1,sizeof(mypcb));
	pcbProc->pID = pID;
	pcbProc->pageTable = pageTable;
	pcbProc->lastInst = lastInst;
	pcbProc->instTable = instTable;
return pcbProc;
}

