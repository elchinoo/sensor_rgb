/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 * 
 * File: esteira.h
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-03
 * Version 0.1
 */

#ifndef __ESTEIRA_H__
#define __ESTEIRA_H__

#include <Arduino.h>
#include <Wire.h>

// Pinos do TCS230
#define S0 7
#define S1 8
#define S2 9
#define S3 10
#define OUT A1
#define RELE 12

// Potenciometros
#define POT_COR A3

#define BT_CFG 3
#define BT_CONFIRM 2

// Monitor
#define M_SDA A4
#define M_SCL A5

//
#define MIN_TIME_PRESS 500

enum COR {
  AMARELO,
  AZUL,
  VERDE,
  VERMELHO,
  INDEFINIDA
};

enum ESTADOS {
  ST_INIT,
  ST_LEITURA,
  ST_CONFIG_VM_MIN,
  ST_CONFIG_VM_MAX,
  ST_CONFIG_VD_MIN,
  ST_CONFIG_VD_MAX,
  ST_CONFIG_AZ_MIN,
  ST_CONFIG_AZ_MAX,
  ST_RESET,
};

#define SENSOR_TIPO VERMELHO

typedef struct {
  uint16_t vm_min;
  uint16_t vm_max;
  uint16_t vd_min;
  uint16_t vd_max;
  uint16_t az_min;
  uint16_t az_max;
  
  uint16_t vm;
  uint16_t vd;
  uint16_t az;
  COR cor;
} SENSOR_INFO;

// 
void setup_monitor();
void imprime_msg(SENSOR_INFO *info);
void imprime_config(ESTADOS estado, SENSOR_INFO *info);

void reset_config(SENSOR_INFO *info);
void salva_config(SENSOR_INFO *info);
void recupera_config(SENSOR_INFO *info);

void log_serial(SENSOR_INFO *info);

//
int ler_cor(SENSOR_INFO *info);

#endif
