#include "Program.h"

#include <Arduino.h>
#include <WiFi.h>
#include "Utils.h"
#include "Secrets.h"

Program::Program() {
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);
}

void Program::start() {
    startWifi();
    ui.start(&data);
    leds.start(&data);
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

        // Use randomness of WiFi connection time as a seed
        srand(static_cast<unsigned int>(millis()));
        
        vTaskDelete(nullptr);
    }, "wifi_setup", 4096, nullptr, 1, nullptr);
}
