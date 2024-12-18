#include <Arduino.h>
#include <Bounce2.h>
#include <Preferences.h>
#include "entradas.h"
#include "saidas.h"

//Definição dos pinos dos botões
#define BOTAO_BOOT_PIN 0
#define BOTAO_EXTERNO_PIN 12

#define tempo_Botao_Pressionado 2000  

//Criação dos objetos para debouncing
Bounce botao_boot = Bounce();
Bounce botaoExterno = Bounce();

extern Preferences preferences;


//Inicializa as entradas digitais
void inicializa_entradas()
{
    botao_boot.attach(BOTAO_BOOT_PIN, INPUT_PULLUP);
    botaoExterno.attach(BOTAO_EXTERNO_PIN, INPUT_PULLUP);

}

//Atualiza o estado dos botões
void atualiza_botoes()
{
    //atualiza do estado do botao_boot
    botao_boot.update();
    botaoExterno.update();
}

//Retorna se o botão boot foi pressionado
bool botao_boot_pressionado()
{
    return botao_boot.fell();
}

//Retorna se o botão boot foi solto
bool botao_boot_solto()
{
    return botao_boot.rose();
}

//Retorna se o botão externo foi pressionado
bool botao_externo_pressionado()
{
    return botaoExterno.fell();
}

void reinicia_flash()
{
    botao_boot.update();  // Atualiza o estado do botão (pressionado ou não)

  if (botao_boot.rose())  // Se o botão foi solto
  {
    if (botao_boot.previousDuration() > tempo_Botao_Pressionado) 
    {
     preferences.clear();  // Limpa as preferências

     Serial.print("Memoria reiniciada");
    }
  }
}