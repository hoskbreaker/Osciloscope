//*****************************************************************************
//
// Codigo de partida Practica 1.
// Autores: Eva Gonzalez, Ignacio Herrero, Jose Manuel Cano
//
//*****************************************************************************

#include<stdbool.h>
#include<stdint.h>

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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "utils/cpu_usage.h"

#include "drivers/rgb.h"
#include "usb_dev_serial.h"
#include "protocol.h"
#include "remote.h"

#include "inc/hw_udma.h"
#include "driverlib/udma.h"
#include"configADC.h"
#include "event_groups.h"
#include "inc/hw_adc.h"
#include "inc/hw_gpio.h"

#define LED1TASKPRIO 1
#define LED1TASKSTACKSIZE 128

//Globales

uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;

extern uint16_t canal_17[512];
extern uint16_t canal_27[512];
extern uint8_t destino[1024];
//Variable para poner un punto de rupture+a
extern uint32_t ui32Period2;
static volatile uint16_t orig;
xSemaphoreHandle mutex;
extern EventGroupHandle_t FlagsEventos;

extern void vUARTTask( void *pvParameters );


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}

#endif

//*****************************************************************************
//
// Aqui incluimos los "ganchos" a los diferentes eventos del FreeRTOS
//
//*****************************************************************************

//Esto es lo que se ejecuta cuando el sistema detecta un desbordamiento de pila
//
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
	//
	// This function can not return, so loop forever.  Interrupts are disabled
	// on entry to this function, so no processor interrupts will interrupt
	// this loop.
	//
	while(1)
	{
	}
}

//Esto se ejecuta cada Tick del sistema. LLeva la estadistica de uso de la CPU (tiempo que la CPU ha estado funcionando)
void vApplicationTickHook( void )
{
	static unsigned char count = 0;

	if (++count == 10)
	{
		g_ui32CPUUsage = CPUUsageTick();
		count = 0;
	}
	//return;
}

//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationIdleHook (void)
{
	SysCtlSleep();
}


//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationMallocFailedHook (void)
{
	while(1);
}



//*****************************************************************************
//
// A continuacion van las tareas...
//
//*****************************************************************************

// El codigo de esta tarea esta definida en el fichero command.c, es la que se encarga de procesar los comandos del interprete a traves
// del terminal serie (puTTY)
//Aqui solo la declaramos para poderla referenciar en la funcion main
extern void vUARTTask( void *pvParameters );



// Codigo de tarea de ejemplo: eliminar para la aplicacion final
static portTASK_FUNCTION(LEDTask,pvParameters)
		{

	int32_t estado_led=0;

	//
	// Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
	//
	while(1)
	{
		estado_led=!estado_led;

		if (estado_led)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , GPIO_PIN_1);
			vTaskDelay(0.1*configTICK_RATE_HZ);        //Espera del RTOS (eficiente, no gasta CPU)
			//Esta espera es de unos 100ms aproximadamente.
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE,  GPIO_PIN_1,0);
			vTaskDelay(2*configTICK_RATE_HZ);        //Espera del RTOS (eficiente, no gasta CPU)
			//Esta espera es de unos 2s aproximadamente.
		}
	}
}
static portTASK_FUNCTION(BOTONTask,pvParameters)
{
    while(1)
    {
        Pulsador param;//como Pulsador es igual que PARAM_COMANDO_PUL, lo vamos a usar sin que cause problemas
        configBOTON_LeeBOTON(&param);
        RemoteSendCommand(COMANDO_PUL,(void *)&param,sizeof(param));
    }
}

// SEMANA2: Tarea que envia los datos del MICRO al PC (podria hacer algo mas)
static portTASK_FUNCTION(ADCTask,pvParameters)
{

    PARAM_COMANDO_ADC parametro;
    MuestrasADC muestras;
    //
    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    //
    int i=0;
    while(1)
    {
        configADC_LeeADC(&muestras);    //Espera y lee muestras del ADC (BLOQUEANTE)
        parametro.chan1[i]=muestras.chan1;
        parametro.chan2[i]=muestras.chan2;
        parametro.chan3[i]=muestras.chan3;
        parametro.chan4[i]=muestras.chan4;

        //Aprovechamos que las estructuras MuestrasADC y PARAM_COMANDO_ADC son iguales para mandarlas directamente;
        if(i==7){
            RemoteSendCommand(COMANDO_ADC,(void *)&parametro,sizeof(parametro));
            i=0;
        }else{
            i++;
        }

    }
}
static portTASK_FUNCTION(DMATask,pvParameters)
{
    PARAM_COMANDO_OSCL par;
    PARAM_COMANDO_ALD param;
    uint32_t ui32Flags;

    //Miramos y borramos el flag
    ui32Flags=uDMAIntStatus();
    uDMAIntClear(ui32Flags);
//    int cnt=0;

    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    while(1)
    {
        int i=0;
        int j=0;
        int n=0;
//        uDMAChannelEnable(17);
//        uDMAChannelEnable(27);
//        uDMAChannelEnable(2);
        //Espero a que se activen AMBOS flags
        ui32Flags=0;
        ui32Flags=xEventGroupWaitBits(FlagsEventos,DMA0_FLAG|DMA1_FLAG|DMA2_FLAG,pdTRUE,pdFALSE,portMAX_DELAY);
        if(ui32Flags==(DMA0_FLAG|DMA1_FLAG)){//(1<<17)
            if(!ROM_uDMAChannelIsEnabled(UDMA_CHANNEL_ADC3))
            {
                //ACTIVAMOS UNA TAREA CON EL FLAG DE EVENTOS
                ROM_uDMAChannelTransferSet(UDMA_CHANNEL_ADC3 | UDMA_PRI_SELECT,
                        UDMA_MODE_BASIC,
                        (void *)(ADC0_BASE + ADC_O_SSFIFO3),
                        canal_17,sizeof(canal_17)/sizeof(uint16_t));
            }

            if(!ROM_uDMAChannelIsEnabled(UDMA_SEC_CHANNEL_ADC13))
            {
                //ACTIVAMOS UNA TAREA CON EL FLAG DE EVENTOS
                ROM_uDMAChannelTransferSet(UDMA_SEC_CHANNEL_ADC13 | UDMA_PRI_SELECT,
                        UDMA_MODE_BASIC,
                        (void *)(ADC1_BASE + ADC_O_SSFIFO3),
                        canal_27,sizeof(canal_27)/sizeof(uint16_t));
            }

//            xEventGroupSetBits(FlagsEventos, DMA0_FLAG);
//            xEventGroupSetBits(FlagsEventos, DMA1_FLAG);
            while(j<512){
                par.chan1[i]=canal_17[j];
                par.chan2[i]=canal_27[j];
                if(i==7){ //Hemos tomado las 32 lecturas,enviamos
                    RemoteSendCommand(COMANDO_OSCL,(void *)&par,sizeof(par));
                    i=0;
                }else{
                    i++;
                }//Aprovechamos que las estructuras MuestrasADC y PARAM_COMANDO_ADC son iguales para mandarlas directamente;
                j++;
            }

            ROM_uDMAChannelEnable(17);
            ROM_uDMAChannelEnable(27);

            ADCComparatorReset(ADC0_BASE,2,true,true); //Se olvida de lo anterior
            ADCSequenceEnable(ADC0_BASE,2); //ACTIVAMOS SECUENCIADOR PARA COMPARAR
            ADCComparatorReset(ADC1_BASE,2,true,true);
            ADCSequenceEnable(ADC1_BASE,2);


        }else if (ui32Flags==DMA2_FLAG){
            if(!ROM_uDMAChannelIsEnabled(2))
            {
                //ACTIVAMOS UNA TAREA CON EL FLAG DE EVENTOS
                uDMAChannelTransferSet(2 | UDMA_PRI_SELECT,
                           UDMA_MODE_BASIC,
                           (void *)(GPIO_PORTB_BASE + GPIO_O_DATA + (0xFF<<2)),
                           ((void*)destino), sizeof(destino)/sizeof(uint8_t));

                ROM_uDMAChannelEnable(2);
            }
            xEventGroupClearBits(FlagsEventos, DMA2_FLAG);
            for(n=0;n<8;n++){
                param.muestras.chan1[n]=destino[n];
                param.muestras.chan2[n]=destino[n+128];
                param.muestras.chan3[n]=destino[n+256];
                param.muestras.chan4[n]=destino[n+384];
                param.muestras.chan5[n]=destino[n+512];
                param.muestras.chan6[n]=destino[n+640];
                param.muestras.chan7[n]=destino[n+768];
                param.muestras.chan8[n]=destino[n+896];
            }
            //enviamos las muestras a Qt
            vTaskDelay(50);

            TimerDisable(TIMER3_BASE,TIMER_A);
            TimerIntClear(TIMER3_BASE,TIMER_TIMA_TIMEOUT);
            TimerDMAEventSet(TIMER3_BASE,TIMER_DMA_TIMEOUT_A);//TIMER_DMA_TIMEOUT_A
            TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
            IntEnable(INT_TIMER3A);
            RemoteSendCommand(COMANDO_ALD,(void *)&param,sizeof(param));
            uDMAIntClear(ui32Flags);
        }
        orig++; //Esto es para tener un punto de ruptura
    }
}

//*****************************************************************************
//
// Funcion main(), Inicializa los perifericos, crea las tareas, etc... y arranca el bucle del sistema
//
//*****************************************************************************
int main(void)
{
    //
    // Set the clocking to run at 40 MHz from the PLL.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
            SYSCTL_OSC_MAIN);   //Ponermos el reloj principal a 40 MHz (200 Mhz del Pll dividido por 5)


    // Get the system clock speed.
    g_ulSystemClock = SysCtlClockGet();


    //Habilita el clock gating de los perifericos durante el bajo consumo --> perifericos que se desee activos en modo Sleep
    //                                                                        deben habilitarse con SysCtlPeripheralSleepEnable
    ROM_SysCtlPeripheralClockGating(true);

    // Inicializa el subsistema de medida del uso de CPU (mide el tiempo que la CPU no esta dormida)
    // Para eso utiliza un timer, que aqui hemos puesto que sea el TIMER3 (ultimo parametro que se pasa a la funcion)
    // (y por tanto este no se deberia utilizar para otra cosa).
    CPUUsageInit(g_ulSystemClock, configTICK_RATE_HZ/10, 5);

    //
    // Inicializa la UARTy la configura a 115.200 bps, 8-N-1 .
    //se usa para mandar y recibir mensajes y comandos por el puerto serie
    // Mediante un programa terminal como gtkterm, putty, cutecom, etc...
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION perifericos
//--------------------------------------------------------------------------------------------------------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION perifericos activos en reposo
//--------------------------------------------------------------------------------------------------------------------------------
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_ADC1);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);   //La UART tiene que seguir funcionando aunque el micro este dormido
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);   //La UART tiene que seguir funcionando aunque el micro este dormido
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UDMA);
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION activacion de puertos
//--------------------------------------------------------------------------------------------------------------------------------
    //activacion botones con 3 posibles estados
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    // Enable pin PE3 for ADC AIN0|AIN1|AIN2|AIN3
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);
    // activacion puerto B con 8 pines
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION UART
//--------------------------------------------------------------------------------------------------------------------------------
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0,115200,SysCtlClockGet());
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION velocidad conversión
//--------------------------------------------------------------------------------------------------------------------------------
    //Configuramos la velocidad de conversion al maximo (1MS/s)
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);
    ADCClockConfigSet(ADC1_BASE, ADC_CLOCK_RATE_FULL, 1);
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION TIMER GENERAL
//--------------------------------------------------------------------------------------------------------------------------------
    uint32_t ui32Period=0;
    ui32Period2=0;
    // Periodo de cuenta de 0.05s. SysCtlClockGet() te proporciona la frecuencia del reloj del sistema, por lo que una cuenta
    // del Timer a SysCtlClockGet() tardara 1 segundo, a 0.5*SysCtlClockGet(), 0.5seg, etc...
    ui32Period = (SysCtlClockGet() / 100); //Empieza a 0.0001 seg
    ui32Period2 = (SysCtlClockGet() / 100); //Empieza a 0.0001 seg
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION TIMER 2
//--------------------------------------------------------------------------------------------------------------------------------
    // Configura el Timer0 para cuenta periodica de 32 bits (no lo separa en TIMER0A y TIMER0B)
    TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
    // Carga la cuenta en el Timer3A
    TimerLoadSet(TIMER3_BASE, TIMER_A, ui32Period -1);
    // Y habilita, dentro del modulo TIMER0, la interrupcion de particular de "fin de cuenta"
    // Habilita permiso general de interrupciones el sistema.
    //habilita el disparo por timer;
    TimerControlStall(TIMER3_BASE,TIMER_A,true);
    TimerControlTrigger(TIMER3_BASE,TIMER_A,1);
    // Habilita interrupcion del modulo TIMER
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION TIMER 1
//--------------------------------------------------------------------------------------------------------------------------------
    // Configura el Timer0 para cuenta periodica de 32 bits (no lo separa en TIMER0A y TIMER0B)
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    // Carga la cuenta en el Timer2A
    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period2 -1);
    // Habilita interrupcion del modulo TIMER
    // IntEnable(INT_TIMER2A);
    // Y habilita, dentro del modulo TIMER0, la interrupcion de particular de "fin de cuenta"
    //TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    // Habilita permiso general de interrupciones el sistema.
    TimerControlStall(TIMER2_BASE,TIMER_A,true);
    TimerControlTrigger(TIMER2_BASE,TIMER_A,1);
    //habilita el disparo por timer;
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION RGB
//--------------------------------------------------------------------------------------------------------------------------------
    //Inicializa los LEDs usando libreria RGB --> usa Timers 0 y 1 (eliminar si no se usa finalmente)
    RGBInit(1);
    SysCtlPeripheralSleepEnable(GREEN_TIMER_PERIPH);
    SysCtlPeripheralSleepEnable(BLUE_TIMER_PERIPH);
    SysCtlPeripheralSleepEnable(RED_TIMER_PERIPH);  //Redundante porque BLUE_TIMER_PERIPH y GREEN_TIMER_PERIPH son el mismo
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION BOTONES
//--------------------------------------------------------------------------------------------------------------------------------
    configBOTON_iniciaBOTON();  //SEMANA 1: Inicia el boton
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION ADC
//--------------------------------------------------------------------------------------------------------------------------------
    configADC_IniciaADC();  //SEMANA 2: Inicia el ADC
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION COMPARADOR
//--------------------------------------------------------------------------------------------------------------------------------
    configADC_IniciaCOMPARADOR();  //SEMANA 3: Inicia el COMPARADOR
//--------------------------------------------------------------------------------------------------------------------------------
//                          CONFIGURACION DMA
//--------------------------------------------------------------------------------------------------------------------------------
    configADC_IniciaDMA();//SEMANA 3: Inicia el DMA
//--------------------------------------------------------------------------------------------------------------------------------
//                         BIENVENIDA, INICIALIZACIÓN Y TAREAS
//--------------------------------------------------------------------------------------------------------------------------------
	//
	// Mensaje de bienvenida inicial.
	//
	UARTprintf("\n\nBienvenido a la aplicacion del sistema de instrumentacion de bajo coste (curso 2015/16)!\n");
	UARTprintf("\nAutor: Jose Carlos Sanchez Morales ");

	/**                                              Creacion de tareas 												**/

	// Crea la tarea que gestiona los comandos UART (definida en el fichero commands.c)
	//
	if((xTaskCreate(LEDTask, (portCHAR *)"Led", 512,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
		while(1);
	}
	if((xTaskCreate(BOTONTask, (portCHAR *)"Boton", 256,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
	    while(1);
	}
	//SEMANA 2: Lanza una tarea para leer y enviar los datos del ADC
	if((xTaskCreate(ADCTask, (portCHAR *)"ADC", 256,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
	    while(1);
	}
	// Crea la tarea que gestiona los comandos UART (definida en el fichero commands.c)
	if((xTaskCreate(vUARTTask, (portCHAR *)"Uart", 512,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
	    while(1);
	}
	mutex=xSemaphoreCreateMutex();
	if(mutex==NULL){
	    while(1);//no hay memoria para semaforo.
	}
	//  SEMANA 3: Lanza una tarea para DMA
	if((xTaskCreate(DMATask, (portCHAR *)"DMA", 256,NULL,tskIDLE_PRIORITY + 1, NULL) != pdTRUE))
	{
	    while(1);
	}

	UsbSerialInit(32,32);	//Inicializo el  sistema USB
	RemoteInit(); //Inicializo la aplicacion de comunicacion con el PC (Remote)


	//
	// Arranca el  scheduler.  Pasamos a ejecutar las tareas que se hayan activado.
	//
	vTaskStartScheduler();	//el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas

	//De la funcion vTaskStartScheduler no se sale nunca... a partir de aqui pasan a ejecutarse las tareas.
	while(1)
	{
		//Si llego aqui es que algo raro ha pasado
	}
}

