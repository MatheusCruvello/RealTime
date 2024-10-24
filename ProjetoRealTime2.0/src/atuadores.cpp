
#include <ESP32Servo.h>
#include <MFRC522.h>
#include "atuadores.h"
#include "saidas.h"

#define SERVO_PIN 15
#define LED1_PIN 26
#define LED2_PIN 25
#define RST_PIN 22
#define SS_PIN 21
#define DEBUG false
#define UID1 3440331809
#define UID2 385974630

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
Servo servoMotor;

unsigned long tempoAbertura = 0;            // Armazena o tempo de abertura do servo
bool servoAberto = false;                   // Indica se o servo está aberto (em 90 graus)
const unsigned long tempoParaFechar = 5000; // Tempo em milissegundos antes de fechar o servo (5 segundos)

void atualizaLEDs()
{
    // Atualiza o estado dos LEDs
    digitalWrite(LED1_PIN, LedBuiltInState ? HIGH : LOW);
}

void inicializa_servos()
{
    servoMotor.attach(15); // Pino onde o servo está conectado (ajuste conforme necessário)
    moveServo(0);          // Inicializa o servo na posição 0 graus
}

void verificaTempoParaFecharServo()
{
    // Verifica se o tempo especificado para fechar o servo já passou
    if (servoAberto && (millis() - tempoAbertura >= tempoParaFechar))
    {
        Serial.println("Tempo esgotado, fechando o servo");
        servoMotor.write(0); // Move o servo de volta para a posição 0 graus
        servoAberto = false; // Atualiza o estado do servo
    }
}

void inicia_RFID()
{
    SPI.begin();
    mfrc522.PCD_Init();
}

unsigned long lerRFID()
{

    // Procura por cartões RFID
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return 0;
    }

    // Seleciona um dos cartões
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return 1;
    }

    unsigned long numericUID = 0;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        numericUID = numericUID * 256 + mfrc522.uid.uidByte[i];
    }

    return numericUID;
}

void loop_RFID()
{
    verificaTempoParaFecharServo();

    unsigned long uid = lerRFID();

    if (DEBUG)
    {
        Serial.println("Aproxime o cartão do leitor");
        if (uid == 0)
        {
            Serial.println("Cartão não encontrado");
        }
        else if (uid == 1)
        {
            Serial.println("Erro ao ler cartão");
        }
    }

    if (uid > 1)
    {
        switch (uid)
        {
        case UID1:
            Serial.println("Acesso Liberado");
            LedBuiltInState = !LedBuiltInState;
            atualizaLEDs(); // Atualiza os LEDs com os novos estados
            // Move o servo para 90 graus (posição aberta)
            servoMotor.write(90);
            
            break;

        case UID2:
            Serial.println("Cartão 2 reconhecido");
            LedBuiltInState = !LedBuiltInState;
            atualizaLEDs(); // Atualiza os LEDs com os novos estados

            // Move o servo para 90 graus (posição aberta)
            servoMotor.write(0);

            break;

        default:
            Serial.println("Cartão não cadastrado");

            break;
        }
    }
}
