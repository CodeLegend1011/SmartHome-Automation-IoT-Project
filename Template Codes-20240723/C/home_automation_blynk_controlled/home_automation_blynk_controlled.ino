/*************************************************************
Title         :   Home automation using Blynk
Description   :   To control heater, cooler, monitor temperature, and water level in the tank through the Blynk app
Peripherals   :   Arduino UNO, Temperature sensor, Serial Tank, Blynk cloud, Blynk App.
*************************************************************/

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
#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw = OFF, cooler_sw = OFF, inlet_sw = OFF, outlet_sw = OFF;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address 0x27 for a 16 chars and 2 line display

// Cooler Control (V0)
BLYNK_WRITE(COOLER_V_PIN) {
    int state = param.asInt();  // Get value from Blynk app
    if (state == ON) {
        cooler_sw = ON;
        heater_sw = OFF; // Ensure heater is OFF
        Serial.println("Cooler ON");
    } else {
        cooler_sw = OFF;
        Serial.println("Cooler OFF");
    }
}

// Heater Control (V2)
BLYNK_WRITE(HEATER_V_PIN) {
    int state = param.asInt();  // Get value from Blynk app
    if (state == ON) {
        heater_sw = ON;
        cooler_sw = OFF; // Ensure cooler is OFF
        Serial.println("Heater ON");
    } else {
        heater_sw = OFF;
        Serial.println("Heater OFF");
    }
}

// Inlet Valve Control (V4)
BLYNK_WRITE(INLET_V_PIN) {
    int state = param.asInt();  // Get value from Blynk app
    if (state == ON) {
        enable_inlet();  // Open inlet valve
        inlet_sw = ON;
        Serial.println("Inlet Valve Opened");
    } else {
        disable_inlet(); // Close inlet valve
        inlet_sw = OFF;
        Serial.println("Inlet Valve Closed");
    }
}

// Outlet Valve Control (V5)
BLYNK_WRITE(OUTLET_V_PIN) {
    int state = param.asInt();  // Get value from Blynk app
    if (state == ON) {
        enable_outlet();  // Open outlet valve
        outlet_sw = ON;
        Serial.println("Outlet Valve Opened");
    } else {
        disable_outlet(); // Close outlet valve
        outlet_sw = OFF;
        Serial.println("Outlet Valve Closed");
    }
}

// To display temperature and water volume as gauge on the Blynk App
void update_temperature_reading() {
    int temperature = analogRead(A0); // Example for temperature reading
    temperature = map(temperature, 0, 1023, 0, 100); // Assuming temperature range 0-100°C
    int tankVolume = volume(); // Get volume from the tank

    Blynk.virtualWrite(TEMPERATURE_GAUGE, temperature); // Update Blynk with temperature

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(temperature);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Vol:");
    lcd.print(tankVolume);
    lcd.print("L");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Volume: ");
    Serial.print(tankVolume);
    Serial.println(" L");

    // Display valve states on LCD
    lcd.setCursor(10, 0);
    lcd.print(inlet_sw ? "In:ON" : "In:OFF");
    lcd.setCursor(10, 1);
    lcd.print(outlet_sw ? "Out:ON" : "Out:OFF");
}

// To turn off the heater if the temperature raises above 35°C
void handle_temp(void) {
    int temperature = analogRead(A0);
    temperature = map(temperature, 0, 1023, 0, 100); // Map to actual temperature range

    Serial.print("Checking Temperature: ");
    Serial.println(temperature);

    if (temperature > 35 && heater_sw == ON) {
        Blynk.virtualWrite(HEATER_V_PIN, OFF); // Turn OFF heater
        Serial.println("Temperature exceeded 35°C, turning off heater");
    }
}

// To control water volume above 2000 liters
void handle_tank(void) {
    int tankVolume = volume();
    Serial.print("Checking Tank Volume: ");
    Serial.println(tankVolume);

    if (tankVolume > 2000 && inlet_sw == ON) {
        Blynk.virtualWrite(INLET_V_PIN, OFF); // Stop inlet valve
        Serial.println("Tank volume exceeded 2000L, closing inlet valve");
    }
}

void setup(void) {
    Serial.begin(115200); // Increase baud rate for faster communication
    Blynk.begin(auth);

    init_serial_tank(); // Initialize serial tank
    lcd.begin(16, 2);   // Initialize LCD with 16 columns and 2 rows
    lcd.backlight();

    timer.setInterval(1000L, update_temperature_reading); // Update every second
    timer.setInterval(2000L, handle_temp); // Check temperature every 2 seconds
    timer.setInterval(2000L, handle_tank); // Check tank volume every 2 seconds
}

void loop(void) {
    Blynk.run();
    timer.run(); // Initiates Blynk timer
}
