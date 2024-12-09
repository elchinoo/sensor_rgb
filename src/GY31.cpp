/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: GY31.cpp
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-09
 * Version 0.1
 *
 * Sensor GY-31 (TCS-3200)
 * Datasheet: https://www.mouser.com/catalog/specsheets/tcs3200-e11.pdf
 *
 * OBS: Os sensores adquiridos correspondem ao GY-31, todavia nao foi possivel encontrar o datasheet do mesmo.
 *      Todos os datasheet encontrados eram dos sensores TCS-230 e TCS-3200.
 *      Os 3 hardwares aparentemente utilizam o mesmo sensor RGB, todavia a construcao eh diferente,
 *          o que ficou evidente com o hardware que estamos utilizando, o qual nao expoe o pino OE (output enable)
 *          mas expoe 2 pinos GND e 2 pinos VCC.
 *      Todos os PDF's de referencia encontrados, como o abaixo, dao uma breve descricao do sensor e dos pinos,
 *          mas sem o mesmo grau de detalhes do datasheet. Por hora vamos trabalhar como sendo os mesmos paramentros
 *          utilizados no TCS-3200.
 *      Ref: https://www.e-gizmo.net/oc/kits%20documents/GY31%20Color%20sensor/GY-31%20Color%20Module.pdf
 *
 *      Operating Voltage Range: 2.7 to 5.5 V DC
 *      Supply Current: 5V LED (approx. 25mA)
 *      Interfaces: Output Frequency
 *      Operating Temperatures: 40°~ 85°
 *      PCB Dimensions: 33.2mm×33.2mm×25mm
 *
 * Pinos:
 * GND: Ground
 * VCC: Supply voltage 2.7v - 5v
 * OUT: Output signal (frequency)
 * S0 : Output frequency scaling selection input
 * S1 : Output frequency scaling selection input
 * S2 : Photodiode type selection input
 * S3 : Photodiode type selection input
 * LED: LED control (1:on, 0:off)
 *
 * Frequency scaling:
 * S0   S1  | Output frequency scaling
 * L    L   | Power down
 * L    H   |   2%
 * H    L   |  20%
 * H    H   | 100%
 *
 * Photodiode sellection:
 * S2   S3  | Photodiode type
 * L    L   | Red
 * L    H   | Blue
 * H    L   | Clear (No filter)
 * H    H   | Green
 *
 */

#include "GY31.h"

void GY31_initialize()
{
    // Configura os pinos
    pinMode(GY31_S0, OUTPUT);
    pinMode(GY31_S1, OUTPUT);
    pinMode(GY31_S2, OUTPUT);
    pinMode(GY31_S3, OUTPUT);
    pinMode(GY31_OUT, INPUT);

    // Seta a escala de leitura para 100% como padrao
    GY31_setReadScale(SCALE_100);
}

void GY31_setReadScale(GY31_READ_SCALE scale)
{
    switch (scale)
    {
    case SCALE_0:
        digitalWrite(GY31_S0, LOW);
        digitalWrite(GY31_S1, LOW);
        break;
    case SCALE_2:
        digitalWrite(GY31_S0, LOW);
        digitalWrite(GY31_S1, HIGH);
        break;
    case SCALE_20:
        digitalWrite(GY31_S0, HIGH);
        digitalWrite(GY31_S1, LOW);
        break;
    case SCALE_100:
        digitalWrite(GY31_S0, HIGH);
        digitalWrite(GY31_S1, HIGH);
        break;
    }
}

void GY31_setPhotoDiodeMode(GY31_FILTRO_COR diodo)
{
    switch (diodo)
    {
    case PHD_VERMELHO:
        digitalWrite(GY31_S2, LOW);
        digitalWrite(GY31_S3, LOW);
        break;

    case PHD_AZUL:
        digitalWrite(GY31_S2, LOW);
        digitalWrite(GY31_S3, HIGH);
        break;

    case PHD_CLEAR:
        digitalWrite(GY31_S2, HIGH);
        digitalWrite(GY31_S3, LOW);
        break;

    case PHD_VERDE:
        digitalWrite(GY31_S2, HIGH);
        digitalWrite(GY31_S3, HIGH);
        break;
    }
}

void GY31_getRGBDdata(const SENSOR_CONFIG *config, COR_RGB *data)
{
    uint32_t tmp_data;

    GY31_setPhotoDiodeMode(PHD_VERMELHO);
    tmp_data = GY31_getSensorData(config->r_base);
    data->red = tmp_data;

    GY31_setPhotoDiodeMode(PHD_AZUL);
    tmp_data = GY31_getSensorData(config->b_base);
    data->blue = tmp_data;

    // setPhotoDiodeMode(PHD_CLEAR);
    // tmp_data = (uint32_t) getSensorData();
    // data->clear = tmp_data;

    GY31_setPhotoDiodeMode(PHD_VERDE);
    tmp_data = GY31_getSensorData(config->g_base);
    data->green = tmp_data;
}

uint32_t GY31_getSensorData(uint32_t thold)
{
    uint8_t ctt;
    uint64_t pulse_low;
    uint32_t freq_map;
    float frequency = 0;

    pulse_low = 0;
    frequency = 0;

    for (ctt = 0; ctt < 50; ctt++)
    {
        pulse_low = pulseIn(GY31_OUT, LOW);
        frequency += (100000 / pulse_low);
        // frequency += pulse_low;
    }
    freq_map = (uint32_t)(frequency / ctt);

    if ((freq_map - thold) > 2147483639)
        return 0;

    return (freq_map  - thold);
}

String GY31_decodeCor(COR_LEITURA cor)
{
    switch (cor)
    {
    case VERMELHO:
        return "VERMELHO";
        break;
    case VERDE:
        return ("VERDE");
        break;
    case AZUL:
        return ("AZUL");
        break;
    case AMARELO:
        return ("AMARELO");
        break;
    default:
        return ("INDEFINIDA");
    }
}

String GY31_corFromRGB(const COR_RGB *rgb, COR_LEITURA *cor)
{

    COR_LEITURA tmp;

    // Quando o vermelho eh a cor predominante pode ser tanto vermelho quanto amarelo
    if ((rgb->red > rgb->green) && (rgb->red > rgb->blue))
    {
        float perc1 = ((float)rgb->green) / ((float)rgb->red);
        float perc2 = ((float)rgb->blue) / ((float)rgb->red);

        // Verificfamos se as outras cores estao presentes em ao menos 40% de proporcionalidade:
        if ((perc1 >= RGB_AMARELO_PROP) && (perc2 >= RGB_AMARELO_PROP))
            tmp = AMARELO;
        else
            tmp = VERMELHO;
    }
    else if ((rgb->green > rgb->red) && (rgb->green > rgb->blue))
    {
        tmp = VERDE;
    }
    else if ((rgb->blue > rgb->green) && (rgb->blue > rgb->red))
    {
        tmp = AZUL;
    }
    else
    {
        tmp = INDEFINIDA;
    }

    if (cor != NULL)
        *cor = tmp;

    return GY31_decodeCor(tmp);
}