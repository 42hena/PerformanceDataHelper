#pragma comment(lib,"Pdh.lib")

#include "PerformanceDataHelper.h"

#include <stdio.h>
#include <Windows.h>
#include <string>
#include <Pdh.h>    // 필수
#include <strsafe.h>// 필수
#include <Pdhmsg.h> // 필수

#pragma region MemoryPerformanceDataHelper 특수 멤버 함수

MemoryPerformanceDataHelper::MemoryPerformanceDataHelper()
    : _memoryQuery(nullptr)
    , _memoryNonPagedByte(nullptr)
    , _memoryPagedByte(nullptr)
    , _memoryAvailableByte(nullptr)
{
    InitMemoryPerformanceData();
}

#pragma endregion

#pragma region MemoryPerformanceDataHelper 초기화 함수

void MemoryPerformanceDataHelper::InitMemoryPerformanceData()
{
    PDHUtils::PDHOpenQueuryA(NULL, NULL, &_memoryQuery);

    PDHUtils::PDHAddCounterA(_memoryQuery, "\\Memory\\Available Bytes", &_memoryAvailableByte);
    PDHUtils::PDHAddCounterA(_memoryQuery, "\\Memory\\Pool Nonpaged Bytes", &_memoryNonPagedByte);
    PDHUtils::PDHAddCounterA(_memoryQuery, "\\Memory\\Pool Paged Bytes", &_memoryPagedByte);
}

#pragma endregion

#pragma region MemoryPerformanceDataHelper 메모리 PDH 출력 함수

void MemoryPerformanceDataHelper::MemoryMonitor(EByteType byteType)
{
    PDHUtils::UpdatePDH(_memoryQuery);

    switch (byteType)
    {
    case EByteType::EByteType_Byte:
    {
        printf("##### Memory Part #####\n""Paged %.3lf(B) / NonPaged %.3lf(B) / Available %.3lf(B) \n\n", static_cast<double>(GetPagedBytes()), static_cast<double>(GetNonPagedBytes()), static_cast<double>(GetAvailableBytes()));
        break;
    }
    case EByteType::EByteType_KiloByte:
    {
        printf("##### Memory Part #####\n""Paged %.3lf(KB) / NonPaged %.3lf(KB) / Available %.3lf(KB) \n\n", static_cast<double>(GetPagedBytes()) / (1024) , static_cast<double>(GetNonPagedBytes()) / (1024), static_cast<double>(GetAvailableBytes()) / (1024));
        break;
    }
    case EByteType::EByteType_MegaByte:
    {
        printf("##### Memory Part #####\n""Paged %.3lf(MB) / NonPaged %.3lf(MB) / Available %.3lf(MB) \n\n", static_cast<double>(GetPagedBytes()) / (1024 * 1024), static_cast<double>(GetNonPagedBytes()) / (1024 * 1024), static_cast<double>(GetAvailableBytes()) / (1024 * 1024));
        break;
    }
    case EByteType::EByteType_GigaByte:
    {
        printf("##### Memory Part #####\n""Paged %.3lf(GB) / NonPaged %.3lf(GB) / Available %.3lf(GB) \n\n", static_cast<double>(GetPagedBytes()) / (1024 * 1024 * 1024), static_cast<double>(GetNonPagedBytes()) / (1024 * 1024 * 1024), static_cast<double>(GetAvailableBytes()) / (1024 * 1024 * 1024));
        break;
    }
    default:
    {
        __debugbreak();
        break;
    }
    }
}

#pragma endregion

#pragma region MemoryPerformanceDataHelper Getter 함수

long long MemoryPerformanceDataHelper::GetNonPagedBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_memoryNonPagedByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

long long MemoryPerformanceDataHelper::GetPagedBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_memoryPagedByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

long long MemoryPerformanceDataHelper::GetAvailableBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_memoryAvailableByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

#pragma endregion






#pragma region ProcessPerformanceDataHelper 특수 멤버 함수

ProcessPerformanceDataHelper::ProcessPerformanceDataHelper(const std::string& filename)
    :_filename(filename)
    , _processQuery(nullptr)
    , _processHandleCount(nullptr)
    , _processThreadCount(nullptr)
    , _processUserCpuUsage(nullptr)
    , _processTotalCpuUsage(nullptr)
    , _processPagedByte(nullptr)
    , _processNonPagedByte(nullptr)
    , _processPrivateByte(nullptr)
    , _processWorkingPrivateByte(nullptr)
{
    printf("filename:%s\n", filename.c_str());

    InitProcessPerformanceData();
}

#pragma endregion

void ProcessPerformanceDataHelper::InitProcessPerformanceData()
{
    PDHUtils::PDHOpenQueuryA(NULL, NULL, &_processQuery);

    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\Handle Count", &_processHandleCount);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\Thread Count", &_processThreadCount);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\% User Time", &_processUserCpuUsage);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\% Processor Time", &_processTotalCpuUsage);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\Pool Paged Bytes", &_processPagedByte);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\Pool Nonpaged Bytes", &_processNonPagedByte);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\Private Bytes", &_processPrivateByte);
    PDHUtils::PDHAddCounterA(_processQuery, "\\Process(" + _filename + ")\\Working Set - Private", &_processWorkingPrivateByte);
}

void ProcessPerformanceDataHelper::ProcessPDHMonitor(EByteType byteType)
{
    PDHUtils::UpdatePDH(_processQuery);

    switch (byteType)
    {
    case EByteType::EByteType_Byte:
    {
        printf("##### Process(%s) Part #####\n""Handle %lld / Thread %lld / \n""CPU [%.3lf%% / %.3lf%%]\n""NP %.3lf(B) / P %.3lf(B) / Private Mem %.3lf(B) / WorkPrivateMem %.3lf(B)\n\n", _filename.c_str(), GetHandleCount(), GetThreadCount(), GetUserCpuPercentage(), GetTotalCpuPercentage(), static_cast<double>(GetNonPagedBytes()), static_cast<double>(GetPagedBytes()), static_cast<double>(GetPrivateMemoryBytes()),static_cast<double>(GetWorkingPrivateMemoryBytes()));
        break;
    }
    case EByteType::EByteType_KiloByte:
    {
        printf("##### Process(%s) Part #####\n""Handle %lld / Thread %lld / \n""CPU [%.3lf%% / %.3lf%%]\n""NP %.3lf(KB) / P %.3lf(KB) / Private Mem %.3lf(KB) / WorkPrivateMem %.3lf(KB)\n\n", _filename.c_str(), GetHandleCount(), GetThreadCount(), GetUserCpuPercentage(), GetTotalCpuPercentage(), static_cast<double>(GetNonPagedBytes()) / (1024), static_cast<double>(GetPagedBytes()) / (1024), static_cast<double>(GetPrivateMemoryBytes()) / (1024), static_cast<double>(GetWorkingPrivateMemoryBytes()) / (1024));
        break;
    }
    case EByteType::EByteType_MegaByte:
    {
        printf("##### Process(%s) Part #####\n""Handle %lld / Thread %lld / \n""CPU [%.3lf%% / %.3lf%%]\n""NP %.3lf(MB) / P %.3lf(MB) / Private Mem %.3lf(MB) / WorkPrivateMem %.3lf(MB)\n\n", _filename.c_str(), GetHandleCount(), GetThreadCount(), GetUserCpuPercentage(), GetTotalCpuPercentage(), static_cast<double>(GetNonPagedBytes()) / (1024 * 1024), static_cast<double>(GetPagedBytes()) / (1024 * 1024), static_cast<double>(GetPrivateMemoryBytes()) / (1024 * 1024), static_cast<double>(GetWorkingPrivateMemoryBytes()) / (1024 * 1024));
        break;
    }
    case EByteType::EByteType_GigaByte:
    {
        printf("##### Process(%s) Part #####\n""Handle %lld / Thread %lld / \n""CPU [%.3lf%% / %.3lf%%]\n""NP %.3lf(GB) / P %.3lf(GB) / Private Mem %.3lf(GB) / WorkPrivateMem %.3lf(GB)\n\n", _filename.c_str(), GetHandleCount(), GetThreadCount(), GetUserCpuPercentage(), GetTotalCpuPercentage(), static_cast<double>(GetNonPagedBytes()) / (1024 * 1024 * 1024), static_cast<double>(GetPagedBytes()) / (1024 * 1024 * 1024), static_cast<double>(GetPrivateMemoryBytes()) / (1024 * 1024 * 1024), static_cast<double>(GetWorkingPrivateMemoryBytes()) / (1024 * 1024 * 1024));
        break;
    }
    default:
    {
        __debugbreak();
        break;
    }
    }
    
}

#pragma region ProcessPerformanceDataHelper Getter 함수

long long ProcessPerformanceDataHelper::GetHandleCount()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processHandleCount, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

long long ProcessPerformanceDataHelper::GetThreadCount()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processThreadCount, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

double ProcessPerformanceDataHelper::GetUserCpuPercentage()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processUserCpuUsage, PDH_FMT_DOUBLE, &counterValue);
    return counterValue.doubleValue;
}

double ProcessPerformanceDataHelper::GetTotalCpuPercentage()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processTotalCpuUsage, PDH_FMT_DOUBLE, &counterValue);
    return counterValue.doubleValue;
}

long long ProcessPerformanceDataHelper::GetNonPagedBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processNonPagedByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

long long ProcessPerformanceDataHelper::GetPagedBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processPagedByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

long long ProcessPerformanceDataHelper::GetPrivateMemoryBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processPrivateByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

long long ProcessPerformanceDataHelper::GetWorkingPrivateMemoryBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_processWorkingPrivateByte, PDH_FMT_LARGE, &counterValue);
    return counterValue.largeValue;
}

#pragma endregion






#pragma region ProcessorPerformanceDataHelper 특수 멤버 함수

ProcessorPerformanceDataHelper::ProcessorPerformanceDataHelper()
{
    InitProcessorPerformanceData();
}

#pragma endregion

void ProcessorPerformanceDataHelper::InitProcessorPerformanceData()
{
    PDHUtils::PDHOpenQueuryA(NULL, NULL, &_processorQuery);

    PDHUtils::PDHAddCounterA(_processorQuery, "\\Processor(_Total)\\% Processor Time", &_cpuUsage);
}

void ProcessorPerformanceDataHelper::ProcessorPDHMonitor()
{
    PDHUtils::UpdatePDH(_processorQuery);
    
    printf("##### Processor Part #####\n""CPU Usage: %.3lf\n\n", GetTotalCpuUsagePercentage());
}

#pragma region ProcessorPerformanceDataHelper Getter 함수

double ProcessorPerformanceDataHelper::GetTotalCpuUsagePercentage()
{
    PDH_FMT_COUNTERVALUE counterValue;

    PDHUtils::PDHGetValue(_cpuUsage, PDH_FMT_DOUBLE, &counterValue);
    return counterValue.doubleValue;
}

#pragma endregion





#pragma region NetworkPerformanceDataHelper 특수 멤버 함수

NetworkPerformanceDataHelper::NetworkPerformanceDataHelper()
{
    InitNetworkPerformanceData();
}

#pragma endregion

void NetworkPerformanceDataHelper::InitNetworkPerformanceData()
{
    int index = 0;
    CHAR* currentStr = NULL;
    CHAR* countersStr = NULL;
    CHAR* interfacesStr = NULL;
    DWORD counterSize = 0;
    DWORD interfaceSize = 0;
    CHAR szQuery[1024] = { 0, };

    // 네트워크 쿼리
    PDHUtils::PDHOpenQueuryA(NULL, NULL, &_networkQuery);

    PdhEnumObjectItemsA(NULL, NULL, "Network Interface", countersStr, &counterSize, interfacesStr, &interfaceSize, PERF_DETAIL_WIZARD, 0);
    countersStr = new CHAR[counterSize];
    interfacesStr = new CHAR[interfaceSize];

    if (PdhEnumObjectItemsA(NULL, NULL, "Network Interface", countersStr, &counterSize, interfacesStr, &interfaceSize, PERF_DETAIL_WIZARD, 0) != ERROR_SUCCESS)
    {
        delete[] countersStr;
        delete[] interfacesStr;
        DebugBreak();
    }
    currentStr = interfacesStr;

    for ( ; *currentStr != '\0' && index < static_cast<int>(EPDH_Limit::EPDH_Limit_EthernetMaxCount) ; currentStr += strlen(currentStr) + 1, index += 1)
    {
        _ethernet[index].bUse = true;
        _ethernet[index].szName[0] = L'\0';

        strcpy_s(_ethernet[index].szName, currentStr);

        szQuery[0] = '\0';
        StringCbPrintfA(szQuery, sizeof(CHAR) * 1024, "\\Network Interface(%s)\\Bytes Received/sec", currentStr);
        PDHUtils::PDHAddCounterA(_networkQuery, szQuery, &_ethernet[index].pdh_Counter_Network_RecvBytes);

        szQuery[0] = L'\0';
        StringCbPrintfA(szQuery, sizeof(CHAR) * 1024, "\\Network Interface(%s)\\Bytes Sent/sec", currentStr);
        PDHUtils::PDHAddCounterA(_networkQuery, szQuery, &_ethernet[index].pdh_Counter_Network_SendBytes);
    }
}

void NetworkPerformanceDataHelper::NetworkPDHMonitor(EByteType byteType)
{
    PDHUtils::UpdatePDH(_networkQuery);
    switch (byteType)
    {
    case EByteType::EByteType_Byte:
    {
        printf("##### Network Part #####\n""Recv %.3lf(B) / Send %.3lf(B)\n\n", static_cast<double>(GetNetworkRecvBytes()), static_cast<double>(GetNetworkSendBytes()));
        break;
    }
    case EByteType::EByteType_KiloByte:
    {
        printf("##### Network Part #####\n""Recv %.3lf(KB) / Send %.3lf(KB)\n\n", GetNetworkRecvKiloBytes(), GetNetworkSendKiloBytes());
        break;
    }
    case EByteType::EByteType_MegaByte:
    {
        printf("##### Network Part #####\n""Recv %.3lf(MB) / Send %.3lf(MB)\n\n", GetNetworkRecvMegaBytes(), GetNetworkSendMegaBytes());
        break;
    }
    case EByteType::EByteType_GigaByte:
    {
        printf("##### Network Part #####\n""Recv %.3lf(GB) / Send %.3lf(GB)\n\n", GetNetworkRecvGigaBytes(), GetNetworkSendGigaBytes());
        break;
    }
    default:
    {
        __debugbreak();
        break;
    }
    }
    
}

#pragma region NetworkPerformanceDataHelper Getter

long long NetworkPerformanceDataHelper::GetNetworkRecvBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;
    int index = 0;
    
    _totalNetworkRecvBytes = 0;
    for ( ; index < static_cast<int>(EPDH_Limit::EPDH_Limit_EthernetMaxCount) ; index += 1)
    {
        if (_ethernet[index].bUse)
        {
            PDHUtils::PDHGetValue(_ethernet[index].pdh_Counter_Network_RecvBytes, PDH_FMT_LARGE, &counterValue);
            _totalNetworkRecvBytes += counterValue.largeValue;
        }
    }

    return _totalNetworkRecvBytes;
}

double NetworkPerformanceDataHelper::GetNetworkRecvKiloBytes()
{
    return static_cast<double>(GetNetworkRecvBytes()) / (1024);
}

double NetworkPerformanceDataHelper::GetNetworkRecvMegaBytes()
{
    return static_cast<double>(GetNetworkRecvKiloBytes()) / (1024);
}

double NetworkPerformanceDataHelper::GetNetworkRecvGigaBytes()
{
    return static_cast<double>(GetNetworkRecvMegaBytes()) / (1024);
}

long long NetworkPerformanceDataHelper::GetNetworkSendBytes()
{
    PDH_FMT_COUNTERVALUE counterValue;
    int index = 0;

    _totalNetworkSendBytes = 0;
    for ( ; index < static_cast<int>(EPDH_Limit::EPDH_Limit_EthernetMaxCount) ; index += 1)
    {
        if (_ethernet[index].bUse)
        {
            PDHUtils::PDHGetValue(_ethernet[index].pdh_Counter_Network_SendBytes, PDH_FMT_LARGE, &counterValue);
            _totalNetworkSendBytes += counterValue.largeValue;
        }
    }

    return _totalNetworkSendBytes;
}

double NetworkPerformanceDataHelper::GetNetworkSendKiloBytes()
{
    return static_cast<double>(GetNetworkSendBytes()) / (1024);
}

double NetworkPerformanceDataHelper::GetNetworkSendMegaBytes()
{
    return static_cast<double>(GetNetworkSendKiloBytes()) / (1024);
}

double NetworkPerformanceDataHelper::GetNetworkSendGigaBytes()
{
    return static_cast<double>(GetNetworkSendMegaBytes()) / (1024);
}

#pragma endregion






namespace PDHUtils
{
    void PDHOpenQueuryA(LPCSTR szDataSource, DWORD_PTR dwUserData, PDH_HQUERY* phQuery)
    {
        PDH_STATUS code;

        code = PdhOpenQueryA(szDataSource, dwUserData, phQuery);
        if (code != ERROR_SUCCESS)
        {
            PrintPDHErrorCode(code);
            __debugbreak();
        }
    }

    void PDHOpenQueuryW(LPCWSTR szDataSource, DWORD_PTR    dwUserData, PDH_HQUERY* phQuery)
    {
        PDH_STATUS code;

        code = PdhOpenQueryW(szDataSource, dwUserData, phQuery);
        if (code != ERROR_SUCCESS)
        {
            PrintPDHErrorCode(code);
            __debugbreak();
        }
    }

    void PDHAddCounterA(PDH_HQUERY _query, std::string pdh, PDH_HCOUNTER* pPDH_HCOUNTER)
    {
        PDH_STATUS code;

        code = PdhAddCounterA(_query, pdh.c_str(), NULL, pPDH_HCOUNTER);
        if (code != ERROR_SUCCESS)
        {
            PDHUtils::PrintPDHErrorCode(code);
            __debugbreak();
        }
    }

    void PDHAddCounterW(PDH_HQUERY _query, std::wstring pdh, PDH_HCOUNTER* pPDH_HCOUNTER)
    {
        PDH_STATUS code;

        code = PdhAddCounterW(_query, pdh.c_str(), NULL, pPDH_HCOUNTER);
        if (code != ERROR_SUCCESS)
        {
            PDHUtils::PrintPDHErrorCode(code);
            __debugbreak();
        }
    }

    void PDHGetValue(PDH_HCOUNTER hCount, DWORD format, PPDH_FMT_COUNTERVALUE value)
    {
        PDH_STATUS code;

        code = PdhGetFormattedCounterValue(hCount, format, NULL, value);
        if (code != ERROR_SUCCESS && code != 0xC0000BC6 && code != 0x800007D8)
        {
            PDHUtils::PrintPDHErrorCode(code);
            __debugbreak();
        }
    }

    void UpdatePDH(PDH_HQUERY query)
    {
        PDH_STATUS code;

        code = PdhCollectQueryData(query);
        if (code != ERROR_SUCCESS)
        {
            PrintPDHErrorCode(code);
            __debugbreak();
        }
    }

    void PrintPDHErrorCode(LONG pdhCode)
    {
        switch (pdhCode) {
        case PDH_CSTATUS_VALID_DATA:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_VALID_DATA): 반환된 데이터가 유효합니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_NEW_DATA:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_NEW_DATA): 반환 데이터 값은 유효하며 마지막 샘플과 다릅니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_NO_MACHINE:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_NO_MACHINE): 지정된 컴퓨터에 연결할 수 없거나 컴퓨터가 오프라인 상태입니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_NO_INSTANCE:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_NO_INSTANCE): 지정된 인스턴스가 없습니다.\n", pdhCode);
            break;
        }
        case PDH_MORE_DATA:
        {
            std::wprintf(L"0x%08X (PDH_MORE_DATA): 제공된 버퍼에 맞는 것보다 더 많은 데이터를 반환해야 합니다. 더 큰 버퍼를 할당하고 함수를 다시 호출합니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_ITEM_NOT_VALIDATED:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_ITEM_NOT_VALIDATED): 데이터 항목이 쿼리에 추가되었지만 유효성을 검사하거나 액세스하지 않았습니다. 이 데이터 항목에 대한 다른 상태 정보를 사용할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_RETRY:
        {
            std::wprintf(L"0x%08X (PDH_RETRY): 선택한 작업을 다시 시도해야 합니다.\n", pdhCode);
            break;
        }
        case PDH_NO_DATA:
        {
            std::wprintf(L"0x%08X (PDH_NO_DATA): 반환할 데이터가 없습니다.\n", pdhCode);
            break;
        }
        case PDH_CALC_NEGATIVE_DENOMINATOR:
        {
            std::wprintf(L"0x%08X (PDH_CALC_NEGATIVE_DENOMINATOR): 음수 분모 값이 있는 카운터가 검색되었습니다.\n", pdhCode);
            break;
        }
        case PDH_CALC_NEGATIVE_TIMEBASE:
        {
            std::wprintf(L"0x%08X (PDH_CALC_NEGATIVE_TIMEBASE): 시간 기준 값이 음수인 카운터가 검색되었습니다.\n", pdhCode);
            break;
        }
        case PDH_CALC_NEGATIVE_VALUE:
        {
            std::wprintf(L"0x%08X (PDH_CALC_NEGATIVE_VALUE): 음수 값이 있는 카운터가 검색되었습니다.\n", pdhCode);
            break;
        }
        case PDH_DIALOG_CANCELLED:
        {
            std::wprintf(L"0x%08X (PDH_DIALOG_CANCELLED): 사용자가 대화 상자를 취소했습니다.\n", pdhCode);
            break;
        }
        case PDH_END_OF_LOG_FILE:
        {
            std::wprintf(L"0x%08X (PDH_END_OF_LOG_FILE): 로그 파일의 끝에 도달했습니다.\n", pdhCode);
            break;
        }
        case PDH_ASYNC_QUERY_TIMEOUT:
        {
            std::wprintf(L"0x%08X (PDH_ASYNC_QUERY_TIMEOUT): 비동기 카운터 수집 스레드가 종료될 때까지 기다리는 동안 시간 초과가 발생했습니다.\n", pdhCode);
            break;
        }
        case PDH_CANNOT_SET_DEFAULT_REALTIME_DATASOURCE:
        {
            std::wprintf(L"0x%08X (PDH_CANNOT_SET_DEFAULT_REALTIME_DATASOURCE): 설정된 기본 실시간 데이터 원본을 변경할 수 없습니다. 카운터 데이터를 수집하는 실시간 쿼리 세션이 있습니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_NO_OBJECT:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_NO_OBJECT): 시스템에서 지정된 개체를 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_NO_COUNTER:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_NO_COUNTER): 지정된 카운터를 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_INVALID_DATA:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_INVALID_DATA): 반환된 데이터가 잘못되었습니다.\n", pdhCode);
            break;
        }
        case PDH_MEMORY_ALLOCATION_FAILURE:
        {
            std::wprintf(L"0x%08X (PDH_MEMORY_ALLOCATION_FAILURE): PDH 함수가 작업을 완료하기에 충분한 임시 메모리를 할당할 수 없습니다. 일부 애플리케이션을 닫거나 페이지 파일을 확장하고 함수를 다시 시도합니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_HANDLE:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_HANDLE): 핸들이 유효한 PDH 개체가 아닙니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_ARGUMENT:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_ARGUMENT): 필수 인수가 없거나 올바르지 않습니다.\n", pdhCode);
            break;
        }
        case PDH_FUNCTION_NOT_FOUND: // PDH_기능_찾을_수_없음
        {
            std::wprintf(L"0x%08X (PDH_FUNCTION_NOT_FOUND): 지정된 함수를 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_NO_COUNTERNAME:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_NO_COUNTERNAME): 카운터가 지정되지 않았습니다.\n", pdhCode);
            break;
        }
        case PDH_CSTATUS_BAD_COUNTERNAME:
        {
            std::wprintf(L"0x%08X (PDH_CSTATUS_BAD_COUNTERNAME): 카운터 경로를 구문 분석할 수 없습니다. 지정된 경로의 형식과 구문을 확인합니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_BUFFER:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_BUFFER): 호출자가 전달한 버퍼가 잘못되었습니다.\n", pdhCode);
            break;
        }
        case PDH_INSUFFICIENT_BUFFER:
        {
            std::wprintf(L"0x%08X (PDH_INSUFFICIENT_BUFFER): 요청된 데이터가 제공된 버퍼보다 큽니다. 요청된 데이터를 반환할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_CANNOT_CONNECT_MACHINE:
        {
            std::wprintf(L"0x%08X (PDH_CANNOT_CONNECT_MACHINE): 요청된 컴퓨터에 연결할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_PATH:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_PATH): 지정된 카운터 경로를 해석할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_INSTANCE:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_INSTANCE): 지정된 카운터 경로에서 인스턴스 이름을 읽을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_DATA: // PDH_INVALID_DATA (중복된 이름이지만 다른 값일 수 있으므로 주의)
        {
            std::wprintf(L"0x%08X (PDH_INVALID_DATA): 데이터가 잘못되었습니다. (중복된 코드 설명)\n", pdhCode);
            break;
        }
        case PDH_NO_DIALOG_DATA:
        {
            std::wprintf(L"0x%08X (PDH_NO_DIALOG_DATA): 대화 상자 데이터 블록이 없거나 잘못되었습니다.\n", pdhCode);
            break;
        }
        case PDH_CANNOT_READ_NAME_STRINGS:
        {
            std::wprintf(L"0x%08X (PDH_CANNOT_READ_NAME_STRINGS): 지정된 컴퓨터에서 카운터 및/또는 도움말 텍스트를 읽을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_LOG_FILE_CREATE_ERROR:
        {
            std::wprintf(L"0x%08X (PDH_LOG_FILE_CREATE_ERROR): 지정된 로그 파일을 만들 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_LOG_FILE_OPEN_ERROR:
        {
            std::wprintf(L"0x%08X (PDH_LOG_FILE_OPEN_ERROR): 지정된 로그 파일을 열 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_LOG_TYPE_NOT_FOUND:
        {
            std::wprintf(L"0x%08X (PDH_LOG_TYPE_NOT_FOUND): 지정된 로그 파일 형식이 이 시스템에 설치되지 않았습니다.\n", pdhCode);
            break;
        }
        case PDH_NO_MORE_DATA:
        {
            std::wprintf(L"0x%08X (PDH_NO_MORE_DATA): 더 이상 데이터를 사용할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_ENTRY_NOT_IN_LOG_FILE:
        {
            std::wprintf(L"0x%08X (PDH_ENTRY_NOT_IN_LOG_FILE): 로그 파일에서 지정된 레코드를 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_DATA_SOURCE_IS_LOG_FILE:
        {
            std::wprintf(L"0x%08X (PDH_DATA_SOURCE_IS_LOG_FILE): 지정된 데이터 원본이 로그 파일입니다.\n", pdhCode);
            break;
        }
        case PDH_DATA_SOURCE_IS_REAL_TIME:
        {
            std::wprintf(L"0x%08X (PDH_DATA_SOURCE_IS_REAL_TIME): 지정된 데이터 원본이 현재 활동입니다.\n", pdhCode);
            break;
        }
        case PDH_UNABLE_READ_LOG_HEADER:
        {
            std::wprintf(L"0x%08X (PDH_UNABLE_READ_LOG_HEADER): 로그 파일 헤더를 읽을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_FILE_NOT_FOUND:
        {
            std::wprintf(L"0x%08X (PDH_FILE_NOT_FOUND): 지정된 파일을 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_FILE_ALREADY_EXISTS:
        {
            std::wprintf(L"0x%08X (PDH_FILE_ALREADY_EXISTS): 지정된 파일 이름을 가진 파일이 이미 있습니다.\n", pdhCode);
            break;
        }
        case PDH_NOT_IMPLEMENTED: // PDH_구현되지 않음
        {
            std::wprintf(L"0x%08X (PDH_NOT_IMPLEMENTED): 참조된 함수가 구현되지 않았습니다.\n", pdhCode);
            break;
        }
        case PDH_STRING_NOT_FOUND:
        {
            std::wprintf(L"0x%08X (PDH_STRING_NOT_FOUND): 성능 이름 및 도움말 텍스트 문자열 목록에서 지정된 문자열을 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_UNABLE_MAP_NAME_FILES: // PDH_이름 파일을 매핑할 수 없음
        {
            std::wprintf(L"0x%08X (PDH_UNABLE_MAP_NAME_FILES): 성능 카운터 이름 데이터 파일에 매핑할 수 없습니다. 데이터는 레지스트리에서 읽고 로컬로 저장됩니다.\n", pdhCode);
            break;
        }
        case PDH_UNKNOWN_LOG_FORMAT:
        {
            std::wprintf(L"0x%08X (PDH_UNKNOWN_LOG_FORMAT): 지정된 로그 파일의 형식이 PDH DLL에서 인식되지 않습니다.\n", pdhCode);
            break;
        }
        case PDH_UNKNOWN_LOGSVC_COMMAND:
        {
            std::wprintf(L"0x%08X (PDH_UNKNOWN_LOGSVC_COMMAND): 지정된 Log Service 명령 값이 인식되지 않습니다.\n", pdhCode);
            break;
        }
        case PDH_LOGSVC_QUERY_NOT_FOUND:
        {
            std::wprintf(L"0x%08X (PDH_LOGSVC_QUERY_NOT_FOUND): Log Service에서 지정된 쿼리를 찾을 수 없거나 열 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_LOGSVC_NOT_OPENED:
        {
            std::wprintf(L"0x%08X (PDH_LOGSVC_NOT_OPENED): 성능 데이터 로그 서비스 키를 열 수 없습니다. 이는 권한이 부족하거나 서비스가 설치되지 않았기 때문일 수 있습니다.\n", pdhCode);
            break;
        }
        case PDH_WBEM_ERROR:
        {
            std::wprintf(L"0x%08X (PDH_WBEM_ERROR): WBEM 데이터 저장소에 액세스하는 동안 오류가 발생했습니다.\n", pdhCode);
            break;
        }
        case PDH_ACCESS_DENIED:
        {
            std::wprintf(L"0x%08X (PDH_ACCESS_DENIED): 원하는 컴퓨터 또는 서비스에 액세스할 수 없습니다. 모니터링 중인 컴퓨터 또는 서비스에 대한 로그 서비스 또는 대화형 사용자 세션의 사용 권한 및 인증을 확인합니다.\n", pdhCode);
            break;
        }
        case PDH_LOG_FILE_TOO_SMALL:
        {
            std::wprintf(L"0x%08X (PDH_LOG_FILE_TOO_SMALL): 지정된 최대 로그 파일 크기가 너무 작아서 선택한 카운터를 기록할 수 없습니다. 이 로그 파일에는 데이터가 기록되지 않습니다. 더 작은 카운터 집합을 지정하여 로그하거나 더 큰 파일 크기를 지정하고 이 호출을 다시 시도합니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_DATASOURCE:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_DATASOURCE): ODBC DataSource 이름에 연결할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_SQLDB:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_SQLDB): SQL Database에는 Perfmon에 대한 유효한 테이블 집합이 포함되어 있지 않습니다.\n", pdhCode);
            break;
        }
        case PDH_NO_COUNTERS:
        {
            std::wprintf(L"0x%08X (PDH_NO_COUNTERS): 이 Perfmon SQL 로그 집합에 대한 카운터를 찾을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_ALLOC_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_ALLOC_FAILED): SQLAllocStmt 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_ALLOCCON_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_ALLOCCON_FAILED): SQLAllocConnect에 대한 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_EXEC_DIRECT_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_EXEC_DIRECT_FAILED): SQLExecDirect에 대한 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_FETCH_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_FETCH_FAILED): SQLFetch에 대한 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_ROWCOUNT_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_ROWCOUNT_FAILED): SQLRowCount에 대한 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_MORE_RESULTS_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_MORE_RESULTS_FAILED): SQLMoreResults에 대한 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_CONNECT_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_CONNECT_FAILED): SQLConnect 호출에 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_BIND_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_BIND_FAILED): SQLBindCol에 대한 호출이 실패했습니다.\n", pdhCode);
            break;
        }
        case PDH_CANNOT_CONNECT_WMI_SERVER:
        {
            std::wprintf(L"0x%08X (PDH_CANNOT_CONNECT_WMI_SERVER): 요청된 컴퓨터에서 WMI 서버에 연결할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_COLLECTION_ALREADY_RUNNING:
        {
            std::wprintf(L"0x%08X (PDH_PLA_COLLECTION_ALREADY_RUNNING): 컬렉션이 이미 실행 중입니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_SCHEDULE_OVERLAP:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_SCHEDULE_OVERLAP): 지정된 시작 시간은 종료 시간 이후입니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_COLLECTION_NOT_FOUND:
        {
            std::wprintf(L"0x%08X (PDH_PLA_COLLECTION_NOT_FOUND): 컬렉션이 없습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_SCHEDULE_ELAPSED:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_SCHEDULE_ELAPSED): 지정된 종료 시간이 이미 경과되었습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_NOSTART:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_NOSTART): 컬렉션이 시작되지 않았습니다. 애플리케이션 이벤트 로그에서 오류가 있는지 확인합니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_ALREADY_EXISTS:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_ALREADY_EXISTS): 컬렉션이 이미 있습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_TYPE_MISMATCH:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_TYPE_MISMATCH): 설정 유형이 일치하지 않습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_FILEPATH:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_FILEPATH): 지정한 정보가 올바른 경로 이름으로 확인되지 않습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_SERVICE_ERROR:
        {
            std::wprintf(L"0x%08X (PDH_PLA_SERVICE_ERROR): \"성능 로그 & 경고\" 서비스가 응답하지 않았습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_VALIDATION_ERROR:
        {
            std::wprintf(L"0x%08X (PDH_PLA_VALIDATION_ERROR): 전달된 정보가 잘못되었습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_VALIDATION_WARNING:
        {
            std::wprintf(L"0x%08X (PDH_PLA_VALIDATION_WARNING): 전달된 정보가 잘못되었습니다.\n", pdhCode);
            break;
        }
        case PDH_PLA_ERROR_NAME_TOO_LONG:
        {
            std::wprintf(L"0x%08X (PDH_PLA_ERROR_NAME_TOO_LONG): 제공된 이름이 너무 깁니다.\n", pdhCode);
            break;
        }
        case PDH_INVALID_SQL_LOG_FORMAT:
        {
            std::wprintf(L"0x%08X (PDH_INVALID_SQL_LOG_FORMAT): SQL 로그 형식이 잘못되었습니다. 올바른 형식이 SQL:<DSN-name>!<LogSet-Name>.\n", pdhCode);
            break;
        }
        case PDH_COUNTER_ALREADY_IN_QUERY:
        {
            std::wprintf(L"0x%08X (PDH_COUNTER_ALREADY_IN_QUERY): PdhAddCounter 호출의 성능 카운터가 성능 쿼리에 이미 추가되었습니다. 이 카운터는 무시됩니다.\n", pdhCode);
            break;
        }
        case PDH_BINARY_LOG_CORRUPT:
        {
            std::wprintf(L"0x%08X (PDH_BINARY_LOG_CORRUPT): 입력 이진 로그 파일에서 카운터 정보 및 데이터를 읽을 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_LOG_SAMPLE_TOO_SMALL:
        {
            std::wprintf(L"0x%08X (PDH_LOG_SAMPLE_TOO_SMALL): 입력 이진 로그 파일 중 하나 이상에는 두 개 미만의 데이터 샘플이 포함되어 있습니다.\n", pdhCode);
            break;
        }
        case PDH_OS_LATER_VERSION:
        {
            std::wprintf(L"0x%08X (PDH_OS_LATER_VERSION): 컴퓨터의 운영 체제 버전은 로컬 컴퓨터의 버전보다 늦습니다. 이 작업은 로컬 컴퓨터에서 사용할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_OS_EARLIER_VERSION: // PDH_OS_이전_버전
        {
            std::wprintf(L"0x%08X (PDH_OS_EARLIER_VERSION): 이 컴퓨터는 이전 운영 체제 버전을 지원합니다. 컴퓨터에서 운영 체제 버전을 확인합니다.\n", pdhCode);
            break;
        }
        case PDH_INCORRECT_APPEND_TIME:
        {
            std::wprintf(L"0x%08X (PDH_INCORRECT_APPEND_TIME): 출력 파일에는 추가할 파일보다 이전 데이터가 포함되어야 합니다.\n", pdhCode);
            break;
        }
        case PDH_UNMATCHED_APPEND_COUNTER:
        {
            std::wprintf(L"0x%08X (PDH_UNMATCHED_APPEND_COUNTER): 두 파일 모두 추가하려면 동일한 카운터가 있어야 합니다.\n", pdhCode);
            break;
        }
        case PDH_SQL_ALTER_DETAIL_FAILED:
        {
            std::wprintf(L"0x%08X (PDH_SQL_ALTER_DETAIL_FAILED): SQL 데이터베이스에서 CounterDetail 테이블 레이아웃을 변경할 수 없습니다.\n", pdhCode);
            break;
        }
        case PDH_QUERY_PERF_DATA_TIMEOUT: // PDH_QUERY_PERF_DATA_TIMEOUT
        {
            std::wprintf(L"0x%08X (PDH_QUERY_PERF_DATA_TIMEOUT): 시스템이 바쁩니다. 카운터 데이터를 수집할 때 시간 초과가 발생했습니다. 나중에 다시 시도하거나 CollectTime 레지스트리 값을 늘리세요.\n", pdhCode);
            break;
        }
        default:
        {
            std::wprintf(L"0x%08X: 알 수 없는 PDH 상태 코드입니다.\n", pdhCode);
            break;
        }
        }
    }
}
