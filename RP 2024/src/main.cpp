/*******
 * Projeto: Automação IoT com ESP32
 * Escola: SENAI de Informática
 * Curso: Aprendizagem Industrial - Integrador de Soluções em Nuvem
 * Data: 01/08/2024
 *
 * Autor: RealTime
 * Versão: 0.9
 *
 * Descrição:
 * Este projeto utiliza o ESP32 para implementar uma solução
 * de automação com conectividade WiFi e integração via MQTT.
 * O código controla entradas e saídas digitais, monitorando
 * estados e publicando informações para um broker MQTT.
 *******/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <ESP32Servo.h>
#include "atuadores.h"
#include "entradas.h"
#include <Bounce2.h>
#include "funcoes.h"
#include "Display.h"
#include "saidas.h"
#include "tempo.h"
#include "iot.h"



#define mqtt_pub_topic1 "RealTime"
#define mqtt_topic2 "RealRecebe"
#define mqtt_topic3 ""



#define SDA_PIN 21
#define SCL_PIN 22

#define BMP280_ADDRESS 0x76



const int mq135Pin = 34;
const int buzzerPin = 23;
const int ledPin = 2;
const int gasThreshold = 1100;



const unsigned long intervaloSalvamentoFlash = 100000;
unsigned long tempoAnteriorIntervaloPublicacao = 0;
const unsigned long intervaloPublicao = 1000;
unsigned long tempoUltimoSalvamento = 0;
unsigned long acionamento_buzzer = 0;
unsigned long tampo_buzzer = 250;



Preferences preferences;
Adafruit_BMP280 bmp;

extern Servo myservo;
extern LiquidCrystal_I2C lcd;



int contador = 100;
int porta = 0;



String ultimoJSONSalvo = "";



void publicaDadosMQTT();
void temporizador(byte linha, int tempo, int tempoTotal);



void setup()
{
  Serial.begin(115200);

  randomSeed(esp_random());
  inicializa_entradas();
  inicializa_saidas();
  inicializa_servos();
  inicializa_lcd();
  setup_wifi();
  setup_time();


  myservo.write(0);

  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(buzzerPin, OUTPUT);
  pinMode(mq135Pin, INPUT); 
  pinMode(ledPin, OUTPUT);

  
  
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);



  if (!bmp.begin(BMP280_ADDRESS))
  {
    Serial.println(F("Erro ao inicializar o sensor BMP280!"));
    while (1);
  }
  Serial.println(F("Sensor BMP280 inicializado com sucesso!"));
}

void loop()
{
  randomiza_senha();
  atualiza_saidas();
  atualiza_botoes();
  atualiza_mqtt();
  atualiza_time();
  autorizacao();
  
  
  
  float temperatura = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; 
 
  // Sensor de fumaça
  int sensorValue = analogRead(mq135Pin);


  
  boolean LedMq135 = digitalRead(ledPin);

  
  if (sensorValue > gasThreshold) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 261, 250);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }



  JsonDocument doc;
  String json;



  doc["TimeStamp"] = timeStamp();
  doc["MQ135"] = LedMq135;
  doc["Porta"] = Ledexterno;
  doc["Temperatura"] = temperatura;
  doc["Pressao"] = pressure;
  
  

  serializeJson(doc, json);

  publica_mqtt(mqtt_pub_topic1, json);
}

void publicaDadosMQTT()
{
  // Implementação para salvar e publicar os dados se necessário
}



int senha;
const unsigned long intervaloNormal = 30000;
const unsigned long intervaloEstendido = 90000;
unsigned long tempoInicialResetSenha = 0;
unsigned long intervaloResetSenha = 0;
unsigned long intervaloTrava = 10000;



int randomiza_senha()
{
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoInicialResetSenha >= intervaloResetSenha)
  {
    resetaUsuario();
    if (intervaloResetSenha != intervaloNormal)
      intervaloResetSenha = intervaloNormal;
    tempoInicialResetSenha = tempoAtual;
    senha = random(1000, 9999);
    Serial.printf("Nova Senha: %d\n", senha);

    lcd.setCursor(0, 0);
    lcd.print("RealTime  Production");
    lcd.setCursor(2, 1);
    lcd.print("Ultilize a Senha");
    lcd.setCursor(2, 2);
    lcd.print("Para se Conectar");
    lcd.setCursor(7, 3);
    lcd.print(senha);
  }
  return senha;
}



void tempoSenhaEstendido()
{
  if (intervaloResetSenha != intervaloEstendido)
  {
    unsigned long tempoAtual = millis();
    tempoInicialResetSenha = tempoAtual;
    intervaloResetSenha = intervaloEstendido;
    Serial.println("Senha estendida por 90 segundos");
  }
}



void temporizador(byte linha, int tempo, int tempoTotal)
{
  if (intervaloResetSenha = intervaloEstendido)
  {
    lcd.setCursor(1, 0);
    lcd.print("Usuario Conectado  ");
    lcd.setCursor(1, 1);
    lcd.print("Tempo Estendido   ");
    lcd.setCursor(0, linha);
    byte blocosPreechidos = tempo * 20 / tempoTotal;
    byte blocoPArcial = map((tempo * 20) % tempoTotal, 0, tempoTotal, 0, 4);

    for (int i = 0; i < blocosPreechidos; i++)
    {
      lcd.write(byte(255));
    }
    if (blocoPArcial > 0)
    {
      lcd.write(byte(blocoPArcial));
    }
    for (int i = 0; i < (20 - blocosPreechidos); i++)
    {
      lcd.print(" ");
    }
    temporizador(2, contador, 100);
    if (contador > 0)
      contador--;
    else
      contador = 100;
    delay(1000);
  }
}