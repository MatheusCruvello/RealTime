#include <Arduino.h>
#include "saidas.h"

//Definição dos pinos dos leds
#define LedBuiltInPin 2

//Variaveis de controle dos leds
bool LedBuiltInState = LOW;

//Inicializa as saídas digitais
void inicializa_saidas()
{
    pinMode(LedBuiltInPin, OUTPUT);
}

//Atualiza as saídas digitais
void atualiza_saidas()
{
    digitalWrite(LedBuiltInPin, LedBuiltInState);
}