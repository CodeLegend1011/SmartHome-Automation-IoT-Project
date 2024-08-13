/* To use I2C protocol and built-in LCD library */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* Set the LCD address to 0x27 for a 16 chars and 2 line display */
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Input digital
#define INLET_VALVE  0x00
#define OUTLET_VALVE 0x01

// Sensors digital
#define HIGH_FLOAT 0x10
#define LOW_FLOAT  0x11

// Sensor analog
#define VOLUME 0x30
#define TEMPERATURE 0x31

#define ENABLE  0x01
#define DISABLE 0x00

unsigned int volumeValue, volumeValueL;
unsigned char volumeValueH = 0, volumeValue1 = 0;

unsigned int tempValue, tempValueL;
unsigned char tempValueH = 0, tempValue1 = 0;

void setup(void) {
    Serial.begin(19200);
    /* Initialize the LCD */
    lcd.init();                     
    lcd.backlight();
    lcd.clear();
    lcd.home();
    lcd.setCursor(0, 0);
    lcd.print(" Connect srtank");
    
    /* Synchronize communication */
    Serial.write(0xFF); 
    Serial.write(0xFF);
    Serial.write(0xFF);
}

char buff[6];

void readVolume(void) {
    Serial.write(VOLUME);
    while (!Serial.available());
    volumeValueH = Serial.read();
    while (!Serial.available());
    volumeValue1 = Serial.read();
    volumeValue = (volumeValueH << 8) | volumeValue1 ;
    lcd.setCursor(0, 1);
    lcd.print("V=");
    lcd.print(volumeValue);
    lcd.print("L  ");
}

void readTemperature(void) {
    Serial.write(TEMPERATURE);
    while (!Serial.available());
    tempValueH = Serial.read();
    while (!Serial.available());
    tempValue1 = Serial.read();
    tempValue = (tempValueH << 8) | tempValue1 ;
    lcd.setCursor(8, 1);
    lcd.print("T=");
    lcd.print(tempValue);
    lcd.print("C  ");
}

void filling_start(void) {
    Serial.write(INLET_VALVE);
    Serial.write(ENABLE);

    Serial.write(OUTLET_VALVE);
    Serial.write(DISABLE);

    lcd.setCursor(0, 0);
    lcd.print("Filling ");
    
    do {
        Serial.write(HIGH_FLOAT);
        while (!Serial.available());
        volumeValueL = Serial.read();
        readVolume();
        readTemperature();
    } while (volumeValueL == 0);
}

void start_emptying(void) {
    Serial.write(OUTLET_VALVE);
    Serial.write(ENABLE);

    Serial.write(INLET_VALVE);
    Serial.write(DISABLE);

    lcd.setCursor(0, 0);
    lcd.print("Emptying");
    
    do {
        Serial.write(LOW_FLOAT);
        while (!Serial.available()); 
        volumeValueL = Serial.read();
        readVolume();
        readTemperature();
    } while (volumeValueL == 1);
}

void loop(void) {
    filling_start();
    start_emptying();
} 
