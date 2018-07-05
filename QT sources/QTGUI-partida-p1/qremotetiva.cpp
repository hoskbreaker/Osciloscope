#include "qremotetiva.h"


#include <QSerialPort>      // Comunicacion por el puerto serie
#include <QSerialPortInfo>  // Comunicacion por el puerto serie
#include <QColor>
extern "C" {
#include "protocol.h"    // Cabecera de funciones de gestión de tramas; se indica que está en C, ya que QTs
// se integra en C++, y el C puede dar problemas si no se indica.
}
static int n=0;
QRemoteTIVA::QRemoteTIVA(QObject *parent) : QObject(parent)
{

    connected=false;

    // Las funciones CONNECT son la base del funcionamiento de QT; conectan dos componentes
    // o elementos del sistema; uno que GENERA UNA SEÑAL; y otro que EJECUTA UNA FUNCION (SLOT) al recibir dicha señal.
    // En el ejemplo se conecta la señal readyRead(), que envía el componente que controla el puerto USB serie (serial),
    // con la propia clase PanelGUI, para que ejecute su funcion readRequest() en respuesta.
    // De esa forma, en cuanto el puerto serie esté preparado para leer, se lanza una petición de datos por el
    // puerto serie.El envío de readyRead por parte de "serial" es automatico, sin necesidad de instrucciones
    // del programador
    connect(&serial, SIGNAL(readyRead()), this, SLOT(readRequest()));
}


//Este slot se conecta con la señal readyRead() del puerto serie, que se activa cuando hay algo que leer del puerto serie
//Se encarga de procesar y decodificar los datos que llegan de la TIVA y generar señales en respuesta a algunos de ellos
//Estas señales son capturadas por slots de la clase guipanel en este ejemplo.
void QRemoteTIVA::readRequest()
{
    int posicion,tam;   // Solo uso notacin hungara en los elementos que se van a
    // intercambiar con el micro - para control de tamaño -
    uint8_t *pui8Frame; // Puntero a zona de memoria donde reside la trama recibida
    void *ptrtoparam;
    uint8_t ui8Command; // Para almacenar el comando de la trama entrante

    request.append(serial.readAll()); // Añade el contenido del puerto serie USB al array de bytes 'request'
    // así vamos acumulando  en el array la información que va llegando

    // Busca la posición del primer byte de fin de trama (0xFD) en el array
    posicion=request.indexOf((char)STOP_FRAME_CHAR,0);
    //Metodo poco eficiente pero seguro...
    while (posicion>0)
    {
        pui8Frame=(uint8_t*)request.data(); // Puntero de trama al inicio del array de bytes
        tam=posicion-1;    //Caracter de inicio y fin no cuentan en el tamaño
        // Descarta posibles bytes erroneos que no se correspondan con el byte de inicio de trama
        while (((*pui8Frame)!=(uint8_t)START_FRAME_CHAR)&&(tam>0)) // Casting porque Qt va en C++ (en C no hace falta)
        {
            pui8Frame++;  // Descarta el caracter erroneo
            tam--;    // como parte de la trama recibida
        }
        // A. Disponemos de una trama encapsulada entre dos caracteres de inicio y fin, con un tamaño 'tam'
        if (tam > 0)
        {   //Paquete aparentemente correcto, se puede procesar
            pui8Frame++;  //Quito el byte de arranque (START_FRAME_CHAR, 0xFC)
            //Y proceso normalmente el paquete
            // Paso 1: Destuffing y cálculo del CRC. Si todo va bien, obtengo la trama
            // con valores actualizados y sin bytes de checksum
            tam=destuff_and_check_checksum((unsigned char *)pui8Frame,tam);
            // El tamaño se actualiza (he quitado 2bytes de CRC, mas los de "stuffing")
            if (tam>=0)
            {
                //El paquete está bien, luego procedo a tratarlo.
                ui8Command=decode_command_type(pui8Frame); // Obtencion del byte de Comando
                tam=get_command_param_pointer(pui8Frame,tam,&ptrtoparam);



                switch(ui8Command) // Segun el comando tengo que hacer cosas distintas
                {
                /** A PARTIR AQUI ES DONDE SE DEBEN AÑADIR NUEVAS RESPUESTAS ANTE LOS COMANDOS QUE SE ENVIEN DESDE LA TIVA **/
                case COMANDO_PING:  // Algunos comandos no tiene parametros
                    // Crea una ventana popup con el mensaje indicado
                    emit pingReceivedFromTiva();
                    break;

                case COMANDO_RECHAZADO:
                {
                    // En otros comandos hay que extraer los parametros de la trama y copiarlos
                    // a una estructura para poder procesar su informacion
                    PARAM_COMANDO_RECHAZADO parametro;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(parametro),&parametro)>0)
                    {
                       // Muestra en una etiqueta (statuslabel) del GUI el mensaje
                       emit commandRejectedFromTiva(parametro.command);
                    }
                    else
                    {
                       emit commandRejectedFromTiva(-1);
                    }
                }
                    break;

                    //Falta por implementar la recepcion de mas tipos de comando
                    //habria que decodificarlos y emitir las señales correspondientes con los parametros que correspondan
                case COMANDO_SONDEO:
                {
                    PARAM_COMANDO_SONDEO parametro;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(parametro),&parametro)>0){
                         emit RespuestaSondeo(parametro.Izq, parametro.Der);
                    }else{
                        emit commandRejectedFromTiva(-1);
                    }
                }
                    break;
                case COMANDO_PUL:
                {
                    PARAM_COMANDO_PUL parametro;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(parametro),&parametro)>0){
                         emit RespuestaSondeo(parametro.Izq, parametro.Der);
                    }else{
                        emit commandRejectedFromTiva(-1);
                    }
                }
                    break;
                case COMANDO_CHECK:
                {
                    PARAM_COMANDO_CHECK param;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(param),&param)>0){
                         emit Respuesta_check(param.check);
                    }else{
                        emit commandRejectedFromTiva(-1);
                    }
                }
                    break;
                case COMANDO_BRILLO:
                {
                    PARAM_COMANDO_BRILLO param;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(param),&param)>0){
                         emit Respuesta_brillo(param.rIntensity);
                    }else{
                        emit commandRejectedFromTiva(-1);
                    }
                }
                    break;
                    //Falta por implementar la recepcion de mas tipos de comando
                    //habria que decodificarlos y emitir las señales correspondientes con los parametros que correspondan
                case COMANDO_LEDS:
                {
                    PARAM_COMANDO_LEDS param;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(param),&param)>0){
                         emit Respuesta_leds(param.leds.fRed, param.leds.fBlue, param.leds.fGreen);
                    }else{
                        emit commandRejectedFromTiva(-1);
                    }
                }
                    break;
                case COMANDO_ROSCA:
                {
                    PARAM_COMANDO_ROSCA param;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(param),&param)>0){
                        QColor valor;
                        valor.setRed(param.color.fRojo);
                        valor.setBlue(param.color.fAzul);
                        valor.setGreen(param.color.fVerde);
                         emit Respuesta_rosca(valor);
                    }else{
                        emit commandRejectedFromTiva(-1);
                    }
                }
                    break;

                case COMANDO_ADC:
                {    //SEMANA2: Este caso trata la recepcion de datos del ADC desde la TIVA
                    PARAM_COMANDO_ADC parametro;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(parametro),&parametro)>0)
                    {
                        // Muestra en una etiqueta (statuslabel) del GUI el mensaje
                        for(int i=0;i<8;i++){
                            emit commandADCReceived(parametro.chan1[i],parametro.chan2[i],parametro.chan3[i],parametro.chan4[i],n);
                            n++;
                            if(n==1024){
                                n=0;
                            }
                        }

                    }
                    else
                    {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                        LastError=QString("Status: Recibidos datos incorrectos");
                        emit statusChanged(QRemoteTIVA::ReceivedDataError,LastError);

                    }

                }
                break;
                case COMANDO_OSCL:
                {    //SEMANA2: Este caso trata la recepcion de datos del ADC desde la TIVA
                    PARAM_COMANDO_OSCL parametro;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(parametro),&parametro)>0)
                    {
                        // Muestra en una etiqueta (statuslabel) del GUI el mensaje
                        for(int i=0;i<8;i++){
                            emit commandADCReceived(parametro.chan1[i],parametro.chan2[i],0,0,n);
                            n++;
                            if(n==1024){
                                n=0;
                            }
                        }

                    }
                    else
                    {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                        LastError=QString("Status: Recibidos datos incorrectos");
                        emit statusChanged(QRemoteTIVA::ReceivedDataError,LastError);

                    }

                }
                break;

                case COMANDO_ALD:
                {    //SEMANA2: Este caso trata la recepcion de datos del ADC desde la TIVA
                    PARAM_COMANDO_ALD parametro;
                    if (check_and_extract_command_param(ptrtoparam, tam, sizeof(parametro),&parametro)>0)
                    {
                        uint16_t tam=sizeof(parametro.muestras.chan1)/sizeof(uint8_t);
                        // Muestra en una etiqueta (statuslabel) del GUI el mensaje
                            emit commandALDReceived(parametro,tam);
                    }
                    else
                    {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                        LastError=QString("Status: Recibidos datos incorrectos");
                        emit statusChanged(QRemoteTIVA::ReceivedDataError,LastError);

                    }

                }
                break;

                default:
                    //Este error lo notifico mediante la señal statusChanged
                    LastError=QString("Status: Recibido paquete inesperado");
                    emit statusChanged(QRemoteTIVA::UnexpectedPacketError,LastError);
                    break;
                }
            }
        }
        else
        {
            // B. La trama no está completa... no lo procesa, y de momento no digo nada
            //Este error lo notifico mediante la señal statusChanged
            LastError=QString("Status:Fallo trozo paquete recibido");
            emit statusChanged(QRemoteTIVA::FragmentedPacketError,LastError);
        }
        request.remove(0,posicion+1); // Se elimina todo el trozo de información erroneo del array de bytes
        posicion=request.indexOf((char)STOP_FRAME_CHAR,0); // Y se busca el byte de fin de la siguiente trama
    }
}

// Este método realiza el establecimiento de la comunicación USB serie con la TIVA a través del interfaz seleccionado
// Se establece una comunicacion a 9600bps 8N1 y sin control de flujo en el objeto
// 'serial' que es el que gestiona la comunicación USB serie en el interfaz QT
// Si la conexion no es correcta, se generan señales de error.
void QRemoteTIVA::startSlave(QString puerto)
{
    if (serial.portName() != puerto) {
        serial.close();
        serial.setPortName(puerto);

        if (!serial.open(QIODevice::ReadWrite)) {
            LastError=QString("No puedo abrir el puerto %1, error code %2")
                         .arg(serial.portName()).arg(serial.error());

            emit statusChanged(QRemoteTIVA::OpenPortError,LastError);
            return ;
        }

        if (!serial.setBaudRate(9600)) {
            LastError=QString("No puedo establecer tasa de 9600bps en el puerto %1, error code %2")
                         .arg(serial.portName()).arg(serial.error());

            emit statusChanged(QRemoteTIVA::BaudRateError,LastError);
            return;
        }

        if (!serial.setDataBits(QSerialPort::Data8)) {
            LastError=QString("No puedo establecer 8bits de datos en el puerto %1, error code %2")
                         .arg(serial.portName()).arg(serial.error());


             emit statusChanged(QRemoteTIVA::DataBitError,LastError);
            return;
        }

        if (!serial.setParity(QSerialPort::NoParity)) {
            LastError=QString("NO puedo establecer parida en el puerto %1, error code %2")
                         .arg(serial.portName()).arg(serial.error());

            emit statusChanged(QRemoteTIVA::ParityError,LastError);
            return ;
        }

        if (!serial.setStopBits(QSerialPort::OneStop)) {
            LastError=QString("No puedo establecer 1bitStop en el puerto %1, error code %2")
                         .arg(serial.portName()).arg(serial.error());
            emit statusChanged(QRemoteTIVA::StopError,LastError);
            return;
        }

        if (!serial.setFlowControl(QSerialPort::NoFlowControl)) {
            LastError=QString("No puedo establecer el control de flujo en el puerto %1, error code %2")
                         .arg(serial.portName()).arg(serial.error());
             emit statusChanged(QRemoteTIVA::FlowControlError,LastError);
            return;
        }
    }

     emit statusChanged(QRemoteTIVA::TivaConnected,QString(""));

    // Variable indicadora de conexión a TRUE, para que se permita enviar comandos en respuesta
    // a eventos del interfaz gráfico
    connected=true;
}

//Método para leer el último mensaje de error
QString QRemoteTIVA::getLastErrorMessage()
{
    return LastError;
}

// **** Slots asociados al envio de comandos hacia la TIVA. La estructura va a ser muy parecida en casi todos los
// casos. Se va a crear una trama de un tamaño maximo (100), y se le van a introducir los elementos de
// num_secuencia, comando, y parametros.

//Este Slot realiza el envío de un mensaje de PING a la TIVA
void QRemoteTIVA::pingTiva()
{
    char paquete[MAX_FRAME_SIZE];
    int size;

    if (connected) // Para que no se intenten enviar datos si la conexion USB no esta activa
    {
        // El comando PING no necesita parametros; de ahí el NULL, y el 0 final.
        // No vamos a usar el mecanismo de numeracion de tramas; pasamos un 0 como n de trama
        size=create_frame((unsigned char *)paquete, COMANDO_PING, NULL, 0, MAX_FRAME_SIZE);
        // Si la trama se creó correctamente, se escribe el paquete por el puerto serie USB
        if (size>0) serial.write(paquete,size);
    }


}

//Este Slot realiza el envío del comando LED a la TIVA
void QRemoteTIVA::LEDsGpioTiva(bool red, bool green, bool blue)
{
    PARAM_COMANDO_LEDS parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        // Se rellenan los parametros del paquete (en este caso, el estado de los LED)
        parametro.leds.fRed=red;
        parametro.leds.fGreen=green;
        parametro.leds.fBlue=blue;
        // Se crea la trama con n de secuencia 0; comando COMANDO_LEDS; se le pasa la
        // estructura de parametros, indicando su tamaño; el nº final es el tamaño maximo
        // de trama
        size=create_frame((uint8_t *)pui8Frame, COMANDO_LEDS, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}


//Este Slot realiza el envio del comando de brillo a la TIVA
void QRemoteTIVA::LEDPwmBrightness(double value)
{
    PARAM_COMANDO_BRILLO parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        // Se rellenan los parametros del paquete (en este caso, el brillo)
        parametro.rIntensity=(float)value;
        // Se crea la trama con n de secuencia 0; comando COMANDO_LEDS; se le pasa la
        // estructura de parametros, indicando su tamaño; el nº final es el tamaño maximo
        // de trama
        size=create_frame((uint8_t *)pui8Frame, COMANDO_BRILLO, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

void QRemoteTIVA::SondeoTiva()
{
    char paquete[MAX_FRAME_SIZE];
    int size;
    if (connected) // Para que no se intenten enviar datos si la conexion USB no esta activa
    {
        // El comando PING no necesita parametros; de ahí el NULL, y el 0 final.
        // No vamos a usar el mecanismo de numeracion de tramas; pasamos un 0 como n de trama
        size=create_frame((unsigned char *)paquete, COMANDO_SONDEO, NULL, 0, MAX_FRAME_SIZE);
        // Si la trama se creó correctamente, se escribe el paquete por el puerto serie USB
        if (size>0) serial.write(paquete,size);
    }
}

//Este Slot realiza el envío del comando LED PWM a la TIVA
void QRemoteTIVA::LEDsPWMTiva(const QColor &arg1)
{
    PARAM_COMANDO_ROSCA parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        // Se rellenan los parametros del paquete (en este caso, el estado de los LED)
        parametro.color.fRojo=arg1.red();
        parametro.color.fVerde=arg1.green();
        parametro.color.fAzul=arg1.blue();
        // Se crea la trama con n de secuencia 0; comando COMANDO_LEDS; se le pasa la
        // estructura de parametros, indicando su tamaño; el nº final es el tamaño maximo
        // de trama
        size=create_frame((uint8_t *)pui8Frame, COMANDO_ROSCA, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

//Este Slot cambia de modo PWM a modo GPIO
void QRemoteTIVA::SwitchPWM_GPIO(bool value)
{
    PARAM_COMANDO_CHECK parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        // Se rellenan los parametros del paquete (en este caso, el brillo)
        parametro.check=value;
        // Se crea la trama con n de secuencia 0; comando COMANDO_LEDS; se le pasa la
        // estructura de parametros, indicando su tamaño; el nº final es el tamaño maximo
        // de trama
        size=create_frame((uint8_t *)pui8Frame, COMANDO_CHECK, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

//SEMANA2: Este Slot permite ordenar al objeto TIVA que envie un comando de conversion
void QRemoteTIVA::ADCSample(void)
{
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        size=create_frame((uint8_t *)pui8Frame, COMANDO_ADC, NULL, 0, MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

void QRemoteTIVA::CambioPeriodo(double fre)
{
    PARAM_COMANDO_PERIODO parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        // Se rellenan los parametros del paquete (en este caso, el brillo)
        parametro.frec=fre;
        size=create_frame((uint8_t *)pui8Frame, COMANDO_PERIODO, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}
//SEMANA2: Este Slot permite ordenar al objeto TIVA que envie un comando de conversion
void QRemoteTIVA::RealTimeControl(uint8_t val)
{
    PARAM_COMANDO_STOP param;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        param.tick=val;
        size=create_frame((uint8_t *)pui8Frame, COMANDO_STOP, &param, sizeof(param), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

void QRemoteTIVA::cambioModo(uint8_t val)
{
    PARAM_COMANDO_OSCILOSCOPIO param;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        param.tick=val;
        size=create_frame((uint8_t *)pui8Frame, COMANDO_OSCILOSCOPIO, &param, sizeof(param), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}
void QRemoteTIVA::CambioMuestreo(double val)
{
    PARAM_COMANDO_MUESTREO parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        // Se rellenan los parametros del paquete (en este caso, el brillo)
        parametro.fre=val;

        size=create_frame((uint8_t *)pui8Frame, COMANDO_MUESTREO, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

void QRemoteTIVA::ActualizarDatos()
{
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        size=create_frame((uint8_t *)pui8Frame, COMANDO_ALD, NULL, 0, MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}

void QRemoteTIVA::PermisoSondeo(bool val)
{
    PARAM_COMANDO_EN parametro;
    uint8_t pui8Frame[MAX_FRAME_SIZE];
    int size;
    if(connected)
    {
        parametro.Check=(uint8_t)val;
        size=create_frame((uint8_t *)pui8Frame, COMANDO_EN, &parametro, sizeof(parametro), MAX_FRAME_SIZE);
        // Se se pudo crear correctamente, se envia la trama
        if (size>0) serial.write((char *)pui8Frame,size);
    }
}
