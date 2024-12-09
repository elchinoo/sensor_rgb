/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: GY31.h
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-09
 * Version 0.1
 *
 * Sensor GY-31 (TCS-3200)
 */

#ifndef __EST_GY31__
#define __EST_GY31__

#include "config.h"

typedef enum 
{
    PHD_VERMELHO,
    PHD_VERDE,
    PHD_AZUL,
    PHD_CLEAR
} GY31_FILTRO_COR;

typedef enum 
{
    SCALE_0,
    SCALE_2,
    SCALE_20,
    SCALE_100
} GY31_READ_SCALE;

typedef struct
{
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t clear;
} COR_RGB;

#define SENSOR_TIPO VERMELHO

void GY31_initialize();
void GY31_setReadScale(GY31_READ_SCALE scale);
void GY31_setPhotoDiodeMode(GY31_FILTRO_COR diodo);
void GY31_getRGBDdata(const SENSOR_CONFIG *config, COR_RGB *data);
uint32_t GY31_getSensorData(uint32_t thold);
String GY31_decodeCor(COR_LEITURA cor);
String GY31_corFromRGB(const COR_RGB *rgb, COR_LEITURA *cor);

#endif  // __EST_GY31__