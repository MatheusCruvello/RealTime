#include <Keypad.h>
#include <ESP32Servo.h>
#include "saidas.h"



const byte linhas = 4;  // Número de linhas
const byte colunas = 3; // Número de colunas



// Mapeamento das teclas numéricas no teclado
char teclas[linhas][colunas] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};



// Pinos das linhas e colunas
byte pinosLinhas[linhas] = {14, 27, 26, 25};
byte pinosColunas[colunas] = {13, 12, 33};



// Inicialização do Keypad
Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, linhas, colunas);



// Configuração do servo
Servo myservo;
int servoPin = 5; // Defina o pino do servo (ajuste conforme necessário)



int Pin = 2024;          // Senha correta
int senhaDigitada = 0;   // Armazena a senha digitada
int contadorDigitos = 0; // Contador de dígitos



void inicializa_servos()
{
  myservo.attach(servoPin);
  myservo.write(0); // Posição inicial do servo
}



void autorizacao()
{
  char tecla = teclado.getKey(); // Lê a tecla pressionada

  if (tecla)
  {
    Serial.println(tecla);

    // Se a tecla é um número, adiciona à senha digitada
    if (tecla >= '0' && tecla <= '9')
    {
      senhaDigitada = senhaDigitada * 10 + (tecla - '0');
      contadorDigitos++;
    }

    // Verifica se a senha possui 4 dígitos
    if (contadorDigitos == 4)
    {
      if (senhaDigitada == Pin)
      {
        Serial.println("Senha correta!");
        myservo.write(90); // Movimenta o servo para 90 graus
        Ledexterno = HIGH;
      }
      else
      {
        Serial.println("Senha incorreta. Tente novamente.");
        myservo.write(0); // Retorna o servo para a posição inicial
      }

      // Reseta a senha e o contador para uma nova tentativa
      senhaDigitada = 0;
      contadorDigitos = 0;
    }

    // Reset da senha ao pressionar '*'
    if (tecla == '*')
    {
      senhaDigitada = 0;
      contadorDigitos = 0;
      Serial.println("Senha resetada. Digite novamente:");
      myservo.write(0); // Retorna o servo para a posição inicial
      Ledexterno = LOW;
    }
  }
}