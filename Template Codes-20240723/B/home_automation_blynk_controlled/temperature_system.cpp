#include "temperature_system.h"
#include "Arduino.h"

void init_temperature_system(void)
{
    pinMode(HEATER, OUTPUT);
    pinMode(COOLER, OUTPUT);
    digitalWrite(HEATER, LOW);  // Initially off
    digitalWrite(COOLER, LOW);  // Initially off
}

float read_temperature(void)
{
    int sensorValue = analogRead(TEMPERATURE_SENSOR);
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperatureC = (voltage - 0.5) * 100.0;  // Assuming an LM35 sensor
    return temperatureC;
}

void cooler_control(bool control)
{
    digitalWrite(COOLER, control ? HIGH : LOW);
}

void heater_control(bool control)
{
    digitalWrite(HEATER, control ? HIGH : LOW);
}
