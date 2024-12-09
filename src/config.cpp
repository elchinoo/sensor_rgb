/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: config.cpp
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-09
 * Version 0.1
 */

#include <EEPROM.h>
#include "config.h"

#define EEPROM_ADDR_0 0
#define EEPROM_BLK_SIZE sizeof(SENSOR_CONFIG)

void static CFG_set_config_str(const SENSOR_CONFIG *base_config, SENSOR_CONFIG *new_config)
{
    new_config->r_base = base_config->r_base;
    new_config->g_base = base_config->g_base;
    new_config->b_base = base_config->b_base;
    new_config->cor = base_config->cor;
    new_config->finger = base_config->finger;
}

void CFG_initialize(SENSOR_CONFIG *config)
{
    SENSOR_CONFIG tmp;
    CFG_ler(&tmp);
    if (tmp.finger != CFG_FINGER || CFG_RESET == 1)
    {
        tmp.r_base = DEFAULT_R_BASE;
        tmp.g_base = DEFAULT_G_BASE;
        tmp.b_base = DEFAULT_B_BASE;
        tmp.cor = INDEFINIDA;
        tmp.time_upd_display = DEFAULT_TIME_UPD_DSP;
        tmp.finger = CFG_FINGER;
        CFG_gravar(&tmp);
    }

    CFG_set_config_str((const SENSOR_CONFIG *)&tmp, config);
}

void CFG_gravar(SENSOR_CONFIG *config)
{
    SENSOR_CONFIG tmp;
    tmp.r_base = config->r_base;
    tmp.g_base = config->g_base;
    tmp.b_base = config->b_base;
    tmp.cor = config->cor;
    tmp.finger = config->finger;

    EEPROM.put(EEPROM_ADDR_0, tmp);
}

void CFG_ler(SENSOR_CONFIG *config)
{
    SENSOR_CONFIG tmp;
    EEPROM.get(EEPROM_ADDR_0, tmp);
    CFG_set_config_str((const SENSOR_CONFIG *)&tmp, config);
}
