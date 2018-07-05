/*
 * remote.c
 *
 *  Created on: 1/4/2016
 *      Author: jcgar
 */

#include"remote.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "drivers/rgb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "configADC.h"
#include "driverlib/udma.h"

static uint8_t frame[MAX_FRAME_SIZE];	//Usar una global permite ahorrar pila en la tarea, pero hay que tener cuidado!!!!
static uint32_t gRemoteProtocolErrors=0;
uint32_t ui32Period2;
extern xSemaphoreHandle mutex;
static uint8_t ok;
static int rgb;

//Defino a un tipo que es un puntero a funcion con el prototipo que tienen que tener las funciones que definamos
typedef int32_t (*remote_fun)(uint32_t param_size, void *param);


//Funcion que se ejecuta cuando llega un paquete indicando comando rechazado
int32_t ComandoRechazadoFun(uint32_t param_size, void *param)
{
	//He recibido "Comando rechazado" desde el PC
	//TODO, por hacer: Tratar dicho error??
	gRemoteProtocolErrors++;
	return 0;
}


//Funcion que se ejecuta cuando llega un PING
int32_t ComandoPingFun(uint32_t param_size, void *param)
{
	int32_t numdatos;

	numdatos=create_frame(frame,COMANDO_PING,0,0,MAX_FRAME_SIZE);
	if (numdatos>=0)
	{
		send_frame(frame,numdatos);
	}

	return numdatos;
}

int32_t ComandoSondeo(uint32_t param_size, void *param)
{
   PARAM_COMANDO_SONDEO respuesta;
   uint8_t uint8Buttons,uint8Changed;
   ButtonsPoll(&uint8Changed,&uint8Buttons);
   respuesta.Izq=0;
   respuesta.Der=0;
   if(LEFT_BUTTON & uint8Buttons){
       respuesta.Izq=1;
   }
   if(RIGHT_BUTTON & uint8Buttons){
       respuesta.Der=1;
   }
   if(!ok){
       RemoteSendCommand(COMANDO_SONDEO,(void *)&respuesta,sizeof(respuesta));
   }

   return 0;   //Devuelve Ok (valor mayor no negativo)
}

//Funcion que se ejecuta cuando llega el comando que configura los LEDS
int32_t ComandoLedsFun(uint32_t param_size, void *param)
{
	PARAM_COMANDO_LEDS parametro;
	uint32_t ulColors[3] = {0x0000, 0x0000, 0x0000 };

	if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
	{
		//Ahora mismo se hace usando el PWM --> TODO: Cambiar a GPIO para cumplir las especificaciones
		ulColors[0]= parametro.leds.red ? 0x8000 : 0x0000;
		ulColors[1]=parametro.leds.green ? 0x8000 : 0x0000;
		ulColors[2]= parametro.leds.blue ? 0x8000 : 0x0000;

		RGBColorSet(ulColors);

		return 0;	//Devuelve Ok (valor mayor no negativo)
	}
	else
	{
		return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
	}
}
int32_t ComandoRoscaFun(uint32_t param_size, void *param)
{
    PARAM_COMANDO_ROSCA parametro;
    uint32_t ulColors[3] = {0x0000, 0x0000, 0x0000 };
    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        //Ahora mismo se hace usando el PWM --> TODO: Cambiar a GPIO para cumplir las especificaciones
        ulColors[0]= ((uint16_t)parametro.rosca.Rojo)<<8;
        ulColors[1]=((uint16_t)parametro.rosca.Verde)<<8;
        ulColors[2]= ((uint16_t)parametro.rosca.Azul)<<8;


        if(rgb){
            RGBColorSet(ulColors);
        }

        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}
//Funcion que se ejecuta cuando recibimos un comando que no tenemos aun implementado
int32_t ComandoNoImplementadoFun(uint32_t param_size, void *param)
{
	return PROT_ERROR_UNIMPLEMENTED_COMMAND; /* Devuelve un error para que lo procese la tarea que recibe los comandos */
}


//Funcion que se ejecuta cuando llega el comando que configura los LEDS
int32_t ComandoBrilloLedsFun(uint32_t param_size, void *param)
{
	PARAM_COMANDO_BRILLO parametro;


	if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
	{


		RGBIntensitySet(parametro.rIntensity);

		return 0;	//Devuelve Ok (valor mayor no negativo)
	}
	else
	{
		return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
	}
}
int32_t ComandoCheck(uint32_t param_size, void *param)
{
    PARAM_COMANDO_CHECK checked;

    if (check_and_extract_command_param(param, param_size, sizeof(checked),&checked)>0)
    {
        if(checked.Check){
               rgb=0;
               RGBDisable();
               ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
            }else{
               rgb=1;
               RGBEnable();
            }
        return 0;
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

int32_t ComandoEnableBoton(uint32_t param_size, void *param)
{
    PARAM_COMANDO_EN checked;

    if (check_and_extract_command_param(param, param_size, sizeof(checked),&checked)>0)
    {
        if(checked.Check){
            GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
            ok=0;
        }else{
            GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
            ok=1;
        }
        return 0;
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}
int32_t ComandoADCFun(uint32_t param_size, void *param)
{
    if (param_size!=0) return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error si viene con datos (no los esperamos)

    configADC_DisparaADC(); //Dispara la conversion

    return 0;
}

int32_t ComandoALDFun(uint32_t param_size, void *param)
{
    if (param_size!=0) return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error si viene con datos (no los esperamos)

    configADC_DisparaALD(); //Dispara la conversion

    return 0;
}

int32_t ComandoPeriodoFun(uint32_t param_size, void *param)
{
    PARAM_COMANDO_PERIODO parametro;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        configADC_frecuenciaADC(parametro.frec);
        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}
int32_t ComandoMUESTREOFun(uint32_t param_size, void *param)
{
    PARAM_COMANDO_MUESTREO parametro;

    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        configADC_muestreoALD(parametro.fre);
        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

int32_t ComandoRTCFun(uint32_t param_size, void *param)
{
    PARAM_COMANDO_STOP parametro;
    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        if(parametro.tick==1){
            TimerDisable(TIMER2_BASE, TIMER_A);
        }else{
            TimerEnable(TIMER2_BASE, TIMER_A);
        }
        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}

int32_t ComandoMODOFun(uint32_t param_size, void *param)
{
    PARAM_COMANDO_OSCILOSCOPIO parametro;
    if (check_and_extract_command_param(param, param_size, sizeof(parametro),&parametro)>0)
    {
        if(parametro.tick==1){
            //cambio de OSCILOSCOPIO a RTC
            TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period2 -1);
            ADCComparatorReset(ADC0_BASE,2,true,true); //Se olvida de lo anterior
            ADCComparatorReset(ADC1_BASE,2,true,true); //Se olvida de lo anterior
            ADCSequenceDisable(ADC0_BASE,2); //DESACTIVO LA SECUENCIA
            ADCSequenceDisable(ADC0_BASE,3); //DESACTIVO LA SECUENCIA
            ADCSequenceDisable(ADC1_BASE,2); //DESACTIVO LA SECUENCIA
            ADCSequenceDisable(ADC1_BASE,3); //DESACTIVO LA SECUENCIA
            ADCIntClear(ADC0_BASE,1);//LIMPIAMOS EL FLAG DE INTERRUPCIONES
            ADCSequenceEnable(ADC0_BASE,1);

        }else{
            //cambio de RTC a OSCILOSCOPIO
            ADCSequenceDisable(ADC0_BASE,1);
            ADCComparatorIntClear(ADC0_BASE,2);
            ADCComparatorIntClear(ADC1_BASE,2);
            ADCSequenceEnable(ADC0_BASE,2); //ACTIVO LA SECUENCIA
            ADCSequenceEnable(ADC0_BASE,2); //ACTIVO LA SECUENCIA
            ADCComparatorReset(ADC0_BASE,2,true,true); //Se olvida de lo anterior
            ADCComparatorReset(ADC1_BASE,2,true,true); //Se olvida de lo anterior
            ADCComparatorIntEnable(ADC0_BASE,2);
            ADCComparatorIntEnable(ADC1_BASE,2);
//            ADCSequenceEnable(ADC1_BASE,3); //ACTIVO LA SECUENCIA
//            ADCComparatorReset(ADC1_BASE,3,true,true); //Se olvida de lo anterior
        }
        return 0;   //Devuelve Ok (valor mayor no negativo)
    }
    else
    {
        return PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
    }
}
/* Array que contiene las funciones que se van a ejecutar en respuesta a cada comando */
static const remote_fun remote_fun_array[]={
		ComandoRechazadoFun, /* Responde al paquete comando rechazado */
		ComandoPingFun, /* Responde al comando ping */
		ComandoLedsFun, /* Responde al comando LEDS */
		ComandoBrilloLedsFun, /* Responde al comando Brillo */
		ComandoRoscaFun,
		ComandoCheck,
		ComandoSondeo,
		ComandoEnableBoton,
		ComandoADCFun, /* SEMANA2*/
		ComandoPeriodoFun,
		ComandoRTCFun,/* SEMANA3*/
		ComandoMODOFun,
		ComandoNoImplementadoFun,
		ComandoALDFun,/* SEMANA4*/
		ComandoMUESTREOFun,
		ComandoNoImplementadoFun,

};

// Codigo para procesar los comandos recibidos a traves del canal USB del micro ("conector lateral")

//Esta tarea decodifica los comandos y ejecuta la funciï¿½n que corresponda a cada uno de ellos (por posicion)
//Tambiï¿½n gestiona posibles errores en la comunicacion
static portTASK_FUNCTION( CommandProcessingTask, pvParameters ){

	//Frame es global en este fichero, se reutiliza en las funciones que envian respuestas ---> CUIDADO!!!

	int32_t numdatos;
	uint8_t command;
	void *ptrtoparam;

	/* The parameters are not used. (elimina el warning)*/
	( void ) pvParameters;

	for(;;)
	{
		numdatos=receive_frame(frame,MAX_FRAME_SIZE);
		if (numdatos>0)
		{	//Si no hay error, proceso la trama que ha llegado.
			numdatos=destuff_and_check_checksum(frame,numdatos);
			if (numdatos<0)
			{
				//Error de checksum (PROT_ERROR_BAD_CHECKSUM), ignorar el paquete
				gRemoteProtocolErrors++;
				// Procesamiento del error (TODO, POR HACER!!)
			}
			else
			{
				//El paquete esta bien, luego procedo a tratarlo.
				command=decode_command_type(frame);
				numdatos=get_command_param_pointer(frame,numdatos,&ptrtoparam);

				if (command<(sizeof(remote_fun_array)/sizeof(remote_fun)))
				{
					switch(remote_fun_array[command](numdatos,ptrtoparam))
					{
						//La funcion puede devolver cï¿½digos de error.
					    //Se procesarï¿½an a continuaciï¿½n
						case PROT_ERROR_NOMEM:
						{
							// Procesamiento del error NO MEMORY (TODO, por hacer)
						}
						break;
						case PROT_ERROR_STUFFED_FRAME_TOO_LONG:
						{
							// Procesamiento del error STUFFED_FRAME_TOO_LONG (TODO, por hacer)
						}
						break;
						case PROT_ERROR_COMMAND_TOO_LONG:
						{
							// Procesamiento del error COMMAND TOO LONG (TODO, por hacer)
						}
						break;
						case PROT_ERROR_INCORRECT_PARAM_SIZE:
						{
							// Procesamiento del error INCORRECT PARAM SIZE (TODO, por hacer)
						}
						break;
						case PROT_ERROR_UNIMPLEMENTED_COMMAND:
						{
							PARAM_COMANDO_RECHAZADO parametro;

							parametro.command=command;
							//El comando esta bien pero no esta implementado
							numdatos=create_frame(frame,COMANDO_RECHAZADO,&parametro,sizeof(parametro),MAX_FRAME_SIZE);
							if (numdatos>=0)
							{
									send_frame(frame,numdatos);
							}
						}
						break;
						//Aï¿½adir casos de error aqui...
						default:
							/* No hacer nada */
							break;
					}
				}
				else
				{
						/* El comando no es reconocido por el microcontrolador */
						ComandoNoImplementadoFun(numdatos,ptrtoparam);
						gRemoteProtocolErrors++;
				}
			}
		}
		else
		{ // if (numdatos >0)
				//Error de recepcion de trama(PROT_ERROR_RX_FRAME_TOO_LONG), ignorar el paquete
				gRemoteProtocolErrors++;
				// Procesamiento del error (TODO)
		}
	}
}

//SEMANA2: añadida para facilitar el envio de datos a diversas tareas. IMPORTANTE!!! Leer los comentarios que hay abajo
//Ojo!! Frame es global (para ahorrar memoria de pila en las tareas) --> Se deben tomar precauciones al usar esta función en varias tareas
//IDEM en lo que respecta al envio por el puerto serie desde varias tareas....
//Estas precauciones no se han tomado en este codigo de partida, pero al realizar la practica se deberian tener en cuenta....
int32_t RemoteSendCommand(uint8_t comando,void *parameter,int32_t paramsize)
{
    int32_t numdatos;

    xSemaphoreTake(mutex,portMAX_DELAY);
    numdatos=create_frame(frame,comando,parameter,paramsize,MAX_FRAME_SIZE);
    if (numdatos>=0)
    {
        send_frame(frame,numdatos);
    }
    xSemaphoreGive(mutex);

    return numdatos;
}



//Inicializa la tarea que recibe comandos (se debe llamar desde main())
void RemoteInit(void)
{
	//
	// Crea la tarea que gestiona los comandos USB (definidos en CommandProcessingTask)
	//
	if(xTaskCreate(CommandProcessingTask, (portCHAR *)"usbser",REMOTE_TASK_STACK, NULL, REMOTE_TASK_PRIORITY, NULL) != pdTRUE)
	{
		while(1);
	}

}
