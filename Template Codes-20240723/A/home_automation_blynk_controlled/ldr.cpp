#include "ldr.h"
#include "Arduino.h"
#include "main.h"

void init_ldr(void)
{
  pinMode(GARDEN_LIGHT, OUTPUT);
  pinMode(LDR_SENSOR, INPUT);
   
}
void brightness_control(void)
{
    // Read the LDR sensor value (0 to 1023)
    int ldrValue = analogRead(LDR_SENSOR);

    // Map the LDR value inversely to the PWM range (0 to 255)
    int brightness = map(ldrValue, 0, 1023, 255, 0);

    // Set the brightness of the garden light
    analogWrite(GARDEN_LIGHT, brightness);
}

