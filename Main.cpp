#include "PerformanceDataHelper.h"

void MemoryPDHFunction()
{
    MemoryPerformanceDataHelper mpdh;
    for (;;)
    {
        mpdh.MemoryMonitor();
        mpdh.MemoryMonitor(EByteType::EByteType_KiloByte);
        mpdh.MemoryMonitor(EByteType::EByteType_MegaByte);
        mpdh.MemoryMonitor(EByteType::EByteType_GigaByte);
        Sleep(1000);
    }
}

void ProcessPDHFunction()
{
    ProcessPerformanceDataHelper ppdh("chrome");
    for (;;)
    {
        ppdh.ProcessPDHMonitor();
        ppdh.ProcessPDHMonitor(EByteType::EByteType_KiloByte);
        ppdh.ProcessPDHMonitor(EByteType::EByteType_MegaByte);
        ppdh.ProcessPDHMonitor(EByteType::EByteType_GigaByte);
        Sleep(1000);
    }
}

void ProcessorPDHFunction()
{
    ProcessorPerformanceDataHelper ppdh;
    for (;;)
    {
        ppdh.ProcessorPDHMonitor();
        Sleep(1000);
    }
}

void NetworkPDHFunction()
{
    NetworkPerformanceDataHelper npdh;
    for (;;)
    {
        //npdh.NetworkPDHMonitor();
        //npdh.NetworkPDHMonitor(EByteType::EByteType_KiloByte);
        npdh.NetworkPDHMonitor(EByteType::EByteType_MegaByte);
        npdh.NetworkPDHMonitor(EByteType::EByteType_GigaByte);
        Sleep(1000);
    }
}

int main()
{
    //MemoryPDHFunction();
    //ProcessPDHFunction();
    //ProcessorPDHFunction();
    NetworkPDHFunction();
}