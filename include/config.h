/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: config.h
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-09
 * Version 0.1
 *
 * Sensor GY-31 (TCS-3200)
 */

#ifndef __EST_CONFIG__
#define __EST_CONFIG__

#include <Arduino.h>

// Pinos do TCS230
#define GY31_S0 7
#define GY31_S1 8
#define GY31_S2 9
#define GY31_S3 10
#define GY31_OUT A1

// Pinos do RELE
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
#define CFG_FINGER 0x05E4A1
#define CFG_RESET 0

#define DEFAULT_R_MIN  10
#define DEFAULT_G_MIN  10
#define DEFAULT_B_MIN  10
#define DEFAULT_R_BASE 2280
#define DEFAULT_G_BASE 1900
#define DEFAULT_B_BASE 2555
#define DEFAULT_TIME_UPD_DSP 500
#define RGB_AMARELO_PROP 0.52

typedef enum
{
    AMARELO,
    AZUL,
    VERDE,
    VERMELHO,
    INDEFINIDA
} COR_LEITURA;

typedef enum {
  ST_INIT,
  ST_LEITURA,
  ST_CONFIG_R_MIN,
  ST_CONFIG_R_BASE,
  ST_CONFIG_G_MIN,
  ST_CONFIG_G_BASE,
  ST_CONFIG_B_MIN,
  ST_CONFIG_B_BASE,
  ST_CONFIG_TIME_UPD_DSP,
} ESTADOS;

typedef struct
{
    // Valores minimos pra considerar que um objeto passou pelo sensor
    // Dependem do sensor e da iluminacao do ambiente
    uint32_t r_min;
    uint32_t g_min;
    uint32_t b_min;
    
    // Valores para serem deduzidos quando for fazer os calculos de cores
    // Dependem do sensor e da iluminacao do ambiente
    uint32_t r_base;
    uint32_t g_base;
    uint32_t b_base;

    // Finger print pra identificar se a configuracao ja foi inicializada
    uint32_t finger;

    // 
    uint16_t time_upd_display;
    COR_LEITURA cor;
} SENSOR_CONFIG;

void CFG_initialize(SENSOR_CONFIG *config);
void CFG_gravar(SENSOR_CONFIG *config);
void CFG_ler(SENSOR_CONFIG *config);

#endif  // __EST_CONFIG__