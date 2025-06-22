#include "Program.h"

#include <Arduino.h>
#include <WiFi.h>
#include <algorithm>
#include "Utils.h"
#include "Secrets.h"

Program::Program() {}

void Program::setupLvgl() {
    Serial.println("Setting up lvgl...");

    lv_init();
    
    lv_log_register_print_cb([] (lv_log_level_t level, const char* msg) {
        Serial.print(msg);
    });

    lv_tick_set_cb([] () { return (unsigned int) millis(); });

    lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, colourBuffer, sizeof(colourBuffer));
}

void Program::startWifi() {
    xTaskCreate([] (void* arg) {
        Serial.printf("Connecting to WiFi '%s'...\n", secrets::ssid);
        WiFi.setMinSecurity(WIFI_AUTH_OPEN);
        WiFi.begin(secrets::ssid, secrets::password);
        
        wl_status_t lastStatus = static_cast<wl_status_t>(-1);
        while (WiFi.status() != WL_CONNECTED) {
            if (WiFi.status() != lastStatus) {
                Serial.printf("Status changed: ");
                switch (WiFi.status()) {
                    case WL_IDLE_STATUS:
                        Serial.println("WL_IDLE_STATUS");
                        break;
                    case WL_NO_SSID_AVAIL:
                        Serial.println("WL_NO_SSID_AVAIL");
                        break;
                    case WL_CONNECT_FAILED:
                        Serial.println("WL_CONNECT_FAILED");
                        break;
                    case WL_CONNECTION_LOST:
                        Serial.println("WL_CONNECTION_LOST");
                        break;
                    case WL_DISCONNECTED:
                        Serial.println("WL_DISCONNECTED");
                        break;
                    case WL_CONNECTED:
                        Serial.println("WL_CONNECTED");
                        break;
                    default:
                        Serial.println("Unknown status");
                }
                lastStatus = WiFi.status();
            }
            delay(100);
        }
        Serial.println("Connected to WiFi!");
        
        vTaskDelete(nullptr);
    }, "wifi_setup", 4096, nullptr, 1, nullptr);
}

void Program::setup() {
    Serial.begin(115200);
    Serial.println();

    startWifi();

    Serial.println("Initialising backlight pin...");
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);
    
    setupLvgl();
    ui.setup();

    Serial.println("Setup complete");
    
    np.begin();
    np.setBrightness(1);
    np.fill(np.Color(255, 0, 0));
    np.show();
    
    srand(static_cast<unsigned int>(millis()));
}

void Program::loop() {
    lv_timer_handler();
    
    ui.loop(data);
    
    auto podData = data.getPodData();
    for (size_t i = 0; i < std::min(podData.size(), static_cast<size_t>(NP_COUNT)); i++) {
        uint8_t value = podData[i];
        lv_color_t color = utils::color_temp_deep(static_cast<uint8_t>((float)value * 100.0f / 255.0f));
        np.setPixelColor(i, np.Color(color.red, color.green, color.blue));
    }
    np.show();
}
