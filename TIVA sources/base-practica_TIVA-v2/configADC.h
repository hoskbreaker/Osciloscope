/*
 * configADC.h
 *
 *  Created on: 22/4/2016
 *      Author: jcgar
 */

#ifndef CONFIGADC_H_
#define CONFIGADC_H_

//definimos los flags necesarios para la ISR del DMA
#define DMA0_FLAG 0x0001
#define DMA1_FLAG 0x0002
#define DMA2_FLAG 0x0004


#include<stdint.h>

typedef struct
{
	uint16_t chan1;
	uint16_t chan2;
	uint16_t chan3;
	uint16_t chan4;
} MuestrasADC;

typedef struct
{
	uint32_t chan1;
	uint32_t chan2;
	uint32_t chan3;
	uint32_t chan4;
} MuestrasLeidasADC;

typedef struct
{
    uint8_t Izq;
    uint8_t Der;
} Pulsador;

void configADC_ISR(void);
void configCOMPARADOR_ISR(void);
void configDMA_ISR(void);
void configALD_ISR(void);
void configBOTON_ISR(void);
void configADC_DisparaADC(void);
void configADC_DisparaALD(void);
void configADC_LeeADC(MuestrasADC *datos);
void configBOTON_LeeBOTON(Pulsador *datos);
void configBOTON_iniciaBOTON(void);
void configADC_IniciaADC(void);
void configADC_IniciaCOMPARADOR(void);
void configADC_IniciaDMA(void);
void configADC_frecuenciaADC(double valor);
void configADC_muestreoALD(double valor);


#endif /* CONFIGADC_H_ */
