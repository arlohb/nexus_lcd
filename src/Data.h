#pragma once

#include <Arduino.h>
#include <vector>
#include <sys/types.h>

class Data {
public:
    Data();
    
    int getCpuUsage();
    int getMemUsage();
    int getPodCount();
    int getContainerCount();
    
    int getTestValue1();
    int getTestValue2();
    
    std::vector<uint8_t> getPodData();
    std::vector<uint8_t> getPodDateNormalised();

private:
    String promQuery(const String& query);
};
