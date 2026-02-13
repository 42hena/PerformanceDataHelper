#pragma once

#include <Pdh.h>
#include <string>

enum class EByteType
{
    EByteType_Byte,
    EByteType_KiloByte,
    EByteType_MegaByte,
    EByteType_GigaByte,
};






class MemoryPerformanceDataHelper
{
#pragma region 특수 멤버 함수
public:
    MemoryPerformanceDataHelper();
    ~MemoryPerformanceDataHelper() = default;
    
    // 삭제된 함수
public:
    MemoryPerformanceDataHelper(const MemoryPerformanceDataHelper&)             = delete;
    MemoryPerformanceDataHelper& operator=(const MemoryPerformanceDataHelper&)  = delete;
    MemoryPerformanceDataHelper(MemoryPerformanceDataHelper&&)                  = delete;
    MemoryPerformanceDataHelper& operator=(MemoryPerformanceDataHelper&&)       = delete;
#pragma endregion

public:
    void MemoryMonitor(EByteType byteType = EByteType::EByteType_Byte);

#pragma region Getter
public:
    long long GetNonPagedBytes();
    long long GetPagedBytes();
    long long GetAvailableBytes();
#pragma endregion

private:
    void InitMemoryPerformanceData();

#pragma region 멤버 변수
private:
    PDH_HQUERY      _memoryQuery;

    PDH_HCOUNTER    _memoryNonPagedByte;
    PDH_HCOUNTER    _memoryPagedByte;
    PDH_HCOUNTER    _memoryAvailableByte;
#pragma endregion
};






class ProcessPerformanceDataHelper
{
#pragma region 특수 멤버 함수
public:
    ProcessPerformanceDataHelper(const std::string& filename);
    ~ProcessPerformanceDataHelper() = default;

public:
    ProcessPerformanceDataHelper(const ProcessPerformanceDataHelper&)               = delete;
    ProcessPerformanceDataHelper& operator=(const ProcessPerformanceDataHelper&)    = delete;
    ProcessPerformanceDataHelper(ProcessPerformanceDataHelper&&)                    = delete;
    ProcessPerformanceDataHelper& operator=(ProcessPerformanceDataHelper&&)         = delete;
#pragma endregion

public:
    void ProcessPDHMonitor(EByteType byteType = EByteType::EByteType_Byte);

#pragma region Getter
public:
    __forceinline std::string GetFilename() const { return _filename; };

    long long GetHandleCount();
    long long GetThreadCount();
    double GetUserCpuPercentage();
    double GetTotalCpuPercentage();
    long long GetNonPagedBytes();
    long long GetPagedBytes();
    long long GetPrivateMemoryBytes();
    long long GetWorkingPrivateMemoryBytes();
#pragma endregion

private:
    void InitProcessPerformanceData();

#pragma region 멤버 변수
private:
    std::string _filename;

    PDH_HQUERY _processQuery;

    PDH_HCOUNTER _processHandleCount;
    PDH_HCOUNTER _processThreadCount;
    PDH_HCOUNTER _processUserCpuUsage;
    PDH_HCOUNTER _processTotalCpuUsage;
    PDH_HCOUNTER _processPagedByte;
    PDH_HCOUNTER _processNonPagedByte;
    PDH_HCOUNTER _processPrivateByte;
    PDH_HCOUNTER _processWorkingPrivateByte;
#pragma endregion
};






class ProcessorPerformanceDataHelper
{
#pragma region 특수 멤버 함수
public:
    ProcessorPerformanceDataHelper();
    ~ProcessorPerformanceDataHelper() = default;

public:
    ProcessorPerformanceDataHelper(const ProcessorPerformanceDataHelper&)               = delete;
    ProcessorPerformanceDataHelper& operator=(const ProcessorPerformanceDataHelper&)    = delete;
    ProcessorPerformanceDataHelper(ProcessorPerformanceDataHelper&&)                    = delete;
    ProcessorPerformanceDataHelper& operator=(ProcessorPerformanceDataHelper&&)         = delete;
#pragma endregion

public:
    void ProcessorPDHMonitor();

public:
    double GetTotalCpuUsagePercentage();

private:
    void InitProcessorPerformanceData();

#pragma region 멤버 변수
private:
    PDH_HQUERY _processorQuery;

    /*
    * PDH_HQUERY _cpuQuery;
    */

    PDH_HCOUNTER _cpuUsage;
#pragma endregion
};
// ########################################################################
//                          Network PDH 클래스
// ########################################################################

class NetworkPerformanceDataHelper
{
    enum class EPDH_Limit
    {
        EPDH_Limit_EthernetMaxCount = 8,
    };

    // 이더넷 하나에 대한 Send,Recv PDH 쿼리 정보.
    struct Ethernet
    {
        bool bUse;
        CHAR szName[128];
        PDH_HCOUNTER pdh_Counter_Network_RecvBytes;
        PDH_HCOUNTER pdh_Counter_Network_SendBytes;
    };
    Ethernet _ethernet[static_cast<int>(EPDH_Limit::EPDH_Limit_EthernetMaxCount)]; // 랜카드 별 PDH 정보

#pragma region 특수 멤버 함수
public:
    NetworkPerformanceDataHelper();
    ~NetworkPerformanceDataHelper() = default;

public:
    NetworkPerformanceDataHelper(const NetworkPerformanceDataHelper&)               = delete;
    NetworkPerformanceDataHelper& operator=(const NetworkPerformanceDataHelper&)    = delete;
    NetworkPerformanceDataHelper(NetworkPerformanceDataHelper&&)                    = delete;
    NetworkPerformanceDataHelper& operator=(NetworkPerformanceDataHelper&&)         = delete;
#pragma endregion

public:
    void NetworkPDHMonitor(EByteType byteType = EByteType::EByteType_Byte);

public:
    long long GetNetworkRecvBytes();
    double GetNetworkRecvKiloBytes();
    double GetNetworkRecvMegaBytes();
    double GetNetworkRecvGigaBytes();

    long long GetNetworkSendBytes();
    double GetNetworkSendKiloBytes();
    double GetNetworkSendMegaBytes();
    double GetNetworkSendGigaBytes();

private:
    void InitNetworkPerformanceData();

#pragma region 멤버 변수
private:
    PDH_HQUERY _networkQuery;

    long long _totalNetworkRecvBytes;
    long long _totalNetworkSendBytes;

    
#pragma endregion
};






namespace PDHUtils
{
    void PDHOpenQueuryA(LPCSTR szDataSource, DWORD_PTR dwUserData, PDH_HQUERY* phQuery);
    void PDHOpenQueuryW(LPCWSTR szDataSource, DWORD_PTR dwUserData, PDH_HQUERY* phQuery);
    
    void PDHAddCounterA(PDH_HQUERY _query, std::string pdh, PDH_HCOUNTER* pPDH_HCOUNTER);
    void PDHAddCounterW(PDH_HQUERY _query, std::wstring pdh, PDH_HCOUNTER* pPDH_HCOUNTER);
    
    void UpdatePDH(PDH_HQUERY query);
    
    void PDHGetValue(PDH_HCOUNTER hCount, DWORD format, PPDH_FMT_COUNTERVALUE value);
    
    void PrintPDHErrorCode(LONG pdhCode);
}
