#include <fmt/core.h>
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println(fmt::format("Hello world! 🤖").c_str());
}

void loop() {}

