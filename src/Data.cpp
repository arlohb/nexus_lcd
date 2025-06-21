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
