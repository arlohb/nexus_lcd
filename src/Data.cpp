#include "Data.h"

#include <stdlib.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Secrets.h"

Data::Data() {}

int Data::getCpuUsage() {
    return static_cast<int>(100 * promQuery("cluster:node_cpu:ratio_rate5m").toFloat());
}

int Data::getMemUsage() {
    return static_cast<int>(100 * promQuery("1 - sum(:node_memory_MemAvailable_bytes:sum) / sum(node_memory_MemTotal_bytes)").toFloat());
}

int Data::getPodCount() {
    return promQuery("sum(kubelet_running_pods{job='kubelet'})").toInt();
}

int Data::getContainerCount() {
    return promQuery("sum(kubelet_running_containers{job='kubelet'})").toInt();
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

String Data::promQuery(const String& query) {
    HTTPClient http;

    String queryEncoded = query;
    queryEncoded.replace(" ", "%20");

    http.begin(String(secrets::promEndpoint) + "/api/v1/query?query=" + queryEncoded);
    
    int code = http.GET();
    if (code != HTTP_CODE_OK) {
        Serial.printf("HTTP GET failed with code %d\n", code);
        http.end();
        return "";
    }
    
    JsonDocument doc;
    deserializeJson(doc, http.getString());
    if (doc["status"] != "success") {
        Serial.println("Prom query not success: " + doc["error"].as<String>());
        http.end();
        return "";
    }
    
    String data = doc["data"]["result"].as<JsonArray>()[0]["value"][1].as<String>();

    http.end();
    
    return data;
}
