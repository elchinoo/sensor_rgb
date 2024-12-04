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
#include<EEPROM.h>
#include "esteira.h"

// 
extern volatile ESTADOS estado;

// 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *decode_cor(COR cor);

void setup_monitor() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.clearDisplay();
}

void imprime_msg(SENSOR_INFO *info) {
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 2);
  display.print("MOD:");
  display.setTextColor(BLACK, WHITE);
  display.print(" ");
  display.print(decode_cor(SENSOR_TIPO));
  display.print(" ");
  
  display.setTextColor(WHITE);
  // Range cor vermelho
  display.setCursor(20, 12);
  display.print("R [");
  display.print(info->vm_min);
  display.print("-");
  display.print(info->vm_max);
  display.print("]");

  // Range cor Verde
  display.setCursor(20, 22);
  display.print("G [");
  display.print(info->vd_min);
  display.print("-");
  display.print(info->vd_max);
  display.print("]");

  // Range cor Azul
  display.setCursor(20, 32);
  display.print("B [");
  display.print(info->az_min);
  display.print("-");
  display.print(info->az_max);
  display.print("]");
  
  // Valores lidos
  display.setCursor(10, 42);
  display.print("LIDO:");
  display.print(info->vm);
  display.print(", ");
  display.print(info->vd);
  display.print(", ");
  display.print(info->az);
  
  display.setCursor(10, 52);
  display.print("COR: ");
  display.setTextColor(BLACK, WHITE);
  display.print(" ");
  display.print(decode_cor(info->cor));
  display.print(" ");

  display.display();

  // log_serial(info);
}

void imprime_config(ESTADOS estado, SENSOR_INFO *info) {
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(10, 2);
  display.print("MOD:");
  display.setTextColor(BLACK, WHITE);
  display.print(" ");
  display.print(decode_cor(SENSOR_TIPO));
  display.print(" ");
  
  display.setTextColor(WHITE);
  // Range cor vermelho
  display.setCursor(20, 12);
  display.print("CONFIG");

  display.setCursor(20, 32);
  switch(estado) {
    case ST_CONFIG_VM_MIN: display.print("VERMELHO");
      display.setCursor(40, 42);
      display.print("MIN: ");
      display.print(info->vm_min);
      break;
    case ST_CONFIG_VM_MAX: display.print("VERMELHO");
      display.setCursor(40, 42);
      display.print("MAX: ");
      display.print(info->vm_max);
      break;
    case ST_CONFIG_VD_MIN: display.print("VERDE"); 
      display.setCursor(40, 42);
      display.print("MIN: ");
      display.print(info->vd_min);
      break;
    case ST_CONFIG_VD_MAX: display.print("VERDE"); 
      display.setCursor(40, 42);
      display.print("MAX: ");
      display.print(info->vd_max);
      break;
    case ST_CONFIG_AZ_MIN: display.print("AZUL"); 
      display.setCursor(40, 42);
      display.print("MIN: ");
      display.print(info->az_min);
      break;
    case ST_CONFIG_AZ_MAX: display.print("AZUL"); 
      display.setCursor(40, 42);
      display.print("MAX: ");
      display.print(info->az_max);
      break;
  }

  display.display();
}


void log_serial(SENSOR_INFO *info) {
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(info->vm);//printing RED cor frequency
  Serial.print("  ");

  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(info->vd);//printing RED cor frequency
  Serial.print("  ");

  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(info->az);//printing RED cor frequency
  Serial.print("  ");

  Serial.print("  | ");
  Serial.println(decode_cor(info->cor));
}

const char *decode_cor(COR cor) {
  switch(cor) {
    case VERMELHO: return "VERMELHO"; break;
    case VERDE: return ("VERDE"); break;
    case AZUL: return ("AZUL"); break;
    case AMARELO: return ("AMARELO"); break;
    default: return ("INDEFINIDA");
  }
}


void reset_config(SENSOR_INFO *info) {
  info->vm_min = 300;
  info->vm_max = 350;
  info->vd_min = 400;
  info->vd_max = 450;
  info->az_min = 290;
  info->az_max = 350;
  EEPROM.put(0x0, *info);
  estado = ST_LEITURA;
}

void salva_config(SENSOR_INFO *info) {
  EEPROM.put(0x0, *info);
}

void recupera_config(SENSOR_INFO *info) {
  SENSOR_INFO tmp;
  EEPROM.get(0x0, tmp);
  info->vm_min = tmp.vm_min;
  info->vm_max = tmp.vm_max;
  info->vd_min = tmp.vd_min;
  info->vd_max = tmp.vd_max;
  info->az_min = tmp.az_min;
  info->az_max = tmp.az_max;
}
