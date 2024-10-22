/*********************
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
 *********************/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include <Preferences.h>
#include <ESP32Servo.h>
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "atuadores.h"
#include "funcoes.h"

#define mqtt_pub_topic1 "RealTime"
#define DHTPIN 14
#define PinAnalogico A0
#define PinDigital 4
#define PinBuzzer 13

unsigned long tempoAnteriorIntervaloPublicacao = 0;
const unsigned long intervaloPublicao = 1000;
const unsigned long intervaloSalvamentoFlash = 100000;
unsigned long tempoUltimoSalvamento = 0;

int ultimaPos = 0;  // Variável para armazenar a posição atual do servo motor

DHTesp dht; 
extern Servo servoMotor;
Preferences preferences;

// Variáveis para armazenar o último JSON salvo
String ultimoJSONSalvo = "";

void publicaDadosMQTT();
void inicializa_servos();
void moveServo(int pos);

// Função para mover o servo para uma posição específica
void moveServo(int pos) {
  servoMotor.write(pos);  // Move o servo para a posição desejada
  delay(500);             // Tempo para o servo alcançar a posição
  ultimaPos = pos;        // Atualiza a última posição do servo
}

// Função para configurar o botão
/*void inicializa_botao()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura o pino do botão como entrada com resistor pull-up
}*/

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  setup_time();
  inicializa_entradas();
  inicializa_saidas();
  inicializa_servos();              // Inicializa o servo motor
  inicia_RFID();
  randomSeed(esp_random());
  //inicializa_botao();               // Inicializa o botão
  dht.setup(DHTPIN, DHTesp::DHT22); // Configura o sensor DHT22 no pino 14
  
  pinMode(PinBuzzer, OUTPUT);
  pinMode(PinDigital, INPUT_PULLUP);
}

void loop()
{
  atualiza_time();
  atualiza_saidas();
  atualiza_botoes();
  atualiza_mqtt();
  randomiza_senha();
  loop_RFID();

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  int LeDigital = digitalRead(PinDigital);

  if(LeDigital > 0)
  {
    Serial.println("Detectado");
    digitalWrite(PinBuzzer, HIGH);
    digitalWrite(LedExternoState, HIGH);
    
    tone(PinBuzzer, 261, 250);
    delay(250);
    noTone(PinBuzzer);
  }
  else
  {
    digitalWrite(PinBuzzer, LOW);
    digitalWrite(LedExternoState, LOW);
  }

  // Cria o objeto JSON
  JsonDocument doc; // Tamanho definido para o JsonDocument
  String json;

  doc["timeStamp"] = timeStamp();   // Adiciona o timestamp atual
  doc["Porta"] = LedExternoState;   // Posição atual do servo motor
  doc["Umidade"] = humidity;        // Umidade medida pelo sensor DHT
  doc["temperatura"] = temperature; // Temperatura medida pelo sensor DHT
  doc["MQ135"] = LeDigital;         // Sensor de fumaça

  // Serializa o JSON em uma string
  serializeJson(doc, json);

  // Publica o JSON via MQTT
  publica_mqtt(mqtt_pub_topic1, json);
}

// Função para gerar e publicar JSON via MQTT e salvar na flash
void publicaDadosMQTT()
{
  // Implementação para salvar e publicar os dados se necessário
}

// Variáveis e funções relacionadas à senha
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