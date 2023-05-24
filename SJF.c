#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting , iResponse , iTaT;
} PCB;

bool checkDuplicateArrivalTime(int array[], int n , int randomNumber);
void randomInputProcess(int numberOfProcess , PCB P[]);
void printProcess(int numberOfProcess, PCB P[]);
void sortProcess(PCB P[], int start, int end, int criteria);
void drawGanttChart(int numberOfProcess, PCB P[]);
void pushProcess(int *numberOfProcess, PCB P[], PCB Process);
void removeProcess(int *numberOfProcess, int index , PCB P[]);
float calculateAWT(int numberOfProcess , PCB P[]);
float calculateATaT(int numberOfProcess , PCB P[]);

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
    sortProcess(Input , 0, iNumberOfProcess - 1 , SORT_BY_ARRIVAL);
    printProcess(iRemain , Input);

    pushProcess(&iReady , ReadyQueue , Input[0]);
    removeProcess(&iRemain , 0 ,Input);

    // xử lí theo FCFS
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart +
    ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart -
    ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish -
    ReadyQueue[0].iArrival;
    // printf("\nReady Queue: ");
    // printProcess(1, ReadyQueue);
    while (iTerminated < iNumberOfProcess)
    {
        while (iRemain > 0)
        {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            }
            else break;
        }

        if (iReady > 0)
        {
            pushProcess(&iTerminated, TerminatedArray,
            ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);

            ReadyQueue[0].iStart = TerminatedArray[iTerminated
            - 1].iFinish;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart +
            ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart -
            ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish -
            ReadyQueue[0].iArrival;
        }
    }

    // sortProcess(TerminatedArray, 0 ,iTerminated - 1, SORT_BY_PID);

    drawGanttChart(iTerminated , TerminatedArray);
    printf("Thoi gian cho trung binh: %.2f\n", calculateAWT(iTerminated, TerminatedArray));
    printf("Thoi gian hoan thanh trung binh: %.2f", calculateATaT(iTerminated, TerminatedArray));
     return 0;
}

bool checkDuplicateArrivalTime(int array[], int n , int randomNumber){
    for(int i = 0; i < n ; i++)
        if(array[i] == randomNumber)
            return true;
    return false;
}

void randomInputProcess(int numberOfProcess , PCB P[]) {
    srand(time(NULL));
    int ArrivalTime[numberOfProcess];
    int n = 0;
    bool flag = false;
    
    for(int i = 0; i < numberOfProcess ; i++)
    {
        P[i].iPID = i + 1;
        //Do điều kiện ArrivalTime thuộc [0,20] và BurstTime thuộc [2,12]
        int arrivalRandom = rand()%20;
        while(checkDuplicateArrivalTime(ArrivalTime, n, arrivalRandom))
        {
            arrivalRandom = rand()%20;
        }

        if(arrivalRandom == 0)
            flag = true;

        ArrivalTime[n++] = arrivalRandom;
        P[i].iArrival = arrivalRandom;
        P[i].iBurst = rand()%(12 - 2 + 1) + 2;
    }

    if(!flag)
    {
        P[rand()%numberOfProcess].iArrival = 0;
    }
}

void printProcess(int numberOfProcess, PCB P[]) {
    printf("Process\tArrivalTime\tBurstTime\n");
    for (int i = 0; i < numberOfProcess; i++)
    {
        printf("P%d\t\t%d\t\t%d\n", P[i].iPID , P[i].iArrival , P[i].iBurst);
    }
    printf("\n");    
}

void sortProcess (PCB P[], int start, int end,  int criteria) {
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

    if(criteria == SORT_BY_PID) {
        for(int i = start; i < end ; i++)
        {
            for(int j = i + 1; j <= end ; j++)
            {
                if(P[i].iPID > P[j].iPID)
                {
                    PCB tempProcess = P[j];
                    P[j] = P[i];
                    P[i] = tempProcess;
                }
            }
        }
    }

    if(criteria == SORT_BY_BURST)
    {
        for(int i = start; i < end ; i++)
        {
            for(int j = i + 1; j <= end ; j++)
            {
                if(P[i].iBurst > P[j].iBurst)
                {
                    PCB tempProcess = P[j];
                    P[j] = P[i];
                    P[i] = tempProcess;
                }
            }
        }
    }
}

void pushProcess(int *n, PCB P[], PCB Process) {
    P[(*n)++] = Process;
}

void removeProcess(int *numberOfProcess, int index , PCB P[]) {
    // xóa phần tử và đôn các phần tử còn lại
    for(int i = index; i < *numberOfProcess - 1 ; i++)
    {
        P[i] = P[i+1];
    }

    (*numberOfProcess)--;
}

float calculateAWT(int numberOfProcess , PCB P[]) {
    int totalWT = 0;
    for(int i = 0; i < numberOfProcess; i++)
        totalWT += P[i].iWaiting;

    return (float)totalWT/numberOfProcess;
}

float calculateATaT(int numberOfProcess , PCB P[]) {
    int totalTaT = 0;
    for(int i = 0; i < numberOfProcess; i++)
        totalTaT += P[i].iTaT;
    
    return (float)totalTaT/numberOfProcess;
}

void drawGanttChart(int numberOfProcess, PCB P[]) {
    // vẽ biển đồ chart
    printf("===GanttChart===\n");
    for(int i = 0; i < numberOfProcess ; i++)
    {
        printf("P%d:  %d  %d\n" , P[i].iPID , P[i].iStart, P[i].iFinish);
    }
}