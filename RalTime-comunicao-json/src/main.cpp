/*************************
 * Projeto IOT com ESP32
 * Escola SENAI de Informática
 * Curso de Apredizagem industrial
 * Integrador de soluções em nuvem
 * Data: 01/08/2024
 *
 * Autor: Matheus Ferrira Cruvello
 *
 * Descrição: Projeto de automação utilizando ESP32
 * com conexão WiFi e MQTT.
 *
 * versão: 0.9
 *************************/

#include <Arduino.h>
#include <ArduinoJson.h>
#include "iot.h"
#include "saidas.h"
#include "entradas.h"
#include "tempo.h"
#include "atuadores.h"

// Definição dos tópicos de publicação
#define mqtt_pub_topic1 "RealTime"

// Criacao de objetos

// Variáveis globais
unsigned long tempo_anterior = 0;
const unsigned long intervalo = 5000;

// Prototipo das funcoes

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  setup_time();
  inicializa_entradas();
  inicializa_saidas();
  inicializa_mqtt();
  inicializa_servos();
}

void loop()
{
  atualiza_time();
  atualiza_saidas();
  atualiza_mqtt();

  JsonDocument doc;
  String json;

  String teste = "Enviado com sucess";
  float martelo = 129;


  doc["teste"] = teste;
  doc["martelo"] = martelo;

  serializeJson(doc, json);
  publica_mqtt(mqtt_pub_topic1, json);
}
