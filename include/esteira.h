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

#include "config.h"
#include "GY31.h"
#include "SSD1306.h"

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

} SENSOR_INFO;

//

#endif
