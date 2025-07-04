#pragma once

#include <Arduino.h>
#include <vector>
#include <sys/types.h>
#include <atomic>
#include <mutex>
#include <ArduinoJson.h>
#include <HTTPClient.h>

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
    std::atomic<uint8_t> nasUsage;
    std::atomic<uint8_t> testValue;
    
    std::atomic<bool> isArrayOk;

private:
    static String promQuery(const String& query);
    
    CookieJar omvCookieJar;
    void omvLogin();
    JsonDocument omvQuery(
        const std::string& service,
        const std::string& method,
        const JsonDocument& params = JsonDocument()
    );

    static int getCpuUsage();
    static int getMemUsage();
    static int getPodCount();
    static int getContainerCount();
    int getNasUsage();
    
    static int getTestValue();
    
    /// Returns 1 if true, 0 if false, -1 if error
    int getIsArrayOk();
};
