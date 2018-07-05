/*
 * usb_dev_serial.h
 *
 *  Created on: 27/01/2013
 *      Author: Jose Manuel Cano, Ignacio Herrero, Eva Gonzalez
 *
 *      Este modulo realiza la integracion de la biblioteca USB del TIVAWAre co FreeRTOS
 *
 *      Su API es parecida a la que gestiona el puerto serie desde el FreeRTOS
 *
 */

#ifndef USB_DEV_SERIAL_H_
#define USB_DEV_SERIAL_H_
#include<stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//Api parecida a la que gestiona el puerto serie desde el freertos.


int32_t UsbSerialInit (  portBASE_TYPE txQueueLength,  portBASE_TYPE rxQueueLength);
 portBASE_TYPE xUSBSerialGetChar (  portCHAR *pcRxedChar, portTickType xBlockTime );
 portBASE_TYPE xUSBSerialPutChar ( portCHAR cOutChar, portTickType xBlockTime );
 int32_t xUSBSerialWrite( uint8_t *buffer, int32_t size, portTickType xBlockTime );
 int32_t xUSBSerialRead( uint8_t *buffer, int32_t size, portTickType xBlockTime );

#endif /* USB_DEV_SERIAL_H_ */
