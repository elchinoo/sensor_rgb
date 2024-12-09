/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: SSD1306.h
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-09
 * Version 0.1
 *
 * Display OLED SSD-1306
 */

#ifndef __EST_SSD1306__
#define __EST_SSD1306__

#include "config.h"
#include "GY31.h"

void SSD1306_initialize();
void SSD1306_tela_principal(SENSOR_CONFIG *info, COR_RGB *cor);
void SSD1306_menu(ESTADOS estado, uint32_t valor);

#endif      // __EST_SSD1306__