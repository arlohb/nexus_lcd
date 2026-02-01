#pragma once

#include <Arduino.h>
#include <vector>
#include <sys/types.h>
#include <atomic>
#include <mutex>
#include <ArduinoJson.h>
#include <HTTPClient.h>

struct Node {
    const char* name;
    const char* ip;
};

class Data {
public:
    static const uint TASK_STACK_SIZE = 7 * 1024;
    static const size_t NODE_COUNT = 4;
    static const std::array<Node, NODE_COUNT> NODES;

    Data();
    
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    
    std::atomic<uint8_t> cpuUsage;
    std::array<std::atomic<uint8_t>, NODE_COUNT> nodeCpuUsage;
    std::atomic<uint8_t> memUsage;
    std::atomic<uint16_t> podCount;
    std::atomic<uint16_t> containerCount;
    std::atomic<uint8_t> nasUsage;
    std::atomic<uint8_t> testValue;
    
    std::atomic<bool> isArrayOk;

private:
    String promQuery(const String& query);
    
    CookieJar omvCookieJar;
    void omvLogin();
    JsonDocument omvQuery(
        const std::string& service,
        const std::string& method,
        const JsonDocument& params = JsonDocument()
    );

    int getCpuUsage();
    int getCpuUsage(const std::string& node);
    int getMemUsage();
    int getPodCount();
    int getContainerCount();
    int getNasUsage();
    
    int getTestValue();
    
    /// Returns 1 if true, 0 if false, -1 if error
    int getIsArrayOk();
};
