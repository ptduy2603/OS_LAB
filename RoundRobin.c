#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_TURN 2
#define SORT_BY_START 3
#define PRINT_DETAIL 1

typedef struct {
    int iPID;
    int iArrival, iBurst, iBurst_have;
    int iStart, iFinish, iWaiting , iResponse , iTaT;
} PCB;

bool check_Duplicate_Arrival_Time(int array[], int n , int randomNumber);
void random_Input_Process(int numberOfProcess , PCB P[]);
int random_QuantumTime();
void print_Process(int numberOfProcess, PCB p[], int option);
void sortProcess(PCB P[], int start, int end, int criteria);
void drawGanttChart(PCB Process, int current, bool option, int quantumTime);
void pushProcess(int *numberoOfProcess, PCB P[], PCB Process);
void removeProcess(int *numberOfProcess, int index , PCB P[]);
float calculateAWT(int numberOfProcess , PCB P[]);
float calculateATaT(int numberOfProcess , PCB P[]);

int current = 0;
bool sort_rq = 1;

int main()
{
    // Lê Quốc Anh_21520565, Phạm Thanh Duy_21522008 - Mô phỏng giải thuật Round Robin
    // Input number of process
    int iNumberOfProcess;
    
    printf("Enter the number of processes: ");
    scanf("%d", &iNumberOfProcess);

    int min, max;
    printf("Enter the minimum number of quantum time: ");
    scanf("%d", &min);
    printf("Enter the maximum number of quantum time: ");
    scanf("%d", &max);
    int quantumTime = random_QuantumTime(min, max);
    printf("quantum time is : %d\n", quantumTime);

    PCB Input[iNumberOfProcess];
    PCB ReadyQueue[iNumberOfProcess];
    PCB TerminatedArray[iNumberOfProcess];
    int iRemain = iNumberOfProcess;
    int iReady = 0, iTerminated = 0;

    random_Input_Process(iNumberOfProcess , Input);
    sortProcess(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    print_Process(iRemain , Input, 0);

    pushProcess(&iReady , ReadyQueue , Input[0]);
    removeProcess(&iRemain , 0 ,Input);
    int runtime = 0;
    printf(".....GanttChart RR.....\n");

    if(ReadyQueue[0].iBurst_have > quantumTime)
    {
        ReadyQueue[0].iBurst_have -= quantumTime;
        ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
        ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
        current = ReadyQueue[0].iStart + quantumTime;

        if(ReadyQueue[0].iStart != current)
            printf("  %d" , ReadyQueue[0].iStart);

        drawGanttChart(ReadyQueue[0], current, 0, quantumTime);
    }
    else 
    {
        ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
        ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst_have;
        ReadyQueue[0].iResponse = ReadyQueue[0].iArrival;
        current= ReadyQueue[0].iFinish;
        ReadyQueue[0].iBurst_have = 0;
        sort_rq = 0;  
 
        if(ReadyQueue[0].iStart != current)
            printf("  %d" , ReadyQueue[0].iStart);

        drawGanttChart(ReadyQueue[0], current, 1, quantumTime);
    }

    while(iTerminated < iNumberOfProcess)
    {
        while(iRemain > 0)
        {
                if(Input[0].iArrival <= current)
                {
                    pushProcess(&iReady , ReadyQueue , Input[0]);
                    removeProcess(&iRemain , 0 ,Input);
                }
                else break;
        }

        if(iReady > 0)
        {
            if(ReadyQueue[0].iBurst_have > 0)
            {
                if(sort_rq == 1 && iReady > 1)
                {
                    sortProcess(ReadyQueue, 0, iReady - 1, SORT_BY_TURN);
                }
                
                if(ReadyQueue[0].iBurst_have == ReadyQueue[0].iBurst)
                {
                    ReadyQueue[0].iStart = current;
                    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                }
                
                if(ReadyQueue[0].iBurst_have > quantumTime)
                {
                    ReadyQueue[0].iBurst_have -= quantumTime;
                    current += quantumTime;
                    sort_rq = 1;
                    
                    drawGanttChart(ReadyQueue[0], current, 0, quantumTime);
                }
                else
                {
                    ReadyQueue[0].iFinish = current + ReadyQueue[0].iBurst_have;
                    current = ReadyQueue[0].iFinish;
                    ReadyQueue[0].iBurst_have = 0;
                    sort_rq = 0;
                   
                    drawGanttChart(ReadyQueue[0], current, 1, quantumTime);
                    
                }
                
            }
            else
            {
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;
                pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
                removeProcess(&iReady, 0, ReadyQueue);
                continue;
            }
        }

        else
        {
            current = Input[0].iArrival;
            printf("  %d" , current);
            pushProcess(&iReady , ReadyQueue , Input[0]);
            removeProcess(&iRemain , 0 ,Input);
        }

        
    }
    
    sortProcess(TerminatedArray, 0 , iTerminated - 1, SORT_BY_PID);
    print_Process(iTerminated, TerminatedArray, PRINT_DETAIL);
    printf("AWT: %.2f\n", calculateAWT(iTerminated, TerminatedArray));
    printf("ATaT: %.2f", calculateATaT(iTerminated, TerminatedArray));

    return 0;
}

bool checkDuplicateArrivalTime(int array[], int n , int randomNumber)
{
    for (int i = 0; i < n; i++)
    {
        if (array[i] == randomNumber)
        {
            return true;
        }
    }
    return false;
}

void random_Input_Process(int numberOfProcess , PCB P[])
{
    printf(".....Random Process.....\n");
    srand(time(NULL));
    int ArrivalTime[numberOfProcess];
    int n = 0;
    int i = 0;
    while (i < numberOfProcess)
    {
        P[i].iPID = i + 1;
        int arrivalRamdom = rand() % 20;
        do{
            arrivalRamdom = rand() % 20;
        }while (checkDuplicateArrivalTime(ArrivalTime, numberOfProcess, arrivalRamdom));

        ArrivalTime[n++] = arrivalRamdom;
        P[i].iArrival = arrivalRamdom;
        P[i].iBurst = rand() % (12 - 2 + 1) + 2;
        P[i].iBurst_have = P[i].iBurst; 
        i++;
    }
}

int random_QuantumTime(int min, int max)
{
    srand(time(NULL));
    return min + rand() % (max - min + 1);
}

void print_Process(int numberOfProcess, PCB P[], int option) {
    printf("\n");
    if(option == PRINT_DETAIL)
    {
        printf("Process\t  Start\t  Finish  Waiting  Response  TaT\n");
        for(int i = 0; i < numberOfProcess; i++)
             printf("P%d\t   %d\t   %d\t   %d\t     %d\t     %d\n", P[i].iPID , P[i].iStart , P[i].iFinish, P[i].iWaiting , P[i].iResponse , P[i].iTaT);
    }
    else {
        printf("Process\t   ArrivalTime\t   BurstTime\n");
        for (int i = 0; i < numberOfProcess; i++)
        {
            printf("P%d\t\t%d\t\t%d\n", P[i].iPID , P[i].iArrival , P[i].iBurst);
        }
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

    if(criteria == SORT_BY_TURN)
    {
        PCB tempProcess = P[start];
        for(int i = start; i < end ; i++)
        {
            P[i] = P[i + 1];
        }
        P[end] = tempProcess;
    }
}

void drawGanttChart(PCB Process, int current, bool option, int quantumTime) {
    // vẽ biển đồ chart
    int runtime = 0;
    if(option == 0)
    {
        while(runtime <= quantumTime)
        {
            if(runtime == quantumTime/2)
                printf("---P%d---", Process.iPID);
            else
                printf("-");
            runtime++;
        }
        printf("%d", current);
        runtime = 0;
    }   
    if (option == 1)
    {
        while(runtime <= Process.iBurst_have)
        {
            if(runtime == Process.iBurst_have/2)
                printf("---P%d---", Process.iPID);
            else
                printf("-");
            runtime++;
        }

        printf("%d", current);
        runtime = 0;
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