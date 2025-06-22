#pragma once

#include <Arduino.h>
#include <vector>
#include <sys/types.h>
#include <atomic>

class Data {
public:
    Data();
    
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    
    std::atomic<uint8_t> cpuUsage;
    std::atomic<uint8_t> memUsage;
    std::atomic<uint16_t> podCount;
    std::atomic<uint16_t> containerCount;
    std::atomic<uint8_t> testValue1;
    std::atomic<uint8_t> testValue2;
    
    std::vector<uint8_t> getPodData() const;
    std::vector<uint8_t> getPodDateNormalised() const;

private:
    static String promQuery(const String& query);

    static int getCpuUsage();
    static int getMemUsage();
    static int getPodCount();
    static int getContainerCount();
    
    static int getTestValue1();
    static int getTestValue2();
};
