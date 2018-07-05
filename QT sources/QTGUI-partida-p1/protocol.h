// Cabecera de la implementacion de protocolo de comunicaciones
// El estudiante debera añadir nuevos comandos a la lista de comandos disponibles, asi como
// crear las estructuras adecuadas para los parameros de los comandos.

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

//Caracteres especiales
#define START_FRAME_CHAR 0xFC
#define STOP_FRAME_CHAR 0xFD
#define ESCAPE_CHAR 0xFE
#define STUFFING_MASK 0x20


#define CHEKSUM_TYPE uint16_t
#define COMMAND_TYPE uint8_t

#define CHECKSUM_SIZE (sizeof(CHEKSUM_TYPE))
#define COMMAND_SIZE (sizeof(COMMAND_TYPE))
#define START_SIZE (1)
#define END_SIZE (1)

#define MINIMUN_FRAME_SIZE (START_SIZE+COMMAND_SIZE+CHECKSUM_SIZE+END_SIZE)

#define MAX_DATA_SIZE (40)
#define MAX_FRAME_SIZE (2*(MAX_DATA_SIZE))

//Códigos de los comandos

// El estudiante debe añadir aqui cada nuevo comando que implemente. IMPORTANTE el orden de los comandos
// debe SER EL MISMO aqui, y en el codigo equivalente en la parte del microcontrolador (Code Composer)
typedef enum {
    COMANDO_RECHAZADO,
    COMANDO_PING,
    COMANDO_LEDS,
    COMANDO_BRILLO,
    COMANDO_ROSCA,
    COMANDO_CHECK,
    COMANDO_SONDEO,
    COMANDO_EN,
    COMANDO_ADC, /* SEMANA2 */
    COMANDO_PERIODO,
    COMANDO_STOP,
    COMANDO_OSCILOSCOPIO,
    COMANDO_OSCL,
    COMANDO_ALD,
    COMANDO_MUESTREO,
    COMANDO_PUL

} commandTypes;

//Codigos de Error de protocolo
#define PROT_ERROR_BAD_CHECKSUM (-1)
#define PROT_ERROR_RX_FRAME_TOO_LONG (-2)
#define PROT_ERROR_NOMEM (-3)
#define PROT_ERROR_STUFFED_FRAME_TOO_LONG (-4)
#define PROT_ERROR_COMMAND_TOO_LONG (-5)
#define PROT_ERROR_INCORRECT_PARAM_SIZE (-6)
#define PROT_ERROR_BAD_SIZE (-7)
#define PROT_ERROR_UNIMPLEMENTED_COMMAND (-7)


//Estructuras relacionadas con los parámetros
// El estudiante debera crear y añadir las estructuras adecuadas segun los
// parametros que quiera asociar a los comandos. Las estructuras deben ser iguales en los
// ficheros correspondientes de la parte del micro (Code Composer)

#pragma pack(1)	//Con esto consigo compatibilizar el alineamiento de las estructuras en memoria del PC (32 bits)
                // y del ARM (aunque, en este caso particular no haria falta ya que ambos son 32bit-Little Endian)

#define PACKED


typedef struct {
    unsigned char command;
} PACKED PARAM_COMANDO_RECHAZADO;

typedef union{
    struct {
                uint8_t fRed:1;
                uint8_t fGreen:1;
                uint8_t fBlue:1;
    } PACKED leds;
    uint8_t ui8Valor;
} PACKED PARAM_COMANDO_LEDS;

typedef struct {
    float rIntensity;
} PACKED PARAM_COMANDO_BRILLO;

typedef union{
    struct {
                uint8_t fRojo;
                uint8_t fVerde;
                uint8_t fAzul;
    } PACKED color;
    uint8_t ui8Valor;
} PACKED PARAM_COMANDO_ROSCA;

typedef struct {
    bool check;
} PACKED PARAM_COMANDO_CHECK;
typedef struct {
    uint8_t Izq;
    uint8_t Der;
} PACKED PARAM_COMANDO_SONDEO;
typedef struct {
    bool Check;
} PACKED PARAM_COMANDO_EN;
typedef struct {
    uint8_t Izq;
    uint8_t Der;
} PACKED PARAM_COMANDO_PUL;


typedef struct
{
    uint16_t chan1[8];
    uint16_t chan2[8];
    uint16_t chan3[8];
    uint16_t chan4[8];
} PACKED PARAM_COMANDO_ADC; //SEMANA2

typedef struct
{
    uint16_t chan1[8];
    uint16_t chan2[8];
} PACKED PARAM_COMANDO_OSCL; //SEMANA2
typedef struct {
    double frec;
} PACKED PARAM_COMANDO_PERIODO;

typedef struct {
    uint8_t tick;
} PACKED PARAM_COMANDO_STOP;
typedef struct {
    uint8_t tick;
} PACKED PARAM_COMANDO_OSCILOSCOPIO;
typedef struct {
    double fre;
} PACKED PARAM_COMANDO_MUESTREO;
typedef struct
{
    struct {
        uint8_t chan1[8];
        uint8_t chan2[8];
        uint8_t chan3[8];
        uint8_t chan4[8];
        uint8_t chan5[8];
        uint8_t chan6[8];
        uint8_t chan7[8];
        uint8_t chan8[8];
    } PACKED muestras;
} PACKED PARAM_COMANDO_ALD; /* SEMANA2 */
#pragma pack()	//...Pero solo para los comandos que voy a intercambiar, no para el resto.

//Funciones que permiten decodificar partes de la trama
uint8_t decode_command_type(uint8_t * buffer);
//int32_t check_and_extract_command_param(uint8_t * buffer, int32_t frame_size, uint32_t payload,void *campo);
int32_t check_and_extract_command_param(void *ptrtoparam, int32_t param_size, uint32_t payload,void *param);
int32_t get_command_param_pointer(uint8_t * buffer, int32_t frame_size, void **campo);

//Funciones de la libreria
int create_frame(uint8_t *frame, uint8_t command_type, void * param, int32_t param_size, int32_t max_size);
int destuff_and_check_checksum (uint8_t *frame, int32_t max_size);


#endif
