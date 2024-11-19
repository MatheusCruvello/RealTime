#include <Arduino.h>
#include "saidas.h"



#define Led 4



bool Ledexterno = LOW;



void inicializa_saidas(){
  pinMode(Led, OUTPUT);
}



void atualiza_saidas(){
    digitalWrite(Led, Ledexterno);
}