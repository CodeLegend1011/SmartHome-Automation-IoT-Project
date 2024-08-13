/*************************************************************
Title         :   Home automation using Blynk
Description   :   Control heater and cooler, monitor temperature, and display status on Blynk app and CLCD in PICSimLab.
Pheripherals  :   Arduino UNO, Temperature system, CLCD, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
#define BLYNK_TEMPLATE_ID "x"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "x"

#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

bool heater_sw = false;
bool cooler_sw = false;
float current_temp = 0.0;

BLYNK_WRITE(V0)  // Cooler control
{
    cooler_sw = param.asInt();
    cooler_control(cooler_sw);
}

BLYNK_WRITE(V2)  // Heater control
{
    heater_sw = param.asInt();
    heater_control(heater_sw);
}

void update_temperature_reading()
{
    current_temp = read_temperature();
    Blynk.virtualWrite(V1, current_temp); // Send temperature to Blynk app
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(current_temp);
    lcd.print("C");
    
    lcd.setCursor(0, 1);
    if (heater_sw) {
        lcd.print("Heater ON ");
    } else if (cooler_sw) {
        lcd.print("Cooler ON ");
    } else {
        lcd.print("Both OFF  ");
    }
}

void setup(void)
{
    Serial.begin(9600);
    Blynk.begin(auth);
    lcd.begin(16, 2);  // Specify 16 columns and 2 rows for the LCD
    lcd.backlight();
    init_temperature_system();
    timer.setInterval(1000L, update_temperature_reading); // Update temperature every second
}

void loop(void) 
{
    Blynk.run();
    timer.run();
}
