#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "funcoes.h"
#include <MFRC522.h>
#include "senhas.h"
#include "saidas.h"
#include <WiFi.h>
#include "iot.h"



#define USUARIO_PADRAO "!@#$%^&*()xyz"



// Definição dos tópicos MQTT
#define mqtt_topic1 "RealTime"
#define mqtt_topic2 "RealRecebe"
#define mqtt_topic3 ""



// ID do cliente MQTT (gerado randomicamente)
const String cliente_id = "ESP32Client_" + String(random(0xffff), HEX);



// Variáveis globais
String usuarioAutorizado = USUARIO_PADRAO;



// Protótipos das funções
void tratar_msg(char *topic, String msg);
void callback(char *topic, byte *payload, unsigned int length);
void reconecta_mqtt();
void inscricao_topicos();



// Instâncias para conexão WiFi e MQTT
WiFiClientSecure espClient;
PubSubClient client(AWS_IOT_ENDPOINT, mqtt_port, callback, espClient);

/**
 * @brief Estabelece a conexão WiFi.
 */



void setup_wifi()
{
  Serial.println();
  Serial.print("Conectando-se a Rede WiFi ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com sucesso com IP: ");
  Serial.println(WiFi.localIP());

  espClient.setCACert(AWS_CERT_CA);
  espClient.setCertificate(AWS_CERT_CRT);
  espClient.setPrivateKey(AWS_CERT_PRIVATE);
}



/**
 * @brief Atualiza a conexão MQTT e reconecta se necessário.
 */
void atualiza_mqtt()
{
  client.loop();
  if (!client.connected())
  {
    reconecta_mqtt();
  }
}

/**
 * @brief Callback chamada ao receber mensagem em um tópico inscrito.
 * 
 * @param topic Tópico onde a mensagem foi recebida.
 * @param payload Conteúdo da mensagem recebida.
 * @param length Tamanho do payload.
 */



void callback(char *topic, byte *payload, unsigned int length)
{
  String msg = "";
  for (int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
  }

   //Serial.printf("Mensagem recebida em [ %s ]: \n\r", topic);
    //Serial.println(msg);
  tratar_msg(topic, msg);
}

/**
 * @brief Tenta reconectar ao Broker MQTT até que a conexão seja estabelecida.
 */



void reconecta_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Tentando se conectar ao Broker MQTT: ");
    Serial.println(AWS_IOT_ENDPOINT);
    if (client.connect(THINGNAME))
    {
      Serial.println("Conectado ao Broker MQTT");
      inscricao_topicos();
    }
    else
    {
      Serial.println("Falha ao conectar ao Broker."); 
      Serial.println("Havera nova tentativa de conexao em 2 segundos");
      delay(2000);
    }
  }
}

/**
 * @brief Publica uma mensagem em um tópico MQTT.
 * 
 * @param topico Tópico onde a mensagem será publicada.
 * @param msg Mensagem a ser publicada.
 */



void publica_mqtt(String topico, String msg)
{
  client.publish(topico.c_str(), msg.c_str());
}

/**
 * @brief Inscreve nos tópicos MQTT definidos.
 */



void inscricao_topicos()
{
  client.subscribe(mqtt_topic1); // Envio Json
  client.subscribe(mqtt_topic2); // Recebe comando
  //client.subscribe(mqtt_topic3); 
}

/**
 * @brief Processa as mensagens recebidas dos tópicos MQTT.
 * 
 * @param topic Tópico onde a mensagem foi recebida.
 * @param msg Conteúdo da mensagem recebida.
 */



void tratar_msg(char *topic, String msg)
{

  // TODO TRATAR MENSSAGENS RECEBIDAS DO TOPICO1 (USUARIO COM VALIDAÇÃO DE TOKEN)
  if (strcmp(topic, mqtt_topic1) == 0)
  {
   
  }


  // TODO TRATAR MENSSAGENS RECEBIDAS DO TOPICO2
  else if (strcmp(topic, mqtt_topic2) == 0)
  {
     int senha = randomiza_senha();

    JsonDocument doc;
    deserializeJson(doc, msg);
    if (doc.containsKey("token")) //tem o campo token?
    {
      if (doc["token"] == senha) //o token é igual ao gerado?
      {
        if (doc.containsKey("user")) //tem o campo user?
        {
          String user = doc["user"]; //pega o valor do campo user

          if (usuarioAutorizado == USUARIO_PADRAO) // se o usuario autorizado for o padrao
            usuarioAutorizado = user; //atualiza o usuario autorizado

          if (usuarioAutorizado == user) //se o usuario autorizado for igual ao usuario que enviou a mensagem
          {
            tempoSenhaEstendido(); //estende o tempo da senha, ao espirar o usuario autorizado volta a ser o padrao
            
            //! *** USUARIO AUTORIZADO ATÉ AQUI ****/
             if (doc.containsKey("user"))
             {
                

             }
          }
        }
      }
    }
  }



  // TODO TRATAR MENSSAGENS RECEBIDAS DO TOPICO3
  else if (strcmp(topic, mqtt_topic3) == 0)
  {

  }
}

/**
 * @brief Reseta o usuário autorizado para o padrão.
 */



void resetaUsuario()
{
  usuarioAutorizado = USUARIO_PADRAO;
}
