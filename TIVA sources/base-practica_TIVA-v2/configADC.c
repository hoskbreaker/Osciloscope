#include<stdint.h>
#include<stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "configADC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "drivers/buttons.h"

#include "inc/hw_udma.h"
#include "driverlib/udma.h"
#include "event_groups.h"

#include "inc/hw_gpio.h"


 //SEMANA2: Este fichero implementa la configuracion del ADC y la ISR asociada. Las tareas pueden llamar a la funcion configADC_LeeADC (bloqueante) para leer datos del ADC
//La funcion configADC_DisparaADC(...) (no bloqueante) realiza el disparo software del ADC
//La funcion configADC_IniciaADC realiza la configuración del ADC: Los Pines E0 a E3 se ponen como entradas analógicas (AIN3 a AIN0 respectivamente). Ademas crea la cola de mensajes necesaria para que la funcion configADC_LeeADC sea bloqueante


static QueueHandle_t cola_adc,cola_boton;
EventGroupHandle_t FlagsEventos;
uint8_t canal_17[512];
uint8_t canal_27[512];
//Array destino del DMA (canal 2)
uint8_t destino[1024]; /* Zona de memoria donde se almacenan los resultados de convertir con el ADC 0*/
uint8_t counter=0;
//*****************************************************************************
//
// The control table used by the uDMA controller.  This table must be aligned
// to a 1024 byte boundary.
// Array con tabla de control para el DMA
// Por lo visto, se le puede poner 512 de tamanho si no se usa "ping-pong"
//
//*****************************************************************************
#if defined(ewarm)
#pragma data_alignment=1024
uint8_t ui8ControlTable[1024];
#elif defined(ccs)
#pragma DATA_ALIGN(ui8ControlTable, 1024)
uint8_t ui8ControlTable[1024];
#else
uint8_t ui8ControlTable[1024] __attribute__ ((aligned(1024)));
#endif



//Provoca el disparo de una conversion (hemos configurado el ADC con "disparo software" (Processor trigger)
void configADC_DisparaADC(void)
{
    TimerEnable(TIMER2_BASE, TIMER_A);
    //TimerDisable(TIMER3_BASE, TIMER_A);
}
void configADC_DisparaALD(void)
{
    TimerEnable(TIMER3_BASE, TIMER_A);
   // TimerDisable(TIMER2_BASE, TIMER_A);
}
void configADC_IniciaADC(void)
{
    //CONFIGURAR SECUENCIADOR 1
    ADCSequenceDisable(ADC0_BASE,1);

    ADCSequenceConfigure(ADC0_BASE,1,ADC_TRIGGER_TIMER,0);
    ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE,1,1,ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE,1,2,ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH3|ADC_CTL_END|ADC_CTL_IE );   //La ultima muestra provoca la interrupcion
    //ADCSequenceEnable(ADC0_BASE,1); //ACTIVO LA SECUENCIA

    //Habilita las interrupciones
    ADCIntClear(ADC0_BASE,1);
    ADCIntEnable(ADC0_BASE,1);
    IntPrioritySet(INT_ADC0SS1,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    IntEnable(INT_ADC0SS1);

    //Creamos una cola de mensajes para la comunicacion entre la ISR y la tara que llame a configADC_LeeADC(...)
    cola_adc=xQueueCreate(8,sizeof(MuestrasADC));
    if (cola_adc==NULL)
    {
        while(1);
    }
}
void configADC_IniciaCOMPARADOR_1(void)
{
    //CONFIGURAR SECUENCIADOR 2 ADC0
    ADCSequenceDisable(ADC0_BASE,2);
    ADCSequenceConfigure(ADC0_BASE,2,ADC_TRIGGER_ALWAYS,0);
    ADCSequenceStepConfigure(ADC0_BASE,2,0,ADC_CTL_CH0|ADC_CTL_CMP2|ADC_CTL_END );   //La ultima muestra provoca la interrupcion

    //CONFIGURACIÓN COMPARADOR ADC0
    ADCComparatorConfigure(ADC0_BASE,2,ADC_COMP_INT_HIGH_HONCE);//ADC_COMP_INT_HIGH_HONCE
    ADCComparatorRegionSet(ADC0_BASE,2,1500,1500);//ADCComparatorRegionSet(ADC0_BASE,2,1500,1500);
    ADCComparatorReset(ADC0_BASE,2,true,true);
    ADCSequenceEnable(ADC0_BASE,2); //ACTIVO LA SECUENCIA

    //Habilita las interrupciones secuenciador 2 ADC0
    ADCComparatorIntClear(ADC0_BASE,2);
    ADCComparatorIntEnable(ADC0_BASE,2);
    IntPrioritySet(INT_ADC0SS2,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    IntEnable(INT_ADC0SS2);
}
void configADC_IniciaCOMPARADOR_2(void)
{
    //CONFIGURAR SECUENCIADOR 2 ADC1
    ADCSequenceDisable(ADC1_BASE,2);
    ADCSequenceConfigure(ADC1_BASE,2,ADC_TRIGGER_ALWAYS,0);
    ADCSequenceStepConfigure(ADC1_BASE,2,0,ADC_CTL_CH0|ADC_CTL_CMP2|ADC_CTL_END );   //La ultima muestra provoca la interrupcion

    //CONFIGURACIÓN COMPARADOR ADC1
    ADCComparatorConfigure(ADC1_BASE,2,ADC_COMP_INT_HIGH_HONCE);//ADC_COMP_INT_HIGH_HONCE
    ADCComparatorRegionSet(ADC1_BASE,2,1500,1500);//ADCComparatorRegionSet(ADC0_BASE,2,1500,1500);
    ADCComparatorReset(ADC1_BASE,2,true,true);
    ADCSequenceEnable(ADC1_BASE,2); //ACTIVO LA SECUENCIA

    //Habilita las interrupciones secuenciador 2 ADC1
    ADCComparatorIntClear(ADC1_BASE,2);
    ADCComparatorIntEnable(ADC1_BASE,2);
    IntPrioritySet(INT_ADC1SS2,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    IntEnable(INT_ADC1SS2);
}
void configADC_IniciaCOMPARADOR(void)
{
    configADC_IniciaCOMPARADOR_1();
    configADC_IniciaCOMPARADOR_2();
}
void DMA_CANAL1(void)
{
    //CONFIGURAR SECUENCIADOR 3
    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_TIMER,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END );   //La ultima muestra provoca la interrupcion
    ADCSequenceDisable(ADC0_BASE,3); //DESACTIVO LA SECUENCIA

    //CONFIGURACION CANAL 17
    uDMAChannelAssign(UDMA_CH17_ADC0_3);

    ROM_uDMAChannelAttributeDisable(UDMA_CHANNEL_ADC3,
            UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
            UDMA_ATTR_HIGH_PRIORITY |
            UDMA_ATTR_REQMASK);

    ROM_uDMAChannelControlSet(UDMA_CHANNEL_ADC3| UDMA_PRI_SELECT,
            UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 |
            UDMA_ARB_1);

    ROM_uDMAChannelTransferSet(UDMA_CHANNEL_ADC3| UDMA_PRI_SELECT,
            UDMA_MODE_BASIC,
            (void *)(ADC0_BASE + ADC_O_SSFIFO3),
            canal_17, sizeof(canal_17)/sizeof(uint16_t));

    ROM_uDMAChannelAttributeEnable(UDMA_CHANNEL_ADC3, UDMA_ATTR_USEBURST);
    ROM_uDMAChannelEnable(UDMA_CHANNEL_ADC3);

    //habilita las interrupciones secuenciador 3
    ADCSequenceDMAEnable(ADC0_BASE,3);
    ADCIntClear(ADC0_BASE,3);
    IntPrioritySet(INT_ADC0SS3,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    IntEnable(INT_ADC0SS3);
}
void DMA_CANAL2(void)
{
    //CONFIGURAR SECUENCIADOR 3
    ADCSequenceConfigure(ADC1_BASE,3,ADC_TRIGGER_TIMER,0);
    ADCSequenceStepConfigure(ADC1_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END );   //La ultima muestra provoca la interrupcion
    ADCSequenceDisable(ADC1_BASE,3); //DESACTIVO LA SECUENCIA

    //CONFIGURAR CANAL 27
    uDMAChannelAssign(UDMA_CH27_ADC1_3);

    ROM_uDMAChannelAttributeDisable(UDMA_SEC_CHANNEL_ADC13,
            UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
            UDMA_ATTR_HIGH_PRIORITY |
            UDMA_ATTR_REQMASK);

    ROM_uDMAChannelControlSet(UDMA_SEC_CHANNEL_ADC13| UDMA_PRI_SELECT,
            UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 |
            UDMA_ARB_1);

    ROM_uDMAChannelTransferSet(UDMA_SEC_CHANNEL_ADC13| UDMA_PRI_SELECT,
            UDMA_MODE_BASIC,
            (void *)(ADC1_BASE + ADC_O_SSFIFO3),
            canal_27, sizeof(canal_27)/sizeof(uint16_t));

    ROM_uDMAChannelAttributeEnable(UDMA_SEC_CHANNEL_ADC13, UDMA_ATTR_USEBURST);
    ROM_uDMAChannelEnable(UDMA_SEC_CHANNEL_ADC13);

    //habilita las interrupciones secuenciador 3
    ADCSequenceDMAEnable(ADC1_BASE,3);
    ADCIntClear(ADC1_BASE,3);
    IntPrioritySet(INT_ADC1SS3,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    IntEnable(INT_ADC1SS3);
}

void DMA_CANAL3(void)
{
    //CONFIGURAR CANAL 2
    uDMAChannelAssign(UDMA_CH2_TIMER3A);

    uDMAChannelAttributeDisable(2,
            UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
            UDMA_ATTR_HIGH_PRIORITY |
            UDMA_ATTR_REQMASK);

    uDMAChannelControlSet(2 | UDMA_PRI_SELECT,
            UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 |
            UDMA_ARB_1);

    uDMAChannelTransferSet(2 | UDMA_PRI_SELECT,
            UDMA_MODE_BASIC,
            (void *)(GPIO_PORTB_BASE + GPIO_O_DATA + (0xFF<<2)),
            ((void*)destino), sizeof(destino)/sizeof(uint8_t));

    ROM_uDMAChannelAttributeEnable(2, UDMA_ATTR_USEBURST);
    ROM_uDMAChannelEnable(2);

    TimerIntClear(TIMER3_BASE,TIMER_TIMA_TIMEOUT);
    TimerDMAEventSet(TIMER3_BASE,TIMER_DMA_TIMEOUT_A);//TIMER_DMA_TIMEOUT_A
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER3A);
}
void configADC_IniciaDMA(void)
{
    ROM_IntEnable(INT_UDMAERR);
    ROM_uDMAEnable();
    ROM_uDMAControlBaseSet(ui8ControlTable);

    DMA_CANAL1();
    DMA_CANAL2();
    DMA_CANAL3();

    //Crea el grupo de eventos
    FlagsEventos = xEventGroupCreate();
    if( FlagsEventos == NULL )
    {
        while(1);
    }
}
void configADC_LeeADC(MuestrasADC *datos)
{
    xQueueReceive(cola_adc,datos,portMAX_DELAY);
}

void configBOTON_LeeBOTON(Pulsador *datos)
{
    xQueueReceive(cola_boton,datos,portMAX_DELAY);
}

void configADC_ISR(void)
{
    portBASE_TYPE higherPriorityTaskWoken=pdFALSE;

    MuestrasLeidasADC leidas;
    MuestrasADC finales;
    ADCIntClear(ADC0_BASE,1);//LIMPIAMOS EL FLAG DE INTERRUPCIONES
    //ADCIntDisable(ADC0_BASE,1);
    ADCSequenceDataGet(ADC0_BASE,1,(uint32_t *)&leidas);//COGEMOS LOS DATOS GUARDADOS

    //Pasamos de 32 bits a 16 (el conversor es de 12 bits, así que sólo son significativos los bits del 0 al 11)
    finales.chan1=leidas.chan1;
    finales.chan2=leidas.chan2;
    finales.chan3=leidas.chan3;
    finales.chan4=leidas.chan4;

    //Guardamos en la cola
    xQueueSendFromISR(cola_adc,&finales,&higherPriorityTaskWoken);
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
void configCOMPARADOR_ISR_1(void)
{
    ADCComparatorIntClear(ADC0_BASE,2);//LIMPIAMOS EL FLAG DE INTERRUPCIONES
    ADCComparatorIntDisable(ADC0_BASE,2);//DESACTIVAMOS COMPARADOR
    ADCSequenceDisable(ADC0_BASE,2);
    ADCIntDisable(ADC0_BASE,2);
    ADCSequenceEnable(ADC0_BASE,3);//activo el secuenciador del DMA
}

void configCOMPARADOR_ISR_2(void)
{
    ADCComparatorIntClear(ADC1_BASE,2);//LIMPIAMOS EL FLAG DE INTERRUPCIONES
    ADCComparatorIntDisable(ADC1_BASE,2);//DESACTIVAMOS COMPARADOR
    ADCSequenceDisable(ADC1_BASE,2);
    ADCIntDisable(ADC1_BASE,2);
    ADCSequenceEnable(ADC1_BASE,3);//activo el secuenciador del DMA
}
void configCOMPARADOR_ISR(void)
{
    TimerEnable(TIMER2_BASE, TIMER_A);
    configCOMPARADOR_ISR_1();
    configCOMPARADOR_ISR_2();
}

void configDMA_ISR(void)
{
    portBASE_TYPE higherPriorityTaskWoken=pdFALSE;
    ADCIntClear(ADC0_BASE,3);//LIMPIAMOS EL FLAG DE INTERRUPCIONES
    ADCIntClear(ADC1_BASE,3);//LIMPIAMOS EL FLAG DE INTERRUPCIONES
    ADCSequenceDisable(ADC0_BASE,3);
    ADCSequenceDisable(ADC1_BASE,3);
    if (counter==1){
        counter=0;
        ADCIntDisable(ADC0_BASE,3);
        ADCIntDisable(ADC1_BASE,3);
        xEventGroupSetBitsFromISR(FlagsEventos, DMA0_FLAG|DMA1_FLAG, &higherPriorityTaskWoken );
        portEND_SWITCHING_ISR(higherPriorityTaskWoken);
    }else{
        counter=1;
    }


}
void configALD_ISR(void)
{
    portBASE_TYPE higherPriorityTaskWoken=pdFALSE;
    TimerIntClear(TIMER3_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntDisable(TIMER3_BASE,TIMER_A);
    xEventGroupSetBitsFromISR(FlagsEventos, DMA2_FLAG , &higherPriorityTaskWoken );
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
void configBOTON_iniciaBOTON(void)
{
    ButtonsInit();

    GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_BOTH_EDGES); //flanco de subida+flanco de bajada
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);

    IntEnable(INT_GPIOF);

    cola_boton=xQueueCreate(4,sizeof(Pulsador));
    if (cola_boton==NULL)
    {
        while(1);
    }
}
void configBOTON_ISR(void)
{
    portBASE_TYPE higherPriorityTaskWoken=pdFALSE;

    uint32_t Flags;
    uint32_t Pins;
    Pulsador boton;
    Flags=GPIOIntStatus(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
    Pins=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);

    if (((Flags==1)||(Flags==16))&&(Pins==0))
    {
        boton.Der=boton.Izq=1;
    }
    else if (Flags&GPIO_PIN_4)
    {
        if (Pins==1)
        {
            boton.Der=0;
            boton.Izq=1;
        }
        else if(Pins==17)
        {
            boton.Der=boton.Izq=0;
        }
        else
        {
            boton.Der=1;
            boton.Izq=0;
        }
    }
    else if (Flags&GPIO_PIN_0)
    {
        if (Pins==16)
        {
            boton.Der=1;
            boton.Izq=0;
        }
        else if(Pins==17)
        {
            boton.Der=boton.Izq=0;
        }
        else
        {
            boton.Der=0;
            boton.Izq=1;
        }
    }

    GPIOIntClear(GPIO_PORTF_BASE,Flags);
    //Guardamos en la cola
    xQueueSendFromISR(cola_boton,&boton,&higherPriorityTaskWoken);
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
void configADC_frecuenciaADC(double valor)
{
    uint32_t ui32Period=SysCtlClockGet()/valor;
    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period -1);
}
void configADC_muestreoALD(double valor)
{
    uint32_t ui32Period=SysCtlClockGet()/valor;
    TimerLoadSet(TIMER3_BASE, TIMER_A, ui32Period -1);
}
//Interrupcion de error de DMA, hay que ponerla por si acaso...
void DMA_DMAError(void)
{
    while(1)
    {
    }
}
