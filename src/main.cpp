/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: sensor_rgb.ino
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-03
 * Version 0.1
 *
 * Descrição:
 * Este projeto implementa um sensor de cor utilizando um módulo RGB integrado a um Arduino, com o objetivo de automatizar uma
 * esteira separadora de tampinhas. Desenvolvido no contexto do curso de Mecatrônica Industrial no Unisenai/SC,
 * este sistema será integrado a CLPs para ampliar sua aplicação em ambientes industriais.
 *
 * O sistema é controlado por uma máquina de estados finitos, que gerencia os seguintes estados operacionais:
 * - **INICIALIZACAO**: Configuração e validação inicial do sensor.
 * - **LEITURA**: Captura e análise da cor das tampinhas.
 * - **CONFIGURACAO VM**: Ajuste para detecção da cor vermelha.
 * - **CONFIGURACAO VD**: Ajuste para detecção da cor verde.
 * - **CONFIGURACAO AZ**: Ajuste para detecção da cor azul.
 * - **RESET**: Retorno ao estado inicial ou reinicialização do sistema.
 *
 * Características principais:
 * - Integração entre sensores RGB e controladores CLP para aplicações industriais.
 * - Implementação modular baseada em máquina de estados finitos para maior controle e flexibilidade.
 * - Capacidade de configuração dinâmica das cores a serem detectadas.
 * - Projeto educativo com foco na automação e eficiência de processos industriais.
 *
 *
 * Este código é fornecido como material didático e está sujeito a modificações para melhorias ou personalizações de acordo com as necessidades do projeto.
 */

#include "esteira.h"
#include "GY31.h"

//
volatile uint8_t estado;

// Timers
unsigned long old_press;
unsigned long new_press;

// Variaveis referentes aos potenciometros
int pot_cor;

// SENSOR_INFO info;
SENSOR_CONFIG config;

COR_RGB cor;

void int_config();
void int_confirma();
void ler_pot();

void setup()
{
  // Serial.begin(115200); // Serial communication

  // Reseta a variavel de configuracao e le a configuracao da memoria
  memset(&config, 0, sizeof(SENSOR_INFO));
  CFG_initialize(&config);

  // Inicializa o sensor de cores GY-31
  GY31_initialize();

  // Configura e inicializa o display OLED SSD-1306
  SSD1306_initialize();

  /***** Configuracao dos restantes do Arduino *****/

  // Botoes
  pinMode(BT_CFG, INPUT);
  pinMode(BT_CONFIRM, INPUT);

  // Rele
  pinMode(RELE, OUTPUT);

  //
  pinMode(POT_COR, INPUT);

  /***** Setup e inicializacao da maquina de estados (ME) *****/
  estado = ST_INIT;

  // "Desliga" o RELE de interface com o CLP
  digitalWrite(RELE, HIGH);

  // Interrupcoes
  attachInterrupt(digitalPinToInterrupt(BT_CFG), int_config, RISING);
  attachInterrupt(digitalPinToInterrupt(BT_CONFIRM), int_confirma, RISING);

  old_press = 0;

  estado = ST_LEITURA;
}

COR_LEITURA cor_lida;
void loop()
{
  //
  switch (estado)
  {
  //
  case ST_CONFIG_R_MIN:
  case ST_CONFIG_R_BASE:
  case ST_CONFIG_G_MIN:
  case ST_CONFIG_G_BASE:
  case ST_CONFIG_B_MIN:
  case ST_CONFIG_B_BASE:
  case ST_CONFIG_TIME_UPD_DSP:
    ler_pot();
    break;
  case ST_LEITURA:
  default:
    estado = ST_LEITURA;
    GY31_getRGBDdata(&config, &cor);

    cor_lida = INDEFINIDA;
    GY31_corFromRGB(&cor, &cor_lida);

    // Dispara o rele
    if (cor_lida == SENSOR_TIPO)
    {
      // Checa se esta acima do valor minimo para as leituras
      switch (SENSOR_TIPO)
      {
      case VERMELHO:
      case AMARELO:
        if (cor.red < config.r_min)
          return;
        break;

      case VERDE:
        if (cor.green < config.g_min)
          return;
        break;

      case AZUL:
        if (cor.blue < config.b_min)
          return;
        break;
      }
      
      digitalWrite(RELE, LOW);
      delay(5);
      digitalWrite(RELE, HIGH);
    }

    SSD1306_tela_principal(&config, &cor);
    break;
  }
}

void int_config()
{
  new_press = millis();
  if ((new_press - old_press) < MIN_TIME_PRESS)
    return;

  old_press = new_press;
  estado++;

  delay(50);
}

void int_confirma()
{
  switch (estado)
  {
  case ST_CONFIG_R_MIN:
    config.r_min = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  case ST_CONFIG_R_BASE:
    config.r_base = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  case ST_CONFIG_G_MIN:
    config.g_min = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  case ST_CONFIG_G_BASE:
    config.g_base = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  case ST_CONFIG_B_MIN:
    config.b_min = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  case ST_CONFIG_B_BASE:
    config.b_base = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  case ST_CONFIG_TIME_UPD_DSP:
    config.time_upd_display = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);
    break;
  default:
    estado = ST_LEITURA;
  }

  CFG_gravar(&config);
}

void ler_pot()
{
  pot_cor = analogRead(POT_COR);
  uint32_t vlr = map(pot_cor, 0, 1023, MAP_FROM, MAP_TO);

  SSD1306_menu((ESTADOS)estado, vlr);
  delay(100);
}
