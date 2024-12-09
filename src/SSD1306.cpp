/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 *
 * File: esteira.cpp
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-03
 * Version 0.1
 */

// Adafruit SSD1306

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SSD1306.h"
#include "GY31.h"

//
extern volatile ESTADOS estado;

//
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void SSD1306_initialize()
{
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.clearDisplay();
  display.display();
}

void SSD1306_tela_topo()
{
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 1.5);
  display.print("MOD:");
  display.setTextColor(BLACK, WHITE);
  display.print(" ");
  display.print(GY31_decodeCor(SENSOR_TIPO));
  display.print(" ");
}

void SSD1306_tela_principal(SENSOR_CONFIG *info, COR_RGB *cor)
{

  SSD1306_tela_topo();
  display.setTextColor(WHITE);
  display.setCursor(20, 16);

  // Range cor Vermelho
  display.print("R[");
  display.print(info->r_min);
  display.print(",");
  display.print(info->r_base);
  display.print("]");

  // Range cor Verde
  display.setCursor(20, 26);
  display.print("G[");
  display.print(info->g_min);
  display.print(",");
  display.print(info->g_base);
  display.print("]");

  // Range cor Azul
  display.setCursor(20, 36);
  display.print("B[");
  display.print(info->b_min);
  display.print(",");
  display.print(info->b_base);
  display.print("]");

  // Valores lidos
  display.setCursor(10, 46);
  display.print("LIDO:");
  display.print(cor->red);
  display.print(",");
  display.print(cor->green);
  display.print(",");
  display.print(cor->blue);

  display.setCursor(10, 55);
  display.print("COR: ");
  display.setTextColor(BLACK, WHITE);
  display.print(GY31_corFromRGB(cor, NULL));
  display.print(" ");

  display.display();
}

void SSD1306_menu_case(ESTADOS estado)
{
  switch (estado)
  {
  case ST_CONFIG_R_MIN:
    display.print("Minimo VERMELHO");
    break;
  case ST_CONFIG_R_BASE:
    display.print("Desconto VERMELHO");
    break;
  case ST_CONFIG_G_MIN:
    display.print("Minimo VERDE");
    break;
  case ST_CONFIG_G_BASE:
    display.print("Desconto VERDE");
    break;
  case ST_CONFIG_B_MIN:
    display.print("Minimo AZUL");
    break;
  case ST_CONFIG_B_BASE:
    display.print("Desconto AZUL");
    break;
  case ST_CONFIG_TIME_UPD_DSP:
    display.print("Delay display");
    break;

  default:
    break;
  }
}

void SSD1306_menu(ESTADOS estado, uint32_t valor)
{
  SSD1306_tela_topo();

  display.setTextColor(WHITE);
  // Range cor vermelho
  display.setCursor(20, 16);
  display.print("CONFIG");

  display.setCursor(20, 36);
  SSD1306_menu_case(estado);
  
  display.setCursor(20, 46);
  display.print("[ ");
  display.print(valor);
  display.print(" ]");

  display.display();
}
