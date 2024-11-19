#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Display.h"




LiquidCrystal_I2C lcd(0x27, 20, 4);



byte vinte_porcento[]{
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000};



byte quarenta_porcento[]{
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000};



byte sessenta_porcento[]{
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100};



byte oitenta_porcento[]{
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110};



void inicializa_lcd()
{
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, vinte_porcento);
    lcd.createChar(1, quarenta_porcento);
    lcd.createChar(2, sessenta_porcento);
    lcd.createChar(3, oitenta_porcento);
}