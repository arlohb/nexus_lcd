#pragma once

#include <Arduino.h>
#include <vector>
#include <sys/types.h>
#include <atomic>
#include <mutex>

class Data {
public:
    static const uint TASK_STACK_SIZE = 16 * 1024;

    Data();
    
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    
    std::atomic<uint8_t> cpuUsage;
    std::atomic<uint8_t> memUsage;
    std::atomic<uint16_t> podCount;
    std::atomic<uint16_t> containerCount;
    std::atomic<uint8_t> testValue1;
    std::atomic<uint8_t> testValue2;

    std::mutex podDataMutex;
    std::vector<uint8_t> podDataNormalised;

private:
    static String promQuery(const String& query);

    static int getCpuUsage();
    static int getMemUsage();
    static int getPodCount();
    static int getContainerCount();
    
    static int getTestValue1();
    static int getTestValue2();

    static std::vector<uint8_t> getPodData();
    static std::vector<uint8_t> getPodDateNormalised();
};
