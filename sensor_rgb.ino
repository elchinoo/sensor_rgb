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

// 
volatile ESTADOS estado;

// Timers
unsigned long old_press;
unsigned long new_press;

// Variaveis referentes aos potenciometros
int pot_cor;

SENSOR_INFO info;

void int_config();
void int_reset();
void ler_pot();

void setup() {
  Serial.begin(9600); // Serial communication

  /***** Configuracao dos pinos do arduino *****/
  // Sensor 
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Botoes
  pinMode(BT_CFG, INPUT);
  pinMode(BT_CONFIRM, INPUT);

  // Rele
  pinMode(RELE, OUTPUT);

  //
  pinMode(POT_COR, INPUT);

  /***** Setup e inicializacao da maquina de estados (ME) *****/
  
  // Configura o monitor LCD
  estado = ST_INIT;
  setup_monitor();

  // Carrega as configuracoes salvas da memoria EEPROM
  memset(&info, 0, sizeof(SENSOR_INFO));
  recupera_config(&info);
  
  // Configura a frequência de saída do sensor RGB para 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // "Desliga" o RELE de interface com o CLP
  digitalWrite(RELE, HIGH);

  // Interrupcoes
  attachInterrupt(digitalPinToInterrupt(BT_CFG), int_config, RISING);
  attachInterrupt(digitalPinToInterrupt(BT_CONFIRM), int_confirma, RISING);

  old_press = 0;

  estado = ST_LEITURA;

  //reset_config(&info);
}

void loop() {
  // 
  switch(estado) {
    // 
    case ST_CONFIG_VM_MIN:
    case ST_CONFIG_VM_MAX:
    case ST_CONFIG_VD_MIN:
    case ST_CONFIG_VD_MAX:
    case ST_CONFIG_AZ_MIN:
    case ST_CONFIG_AZ_MAX: ler_pot(); break;
    //
    // case ST_RESET: reset_config(&info); break;
    //
    // 
    case ST_LEITURA: 
    default: ler_cor(&info); break;
  }
}


void int_config() {
  new_press = millis();
  if((new_press - old_press) < MIN_TIME_PRESS)
    return;

  old_press = new_press;
  
  switch(estado) {
    case ST_CONFIG_VM_MIN: estado = ST_CONFIG_VM_MAX; break;
    case ST_CONFIG_VM_MAX: estado = ST_CONFIG_VD_MIN; break;
    case ST_CONFIG_VD_MIN: estado = ST_CONFIG_VD_MAX; break;
    case ST_CONFIG_VD_MAX: estado = ST_CONFIG_AZ_MIN; break;
    case ST_CONFIG_AZ_MIN: estado = ST_CONFIG_AZ_MAX; break;
    case ST_CONFIG_AZ_MAX: estado = ST_LEITURA; return; break;
    default: estado = ST_CONFIG_VM_MIN;
  }
  
  delay(50);
}

void int_confirma() {
  switch(estado) {
    case ST_CONFIG_VM_MIN: info.vm_min = map(pot_cor, 0, 1023, 0, 700); break;
    case ST_CONFIG_VM_MAX: info.vm_max = map(pot_cor, 0, 1023, 0, 700); break;
    case ST_CONFIG_VD_MIN: info.vd_min = map(pot_cor, 0, 1023, 0, 700); break;
    case ST_CONFIG_VD_MAX: info.vd_max = map(pot_cor, 0, 1023, 0, 700); break;
    case ST_CONFIG_AZ_MIN: info.az_min = map(pot_cor, 0, 1023, 0, 700); break;
    case ST_CONFIG_AZ_MAX: info.az_max = map(pot_cor, 0, 1023, 0, 700); break;
    case ST_RESET: reset_config(&info); break;
    default: estado = ST_LEITURA;
  }
}

void ler_pot() {
  Serial.println("LER POT");
  pot_cor = analogRead(POT_COR);

  imprime_config(estado, &info);
  delay(100);
}
