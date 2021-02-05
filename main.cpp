#include <QCoreApplication>
#include "P7_Client.h"
#include "P7_Trace.h"
#include "P7_Telemetry.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>

using namespace std;

typedef struct
{
    unsigned long usage[16];
} proc_info_t;
unsigned long GetProcessorLoad(proc_info_t *info)
{
    static unsigned long pre_total[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, pre_used[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long cpu[16], nice[16], system[16], idle[16];

    FILE *fp;
    if (!(fp = fopen("/proc/stat", "r")))
    {
    return 0;
    }

    unsigned int index = 0;
    char name[1024];
    do
    {
    fgets(name, sizeof(name)-1, fp);
    if((name[0] != 'c')&&(name[0] != 'p')&&(name[0] != 'u'))
        break;
    sscanf(name, "%*s %d %d %d %d", &cpu[index], &nice[index], &system[index], &idle[index]);
    index++;
    }
    while(index < sizeof(cpu));

    fclose(fp);

    unsigned long used[16];
    unsigned long total[16];
    for(unsigned int k = 0; k < index; k++)
    {
    used[k] = cpu[k] + system[k] + nice[k];
        total[k] = cpu[k] + nice[k] + system[k] + idle[k];
    }

    unsigned long usage = 0;

    for(unsigned int k = 0; k < index; k++)
    {
        if((pre_total[k] == 0)||(pre_used[k] == 0))
    {
        info->usage[k] = 0;
        }
        else
        {
        info->usage[k] = (unsigned long)((100 * (double)(used[k] - pre_used[k]))/(double)(total[k] - pre_total[k]));
    }

    pre_used[k] = used[k];
        pre_total[k] = total[k];
    }
    return index-1;
}


tINT64 Get_Mem_Utilization() {

};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    IP7_Client* l_pClient = P7_Create_Client(TM("/P7.Sink=Baical /P7.Addr=127.0.0.1"));

    tUINT32        l_dwIdx      = 0;
    IP7_Trace     *l_pTrace_1   = NULL;
    IP7_Trace     *l_pTrace_2   = NULL;
    IP7_Telemetry *l_pcTelemetry = NULL;
    IP7_Telemetry *l_pmTelemetry = NULL;
    unsigned long        llCPU      = 0;
    tINT64        llMem      = 0;
    tUINT16       wCpuId     = 0;
    tUINT16       wMemId     = 0;

    l_pTrace_1 = P7_Create_Trace(l_pClient, TM("Trace channel 1"));

    //create P7 trace object 2
    l_pTrace_2 = P7_Create_Trace(l_pClient, TM("Trace channel 2"));

    l_pcTelemetry = P7_Create_Telemetry(l_pClient, TM("Telemetry channel 1"));
    l_pmTelemetry = P7_Create_Telemetry(l_pClient, TM("Telemetry channel 2"));

    if (FALSE == l_pcTelemetry->Create(TM("Group/CPU"),0, 0, 100, 90, 1, &wCpuId))
    {
        goto l_lblExit;
    }

    if (FALSE == l_pmTelemetry->Create(TM("Group/MEM"),0, 0, 500, 450, 1, &wMemId))
    {
        goto l_lblExit;
    }

    l_pTrace_1->P7_TRACE(0, TM("Test trace message #%d"), l_dwIdx ++);
    l_pTrace_1->P7_INFO(0, TM("Test info message #%d"), l_dwIdx ++);
    l_pTrace_1->P7_DEBUG(0, TM("Test debug message #%d"), l_dwIdx ++);
    l_pTrace_1->P7_WARNING(0, TM("Test warning message #%d"), l_dwIdx ++);
    l_pTrace_1->P7_ERROR(0, TM("Test error message #%d"), l_dwIdx ++);
    l_pTrace_1->P7_CRITICAL(0, TM("Test critical message #%d"), l_dwIdx ++);

    l_pTrace_2->P7_QTRACE(1, 0, TM("Test quick trace on channel %d"), 2);

    proc_info_t info;
    while(1)
    {
        llCPU = GetProcessorLoad(&info);
        llCPU = info.usage[3];
        std::cout<<llCPU<<std::endl;
        llMem = Get_Mem_Utilization();

        //deliver info
        l_pcTelemetry->Add(wCpuId, llCPU);
        l_pmTelemetry->Add(wMemId, llMem);
        l_pTrace_1->P7_TRACE(0, TM("Test trace message #%d"), l_dwIdx ++);
        l_pTrace_1->P7_INFO(0, TM("Test info message #%d"), l_dwIdx ++);
        l_pTrace_1->P7_DEBUG(0, TM("Test debug message #%d"), l_dwIdx ++);
        l_pTrace_1->P7_WARNING(0, TM("Test warning message #%d"), l_dwIdx ++);
        l_pTrace_1->P7_ERROR(0, TM("Test error message #%d"), l_dwIdx ++);
        l_pTrace_1->P7_CRITICAL(0, TM("Test critical message #%d"), l_dwIdx ++);

        l_pTrace_2->P7_QTRACE(1, 0, TM("Test quick trace on channel %d"), 2);
    }

    cout << "Hello World!" << endl;

l_lblExit:

    if (l_pcTelemetry)
    {
        l_pcTelemetry->Release();
        l_pcTelemetry = NULL;
    }

    if (l_pmTelemetry)
    {
        l_pmTelemetry->Release();
        l_pmTelemetry = NULL;
    }

    if (l_pTrace_1)
    {
        l_pTrace_1->Release();
        l_pTrace_1 = NULL;
    }

    if (l_pTrace_2)
    {
        l_pTrace_2->Release();
        l_pTrace_2 = NULL;
    }

    if (l_pClient)
    {
        l_pClient->Release();
        l_pClient = NULL;
    }


    return a.exec();
}
