#include <Arduino.h>
#include <SPI.h>                    
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h" // Biblioteca

#define PinAnalogico A0
#define PinDigital 4
#define PinBuzzer 2
#define OledPin 21 // Pino do oled
#define WIRE Wire  // endereço do oled
#define DHTPIN 15 // Pino DHT22

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);
DHTesp dht; // Cria um objeto DHT22


void setup()
{
  Serial.begin(9600);

  pinMode(PinBuzzer, INPUT);
  pinMode(PinAnalogico, INPUT);
  pinMode(PinDigital, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Um endereço de imagem do oled
  dht.setup(DHTPIN, DHTesp::DHT22); // Cria uma conexão com o pino 15

}

void loop()
{
  int LeAnalogica = analogRead(PinAnalogico);
  int LeDigital = digitalRead(PinDigital);
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  



  if (LeDigital == HIGH)
  {
    Serial.println("Detectado");
    digitalWrite(PinBuzzer, HIGH);

    tone(PinBuzzer, 261, 250);
    delay(250);
    noTone(PinBuzzer);
  }
  else
  {
    Serial.println("Nada");
    digitalWrite(PinBuzzer, LOW);
  }
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); // define posição do texto
  display.print("Gas: "); // exibido no oled
  display.println(LeDigital);
  display.setCursor(0,10);
  display.print("Umidade: ");
  display.println(humidity);
  display.setCursor(0,20);
  display.print("Temperatura: ");
  display.println(temperature);
  display.display(); // atualiza oled
}