/*
 * remote.h
 *
 *  Created on: 1/4/2016
 *      Author: jcgar
 */

#ifndef REMOTE_H_
#define REMOTE_H_


#include<stdbool.h>
#include<stdint.h>



#include "protocol.h"

//parametros de funcionamiento de la tarea
#define REMOTE_TASK_STACK (512)
#define REMOTE_TASK_PRIORITY (tskIDLE_PRIORITY+2)

void RemoteInit(void);
int32_t RemoteSendCommand(uint8_t comando,void *parameter,int32_t paramsize);

#endif /* REMOTE_H_ */
