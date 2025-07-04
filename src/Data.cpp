#include "Data.h"

#include <stdlib.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Secrets.h"

Data::Data() :
    cpuUsage(0),
    memUsage(0),
    podCount(0),
    containerCount(0),
    nasUsage(0),
    testValue(0),
    isArrayOk(true)
{
    xTaskCreate([] (void* arg) {
        Data* data = reinterpret_cast<Data*>(arg);

        // Wait until WiFi is connected
        while (WiFi.status() != WL_CONNECTED) {
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getCpuUsage();
                if (value < 0) continue;
                data->cpuUsage = value;
                Serial.println("Cpu usage updated");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }, "data_cpu_usage", TASK_STACK_SIZE, data, 1, nullptr);
        
        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getMemUsage();
                if (value < 0) continue;
                data->memUsage = value;
                Serial.println("Memory usage updated");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }, "data_mem_usage", TASK_STACK_SIZE, data, 1, nullptr);

        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getPodCount();
                if (value < 0) continue;
                data->podCount = value;
                Serial.println("Pod count updated");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }, "data_pod_count", TASK_STACK_SIZE, data, 1, nullptr);

        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getContainerCount();
                if (value < 0) continue;
                data->containerCount = value;
                Serial.println("Container count updated");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }, "data_container_count", TASK_STACK_SIZE, data, 1, nullptr);

        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getNasUsage();
                if (value < 0) continue;
                data->nasUsage = value;
                Serial.println("Nas usage updated");
                vTaskDelay(pdMS_TO_TICKS(60000));
            }
        }, "data_nas_usage", TASK_STACK_SIZE, data, 1, nullptr);

        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getTestValue();
                if (value < 0) continue;
                data->testValue = value;
                Serial.println("Test value updated");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }, "data_test_value", TASK_STACK_SIZE, data, 1, nullptr);
        
        xTaskCreate([] (void* arg) {
            Data* data = reinterpret_cast<Data*>(arg);
            while (true) {
                int value = data->getIsArrayOk();
                if (value < 0) continue;
                data->isArrayOk = value == 1;
                Serial.println("Is array ok updated");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }, "data_is_array_ok", TASK_STACK_SIZE, data, 1, nullptr);
        
        vTaskDelete(nullptr);
    }, "data_task_creator", 1024, this, 1, nullptr);
}

int Data::getCpuUsage() {
    String str = promQuery("cluster:node_cpu:ratio_rate5m");
    if (str.isEmpty()) {
        Serial.println("Failed to get CPU usage from Prometheus");
        return -1;
    }
    return static_cast<int>(100 * str.toFloat());
}

int Data::getMemUsage() {
    String str = promQuery("1 - sum(node_memory_MemAvailable_bytes) / sum(node_memory_MemTotal_bytes)");
    if (str.isEmpty()) {
        Serial.println("Failed to get Memory usage from Prometheus");
        return -1;
    }
    return static_cast<int>(100 * str.toFloat());
}

int Data::getPodCount() {
    String str = promQuery("sum(kubelet_running_pods{job='kubelet'})");
    if (str.isEmpty()) {
        Serial.println("Failed to get Pod count from Prometheus");
        return -1;
    }
    return str.toInt();
}

int Data::getContainerCount() {
    String str = promQuery("sum(kubelet_running_containers{job='kubelet'})");
    if (str.isEmpty()) {
        Serial.println("Failed to get Container count from Prometheus");
        return -1;
    }
    return str.toInt();
}

int Data::getNasUsage() {
    JsonDocument params;
    params["devicefile"] = "/dev/md0";
    JsonDocument doc = omvQuery("FileSystemMgmt", "enumerateMountedFilesystems", params);
    
    if (doc.isNull()) return -1;
    
    const char* deviceName = "md0";
    const JsonObject* device = nullptr;

    for (const JsonObject& dev : doc.as<JsonArray>()) {
        if (dev["devicename"] == deviceName) {
            device = &dev;
            break;
        }
    }
    
    if (!device) return -1;
    
    return (*device)["percentage"] | 0;
}

int Data::getTestValue() {
    vTaskDelay(pdMS_TO_TICKS(rand() % 100));
    srand(millis());
    return rand() % 100;
}

int Data::getIsArrayOk() {
    JsonDocument params;
    params["devicefile"] = "/dev/md0";
    JsonDocument doc = omvQuery("MdMgmt", "get", params);
    
    if (doc.isNull()) {
        return -1;
    }

    int deviceCount = doc["devices"].size() | 0;

    // Array is ok if there are 5 or more devices
    return deviceCount >= 5 ? 1 : 0;
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

void Data::omvLogin() {
    // Create our own WifiClient to ignore SSL certificates
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    
    http.begin(client, secrets::omvEndpoint);
    http.setCookieJar(&omvCookieJar);
    http.addHeader("Content-Type", "application/json");
    
    JsonDocument doc;
    doc["service"] = "session";
    doc["method"] = "login";
    doc["params"]["username"] = secrets::omvUsername;
    doc["params"]["password"] = secrets::omvPassword;

    int code = http.POST(doc.as<String>());
    if (code != HTTP_CODE_OK) {
        Serial.printf("OMV Auth : HTTP POST failed with code %d\n", code);
        http.end();
        return;
    }
    
    deserializeJson(doc, http.getString());
    
    if (!doc["error"].isNull()) {
        Serial.println("OMV Auth : " + doc["error"]["message"].as<String>());
        http.end();
        return;
    }
    
    http.end();
}

JsonDocument Data::omvQuery(
    const std::string& service,
    const std::string& method,
    const JsonDocument& params
) {
    // Check for auth
    if (omvCookieJar.empty()) {
        Serial.println("OMV Query : Cookie Jar is empty, logging in...");
        omvLogin();
        if (omvCookieJar.empty()) {
            Serial.println("OMV Query : Failed to log in to OMV");
            return JsonDocument();
        }
    }

    // Create our own WifiClient to ignore SSL certificates
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    
    http.begin(client, secrets::omvEndpoint);
    http.addHeader("Content-Type", "application/json");
    
    JsonDocument doc;
    doc["service"] = service;
    doc["method"] = method;
    doc["params"] = params;
    
    // Set cookies manually
    // Didn't work through cookie jar, maybe because of the insecure client
    std::string cookies;
    for (const auto& cookie : omvCookieJar) {
        cookies = cookies + cookie.name.c_str() + "=" + cookie.value.c_str() + "; ";
    }
    // Remove the last semicolon and space
    if (!cookies.empty()) {
        cookies.pop_back(); // Remove last space
        cookies.pop_back(); // Remove last semicolon
    }
    http.addHeader("Cookie", cookies.c_str());

    int code = http.POST(doc.as<String>());

    deserializeJson(doc, http.getString());

    if (code != HTTP_CODE_OK || !doc["error"].isNull()) {
        // If we need to re-authenticate
        if (code == HTTP_CODE_UNAUTHORIZED || code == HTTP_CODE_FORBIDDEN) {
            Serial.println("OMV Query : Unauthorized, re-authenticating...");
            omvCookieJar.clear();
            omvLogin();
            if (omvCookieJar.empty()) {
                Serial.println("OMV Query : Failed to log in to OMV");
                return JsonDocument();
            }
            // Retry the query
            return omvQuery(service, method, params);
        }

        Serial.printf("OMV Query : HTTP POST failed with code %d\n", code);

        // Print OMV error if available
        if (!doc.isNull() && !doc["error"].isNull()) {
            Serial.println("OMV Query : " + doc["error"]["message"].as<String>());
        }

        http.end();
        return JsonDocument();
    }

    http.end();
    return doc["response"];
}
