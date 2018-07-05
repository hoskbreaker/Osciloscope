//*****************************************************************************
//
// commands.c - FreeRTOS porting example on CCS4
//
// Este fichero contiene errores que seran explicados en clase
//
//*****************************************************************************


#include <stdbool.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Standard TIVA includes */
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

/* Other TIVA includes */
#include "utils/cpu_usage.h"
#include "utils/cmdline.h"
#include "utils/ustdlib.h"

#include "utils/uartstdio.h"

#include "drivers/rgb.h"
//librerias protocol y remote
#include "protocol.h"
#include "remote.h"

static uint8_t frame[MAX_FRAME_SIZE];
// ==============================================================================
// The CPU usage in percent, in 16.16 fixed point format.
// ==============================================================================
extern uint32_t g_ui32CPUUsage;

// ==============================================================================
// Implementa el comando cpu que muestra el uso de CPU
// ==============================================================================
int  Cmd_cpu(int argc, char *argv[])
{
    //
    // Print some header text.
    //
    UARTprintf("ARM Cortex-M4F %u MHz - ",SysCtlClockGet() / 1000000);
    UARTprintf("%2u%% de uso\n", (g_ui32CPUUsage+32768) >> 16);

    // Return success.
    return(0);
}

// ==============================================================================
// Implementa el comando free, que muestra cuanta memoria "heap" le queda al FreeRTOS
// ==============================================================================
int Cmd_free(int argc, char *argv[])
{
    //
    // Print some header text.
    //
    UARTprintf("%d bytes libres\n", xPortGetFreeHeapSize());

    // Return success.
    return(0);
}

// ==============================================================================
// Implementa el comando task. Sï¿½lo es posible si la opciï¿½n configUSE_TRACE_FACILITY de FreeRTOS estï¿½ habilitada
// ==============================================================================
#if ( configUSE_TRACE_FACILITY == 1 )

extern char *__stack;
int Cmd_tasks(int argc, char *argv[])
{
    char*   pcBuffer;
    uint8_t*    pi8Stack;
    portBASE_TYPE   x;

    pcBuffer = pvPortMalloc(1024);
    vTaskList(pcBuffer);
    UARTprintf("\t\t\t\tUnused\nTaskName\tStatus\tPri\tStack\tTask ID\n");
    UARTprintf("=======================================================\n");
    UARTprintf("%s", pcBuffer);

    // Calculate kernel stack usage
    x = 0;
    pi8Stack = (uint8_t *) &__stack;
    while (*pi8Stack++ == 0xA5)
    {
        x++;    //Esto sólo funciona si hemos rellenado la pila del sistema con 0xA5 en el arranque
    }
    usprintf((char *) pcBuffer, "%%%us", configMAX_TASK_NAME_LEN);
    usprintf((char *) &pcBuffer[10], (const char *) pcBuffer, "kernel");
    UARTprintf("%s\t-\t*%u\t%u\t-\n", &pcBuffer[10], configKERNEL_INTERRUPT_PRIORITY, x/sizeof(portBASE_TYPE));
    vPortFree(pcBuffer);
    return 0;
}

#endif /* configUSE_TRACE_FACILITY */

#if configGENERATE_RUN_TIME_STATS
// ==============================================================================
// Implementa el comando "Stats"
// ==============================================================================
Cmd_stats(int argc, char *argv[])
{
    char*   pBuffer;

    pBuffer = pvPortMalloc(1024);
    if (pBuffer)
    {
        vTaskGetRunTimeStats(pBuffer); //Es un poco inseguro, pero por ahora nos vale...
        UARTprintf("TaskName\tCycles\t\tPercent\r\n");
        UARTprintf("===============================================\r\n");
        UARTprintf("%s", pBuffer);
        vPortFree(pBuffer);
    }
    return 0;
}
#endif

// ==============================================================================
// Implementa el comando help
// ==============================================================================
int Cmd_help(int argc, char *argv[])
{
    tCmdLineEntry *pEntry;

    //
    // Print some header text.
    //
    UARTprintf("Comandos disponibles\n");
    UARTprintf("------------------\n");

    //
    // Point at the beginning of the command table.
    //
    pEntry = &g_psCmdTable[0];

    //
    // Enter a loop to read each entry from the command table.  The end of the
    // table has been reached when the command name is NULL.
    //
    while(pEntry->pcCmd)
    {
        //
        // Print the command name and the brief description.
        //
        UARTprintf("%s%s\n", pEntry->pcCmd, pEntry->pcHelp);

        //
        // Advance to the next entry in the table.
        //
        pEntry++;
    }

    //
    // Return success.
    //
    return(0);
}

// ==============================================================================
// Implementa el comando "LED"
// ==============================================================================
int Cmd_led(int argc, char *argv[])
{
    if (argc != 2)
    {
        //Si los parametros no son suficientes o son demasiados, muestro la ayuda
        UARTprintf(" led [on|off]\n");
    }
    else
    {
        //seconds = ustrtoul(argv[1], NULL, 10);

        if (0==strncmp( argv[1], "on",2))
        {
            UARTprintf("Enciendo el LED\n");
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,GPIO_PIN_3);
        }
        else if (0==strncmp( argv[1], "off",3))
        {
            UARTprintf("Apago el LED\n");
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
        }
        else
        {
            //Si el parametro no es correcto, muestro la ayuda
            UARTprintf(" led [on|off]\n");
        }

    }


    return 0;
}


// ==============================================================================
// Implementa el comando "MODE"
// ==============================================================================
int Cmd_mode(int argc, char *argv[])
{
    if (argc != 2)
    {
        //Si los parametros no son suficientes o son demasiados, muestro la ayuda
        UARTprintf(" mode [gpio|rgb]\n");
    }
    else
    {
        int32_t numdatos;
        PARAM_COMANDO_CHECK parametro;
        //seconds = ustrtoul(argv[1], NULL, 10);

        if (0==strncmp( argv[1], "gpio",4))
        {
            UARTprintf("cambio a modo GPIO\n");
            RGBDisable();
            parametro.Check=1;
            ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
        }
        else if (0==strncmp( argv[1], "rgb",3))
        {
            UARTprintf("Cambio a modo RGB (pwm)\n");
            RGBEnable();
            parametro.Check=0;
        }
        else
        {
            //Si el parametro no es correcto, muestro la ayuda
            UARTprintf(" mode [gpio|rgb]\n");
        }
        numdatos=create_frame(frame,COMANDO_CHECK,&parametro,sizeof(parametro),MAX_FRAME_SIZE);

        if (numdatos>=0)
        {
            send_frame(frame,numdatos);
        }
    }
    return 0;
}

// ==============================================================================
// Implementa el comando "RGB"
// ==============================================================================
int Cmd_rgb(int argc, char *argv[])
{
    uint32_t arrayRGB[3];
    int32_t numdatos;
    PARAM_COMANDO_ROSCA parametro;

    if (argc != 4)
    {
        //Si los parï¿½metros no son suficientes, muestro la ayuda
        UARTprintf(" rgb [red] [green] [blue]\n");
    }
    else
    {

        arrayRGB[0]=ustrtoul(argv[1], NULL, 10)<<8;
        arrayRGB[1]=ustrtoul(argv[2], NULL, 10)<<8;
        arrayRGB[2]=ustrtoul(argv[3], NULL, 10)<<8;

        parametro.rosca.Rojo=(uint8_t)arrayRGB[0];
        parametro.rosca.Azul=(uint8_t)arrayRGB[1];
        parametro.rosca.Verde=(uint8_t)arrayRGB[2];

        if ((arrayRGB[0]>=65535)||(arrayRGB[1]>=65535)||(arrayRGB[2]>=65535))


        {

            UARTprintf(" Valor fuera de rango \n");
        }
        else{
            RGBColorSet(arrayRGB);
        }

    }

    numdatos=create_frame(frame,COMANDO_ROSCA,&parametro,sizeof(parametro),MAX_FRAME_SIZE);

            if (numdatos>=0)
            {
                send_frame(frame,numdatos);
            }
    return 0;
}

int Cmd_intensity(int argc, char *argv[])
{

    int32_t numdatos;
    PARAM_COMANDO_BRILLO parametro;
    float ui32valor;
    if (argc != 2)
    {
        //Si los parï¿½metros no son suficientes, muestro la ayuda
        UARTprintf(" intensity [valor entre 0.0 y 1.0]\n");
    }
    else
    {
        ui32valor = ustrtof(argv[1],NULL);
        parametro.rIntensity=ui32valor;
        RGBIntensitySet(ui32valor);
        numdatos=create_frame(frame,COMANDO_BRILLO,&parametro,sizeof(parametro),MAX_FRAME_SIZE);

            if (numdatos>=0)
            {
                send_frame(frame,numdatos);
            }
    }
    return 0;
}
int Cmd_gpio(int argc, char *argv[])
{
    int32_t numdatos;
    PARAM_COMANDO_LEDS parametro;
    if (argc != 3)
    {
        //Si los parï¿½metros no son suficientes, muestro la ayuda
        UARTprintf(" gpio [on/off] [numero color led] (1->rojo, 2->azul, 3->verde)\n");
    }
    else
    {
        char orden;
        uint8_t puerto, valor;
        if (0==strncmp( argv[1], "on",2)){
            orden='0';
        }
        if (0==strncmp( argv[1], "off",3)){
            orden='1';
        }
       int color= ustrtoul(argv[2], NULL, 10);
         parametro.leds.blue=0;
         parametro.leds.green=0;
         parametro.leds.red=0;
       switch(orden){
       case '0':
           if(color==1){
               puerto=GPIO_PIN_1;
               valor=GPIO_PIN_1;
               parametro.leds.red=1;
           }
           if (color==2){
               puerto=GPIO_PIN_2;
               valor=GPIO_PIN_2;
               parametro.leds.blue=1;
           }
           if (color==3){
               puerto=GPIO_PIN_3;
               valor=GPIO_PIN_3;
               parametro.leds.green=1;
           }
           break;
       case '1':
          if(color==1){
              puerto=GPIO_PIN_1;
          }
          if (color==2){
              puerto=GPIO_PIN_2;
          }
          if (color==3){
              puerto=GPIO_PIN_3;
          }
          valor=0;
          parametro.leds.blue=0;
          parametro.leds.green=0;
          parametro.leds.red=0;
          break;
       }
       GPIOPinWrite(GPIO_PORTF_BASE, puerto ,valor);
       numdatos=create_frame(frame,COMANDO_LEDS,&parametro,sizeof(parametro),MAX_FRAME_SIZE);
           if (numdatos>=0)
           {
               send_frame(frame,numdatos);
           }
    }
    return 0;
}

// ==============================================================================
// Tabla con los comandos y su descripcion. Si quiero anadir alguno, debo hacerlo aqui
// ==============================================================================
tCmdLineEntry g_psCmdTable[] =
{
    { "help",     Cmd_help,      "     : Lista de comandos" },
    { "?",        Cmd_help,      "        : lo mismo que help" },
    { "cpu",      Cmd_cpu,       "      : Muestra el uso de  CPU " },
    { "led",      Cmd_led,       "      : Apaga y enciende el led verde" },
    { "mode",     Cmd_mode,       "      : Cambia los pines PF1, PF2 y PF3 entre modo GPIO y modo PWM (rgb)" },
    { "rgb",      Cmd_rgb,       "      : Establece el color RGB" },
    { "free",     Cmd_free,      "     : Muestra la memoria libre" },
    { "intensity", Cmd_intensity, "     :Cambia la intensidad del LED"},
    { "gpio",      Cmd_gpio,       "      : Apaga y enciende el led rojo, azul y verde" },
#if ( configUSE_TRACE_FACILITY == 1 )
    { "tasks",    Cmd_tasks,     "    : Muestra informacion de las tareas" },
#endif
#if (configGENERATE_RUN_TIME_STATS)
    { "stats",    Cmd_stats,      "     : Muestra estadisticas de las tareas" },
#endif
    { 0, 0, 0 }
};

// ==============================================================================
// Tarea UARTTask.  Espera la llegada de comandos por el puerto serie y los ejecuta al recibirlos...
// ==============================================================================
extern xSemaphoreHandle g_xRxLineSemaphore;
void UARTStdioIntHandler(void);

void vUARTTask( void *pvParameters )
{
    char    pcCmdBuf[64];
    int32_t i32Status;


    UARTprintf("\n\n FreeRTOS %s \n",
        tskKERNEL_VERSION_NUMBER);
    UARTprintf("\n Teclee ? para ver la ayuda \n");
    UARTprintf("> ");

    /* Loop forever */
    while (1)
    {
        /* Wait for a signal indicating we have an RX line to process */
        xSemaphoreTake(g_xRxLineSemaphore, portMAX_DELAY);

        if (UARTPeek('\r') != -1)
        {
            /* Read data from the UART and process the command line */
            UARTgets(pcCmdBuf, sizeof(pcCmdBuf));
            if (ustrlen(pcCmdBuf) == 0)
            {
                UARTprintf("> ");
                continue;
            }

            //
            // Pass the line from the user to the command processor.  It will be
            // parsed and valid commands executed.
            //
            i32Status = CmdLineProcess(pcCmdBuf);

            //
            // Handle the case of bad command.
            //
            if(i32Status == CMDLINE_BAD_CMD)
            {
                UARTprintf("Comando erroneo!\n");   //No pongo acentos adrede
            }

            //
            // Handle the case of too many arguments.
            //
            else if(i32Status == CMDLINE_TOO_MANY_ARGS)
            {
                UARTprintf("El interprete de comandos no admite tantos parametros\n");  //El maximo, CMDLINE_MAX_ARGS, esta definido en cmdline.c
            }

            //
            // Otherwise the command was executed.  Print the error code if one was
            // returned.
            //
            else if(i32Status != 0)
            {
                UARTprintf("El comando devolvio el error %d\n",i32Status);
            }

            UARTprintf("> ");
        }
    }
}
