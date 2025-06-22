#include "Data.h"
#include <stdlib.h>

Data::Data() {}

int Data::getCpuUsage() {
    return getTestValue1();
}

int Data::getMemUsage() {
    return getTestValue1();
}

int Data::getPodCount() {
    return getTestValue1();
}

int Data::getContainerCount() {
    return getTestValue1();
}

int Data::getTestValue1() {
    return rand() % 100;
}

int Data::getTestValue2() {
    return getTestValue1();
}

std::vector<uint8_t> Data::getPodData() {
    std::vector<uint8_t> data;
    data.resize(48);

    for (size_t i = 0; i < data.size(); i++) {
        data[i] = static_cast<uint8_t>(rand() % 256);
    }

    return data;
}

std::vector<uint8_t> Data::getPodDateNormalised() {
    std::vector<uint8_t> data = getPodData();

    uint8_t min = 255;
    uint8_t max = 0;

    for (const uint8_t& value : data) {
        if (value < min) min = value;
        if (value > max) max = value;
    }
    
    for (uint8_t& value : data) {
        value = static_cast<uint8_t>((value - min) * 255 / (max - min));
    }
    
    return data;
}
