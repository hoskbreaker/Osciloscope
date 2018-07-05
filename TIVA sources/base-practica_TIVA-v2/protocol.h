/* protocol.h
 *
 * Este fichero define las funciones para implementar la comunicacion mediante
 * el protocolo propuesto
 *
 * Los mensajes que se envian por el perfil USB constan de un byte de INICIO,
 * un byte de comando, un campo de datos opcional de longitud variable que depende del comando,
 * un CRC de 2 bytes y un byte de FIN.
 *
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include<stdlib.h>
#include<string.h>
#include<FreeRTOS.h>

//Caracteres especiales
#define START_FRAME_CHAR 0xFC
#define STOP_FRAME_CHAR 0xFD
#define ESCAPE_CHAR 0xFE
#define STUFFING_MASK 0x20

//Tipos de los campos
#define CHEKSUM_TYPE uint16_t
#define COMMAND_TYPE uint8_t

#define CHECKSUM_SIZE (sizeof(CHEKSUM_TYPE))
#define COMMAND_SIZE (sizeof(COMMAND_TYPE))
#define START_SIZE (1)
#define END_SIZE (1)

#define MINIMUN_FRAME_SIZE (START_SIZE+COMMAND_SIZE+CHECKSUM_SIZE+END_SIZE)

#define MAX_DATA_SIZE (40)
#define MAX_FRAME_SIZE (2*(MAX_DATA_SIZE))

//Codigos de Error del protocolo
#define PROT_ERROR_BAD_CHECKSUM (-1)
#define PROT_ERROR_RX_FRAME_TOO_LONG (-2)
#define PROT_ERROR_NOMEM (-3)
#define PROT_ERROR_STUFFED_FRAME_TOO_LONG (-4)
#define PROT_ERROR_COMMAND_TOO_LONG (-5)
#define PROT_ERROR_INCORRECT_PARAM_SIZE (-6)
#define PROT_ERROR_BAD_SIZE (-7)
#define PROT_ERROR_UNIMPLEMENTED_COMMAND (-7)
//Codigos de los comandos. EL estudiante deberÃ¡ definir los cÃ³digos para los comandos que vaya
// a crear y usar. Estos deberan ser compatibles con los usados en la parte Qt
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
	//etc, etc...
} commandTypes;

//Estructuras relacionadas con los parametros de los comandos. El estuadiante debera crear las
// estructuras adecuadas a los comandos usados, y asegurarse de su compatibilidad con el extremo Qt
//#pragma pack(1)	//Con esto consigo que el alineamiento de las estructuras en memoria del PC (32 bits) no tenga relleno.
#define PACKED __attribute__ ((packed))

typedef struct {
	uint8_t command;
} PACKED PARAM_COMANDO_RECHAZADO;

typedef union{
	struct {
		 uint8_t red:1;
		 uint8_t green:1;
		 uint8_t blue:1;
	} PACKED leds;
    uint8_t valor;
} PACKED PARAM_COMANDO_LEDS;

typedef struct {
    float rIntensity;
} PACKED PARAM_COMANDO_BRILLO;
typedef struct {
    bool Check;
} PACKED PARAM_COMANDO_CHECK;
typedef struct {
    bool Check;
} PACKED PARAM_COMANDO_EN;
typedef struct {
    uint8_t Izq;
    uint8_t Der;
} PACKED PARAM_COMANDO_PUL;
typedef union{
    struct {
         uint8_t Rojo;
         uint8_t Verde;
         uint8_t Azul;
    } PACKED rosca;
    uint8_t valor;
} PACKED PARAM_COMANDO_ROSCA;
typedef struct {
    double fre;
} PACKED PARAM_COMANDO_MUESTREO;
typedef struct
{
    uint16_t chan1[8];
    uint16_t chan2[8];
    uint16_t chan3[8];
    uint16_t chan4[8];
} PACKED PARAM_COMANDO_ADC; /* SEMANA2 */

typedef struct
{
    uint16_t chan1[8];
    uint16_t chan2[8];
} PACKED PARAM_COMANDO_OSCL; /* SEMANA2 */

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
} PACKED PARAM_COMANDO_ALD; /* SEMANA4 */

//Aqui puedo a�adir comandos....
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
    uint8_t Izq;
    uint8_t Der;
} PACKED PARAM_COMANDO_SONDEO;
//Aqui puedo aï¿½adir comandos....

//#pragma pack()	//...Pero solo para los comandos que voy a intercambiar, no para el resto.

//Funciones que obtienen campos del paquete
uint8_t decode_command_type(uint8_t * buffer);
int32_t check_and_extract_command_param(void *ptrtoparam, int32_t param_size, uint32_t payload,void *param);
int32_t get_command_param_pointer(uint8_t * buffer, int32_t frame_size, void **campo);

//Funciones de la libreria
int32_t create_frame(uint8_t *frame, uint8_t command_type, void * param, int32_t param_size, int32_t max_size);
int32_t send_frame(uint8_t *frame, int32_t FrameSize);
int32_t receive_frame(uint8_t *frame, int32_t maxFrameSize);
int32_t destuff_and_check_checksum(uint8_t *frame, int32_t max_size);




#endif
