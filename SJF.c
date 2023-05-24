#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting , iResponse , iTaT;
} PCB;

int randomInputProcess(int numberOfProcess , PCB P[]);
int printProcess(int numberOfProcess, PCB P[]);
int sortProcess(PCB P[], int start, int end, int criteria);


int main() {
    // Phạm Thanh Duy_21522008, Lê Quốc Anh_21520565 - Mô phỏng giải thuật Shorted Job First
    // Input number of process
    int iNumberOfProcess = 0;
    printf("Enter the number of processes: ");
    scanf("%d" , &iNumberOfProcess);

    PCB Input[iNumberOfProcess];
    PCB ReadyQueue[iNumberOfProcess];
    PCB TerminatedArray[iNumberOfProcess];
    int iRemain = iNumberOfProcess;
    int iReady = 0, iTerminated = 0;

    randomInputProcess(iNumberOfProcess , Input);
    printProcess(iNumberOfProcess, Input);
    sortProcess(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    printf("\t\tProcess Array after sorted\n");
    printProcess(iNumberOfProcess , Input);
    return 0;
}

int randomInputProcess(int numberOfProcess , PCB P[]) {
    srand(time(NULL));
    for(int i = 0; i < numberOfProcess ; i++)
    {
        P[i].iPID = i + 1;
        //Do điều kiện ArrivalTime thuộc [0,20] và BurstTime thuộc [2,12]
        P[i].iArrival = rand()%20;
        P[i].iBurst = rand()%(12 - 2 + 1) + 2;
    }
}

int printProcess(int numberOfProcess, PCB P[]) {
    for (int i = 0; i < numberOfProcess; i++)
    {
        printf("P%d(%d,%d)\n", P[i].iPID , P[i].iArrival , P[i].iBurst);
    }    
}

int sortProcess (PCB P[], int start, int end,  int criteria) {
    // sort mảng tiến trình tăng dần theo tiêu chuẩn được truyền vào với thuật toán interchangeSort
    if(criteria == SORT_BY_ARRIVAL) {
        for(int i = start; i < end ; i++)
        {
            for(int j = i + 1; j <= end ; j++)
            {
                if(P[i].iArrival > P[j].iArrival)
                {
                    PCB tempProcess = P[j];
                    P[j] = P[i];
                    P[i] = tempProcess;
                }
            }
        }
    }
}