#include "serial_tank.h"
#include "Arduino.h"
#include "main.h"

unsigned int volume_value;
unsigned char valueh, valuel;

void init_serial_tank(void) {
    Serial.begin(19200);
    Serial.write(0xFF); // Synchronize communication
    Serial.write(0xFF);
    Serial.write(0xFF);   
}

unsigned int volume(void) {
    // Send a request to the Serial Tank to get the volume
    Serial.write(VOLUME);  // Replace this with the correct command if necessary

    // Wait for the response
    while (Serial.available() < 2) {
        // Waiting for at least 2 bytes of response
    }

    // Read the high and low byte of the volume
    valueh = Serial.read();
    valuel = Serial.read();

    // Combine the high and low byte to form the full volume value
    volume_value = (valueh << 8) | valuel;

    return volume_value;
}


void enable_inlet(void) {
    // Code to enable inlet valve
    Serial.write(INLET_VALVE);
    Serial.write(ENABLE);
}

void disable_inlet(void) {
    // Code to disable inlet valve
    Serial.write(INLET_VALVE);
    Serial.write(DISABLE);
}

void enable_outlet(void) {
    // Code to enable outlet valve
    Serial.write(OUTLET_VALVE);
    Serial.write(ENABLE);
}

void disable_outlet(void) {
    // Code to disable outlet valve
    Serial.write(OUTLET_VALVE);
    Serial.write(DISABLE);
}
