
/**
 * Projeto: Sensor de Cor Baseado em Sensor RGB e Arduino para Automação de Esteira Separadora de Tampinhas
 * 
 * File: s_rgb_helper.cpp
 * Author: Charly Batista <charly@virtlabs.com.br>
 * Date: 2024-12-03
 * Version 0.1
 */

#include "esteira.h"

// 
int maximo, minimo, cor, freq, i;


// 
extern volatile ESTADOS estado;

int get_data();
int calc_cor(SENSOR_INFO *info);
int ler_sensor(COR p_cor);


// 
int ler_cor(SENSOR_INFO *info) {
  info->vm = ler_sensor(VERMELHO);
  delay(35);

  info->vd = ler_sensor(VERDE);
  delay(35);

  info->az = ler_sensor(AZUL);
  delay(35);

  return calc_cor(info);
}

//
int get_data() {
  /* */
  if(digitalRead(OUT) == HIGH){     // Verifica o nível lógico no pino OUT do sensor
    return pulseIn(OUT, LOW);   // Lê duração do pulso na saída
  } else {
    return pulseIn(OUT, HIGH);  // Lê duração do pulso na saída
  }
  /* */

  return pulseIn(OUT, LOW);
}

// 
int ler_sensor(COR p_cor) {
  maximo, minimo, cor, freq = 0;
  
  switch(p_cor) {
    case AZUL:
      digitalWrite(S2, LOW);
      digitalWrite(S3, HIGH);
      break;
    case VERDE:
      digitalWrite(S2, HIGH);
      digitalWrite(S3, HIGH);
      break;
    case VERMELHO:
      digitalWrite(S2, LOW);
      digitalWrite(S3, LOW);
      break;
  }

  for(i = 0; i < 10; i++) {
    freq = get_data();
    
    // Verifica menor valor
    if(!minimo || freq < minimo)
      minimo = freq;

    if(freq > maximo)
      maximo = freq;

    cor += freq;
  }

  cor -= (minimo + maximo);
  cor /= 8;

  return cor;
}

int calc_cor(SENSOR_INFO *info) {

  if ( (info->vm >=  info->vm_min && info->vm <= info->vm_max) && 
      (info->vd >=  info->vd_min && info->vd <= info->vd_max) && 
      (info->az >= info->az_min && info->az <= info->az_max) ) {
    info->cor = SENSOR_TIPO;
    digitalWrite(RELE, LOW);
    delay(10);
    digitalWrite(RELE, HIGH);
  } else {
    info->cor = INDEFINIDA;
  }

  imprime_msg(info);
  return cor;
}
