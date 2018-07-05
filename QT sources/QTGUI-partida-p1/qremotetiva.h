#ifndef QREMOTETIVA_H
#define QREMOTETIVA_H

#include <QObject>
#include <QSerialPort>      // Comunicacion por el puerto serie
#include <QSerialPortInfo>  // Comunicacion por el puerto serie


#include<stdint.h>      // Cabecera para usar tipos de enteros con tamaño
#include<stdbool.h>     // Cabecera para usar booleanos
extern "C" {
#include "protocol.h"    // Cabecera de funciones de gestión de tramas; se indica que está en C, ya que QTs
// se integra en C++, y el C puede dar problemas si no se indica.
}


class QRemoteTIVA : public QObject
{

    Q_OBJECT
public:
    explicit QRemoteTIVA(QObject *parent = 0);

    //Define una serie de etiqueta para los errores y estados notificados por la señal statusChanged(...)
    enum TivaStatus {TivaConnected,
                     TivaDisconnected,
                     OpenPortError,
                     BaudRateError,
                     DataBitError,
                     ParityError,
                     StopError,
                     FlowControlError,
                     UnexpectedPacketError,
                     FragmentedPacketError,
                     ReceivedDataError /* ESP2 */
                    };
    Q_ENUM(TivaStatus)

    //Metodo publico
    QString getLastErrorMessage();

signals:
    void statusChanged(int status, QString message); //Esta señal se genera al realizar la conexión/desconexion o cuando se produce un error de comunicacion
    void pingReceivedFromTiva(void); //Esta señal se genera al recibir una respuesta de PING de la TIVA
    void commandRejectedFromTiva(int16_t code); //Esta señal se genera al recibir una respuesta de Comando Rechazado desde la TIVA

    void RespuestaSondeo(uint8_t izq, uint8_t der);
    void Respuesta_check(bool valor);
    void Respuesta_brillo(float valor);
    void Respuesta_leds(uint8_t color1, uint8_t color2,uint8_t color3);
    void Respuesta_rosca(const QColor &val);

    //SEMANA2: senhal que se genera al recibir del ADC
    void commandADCReceived(uint16_t chan1,uint16_t chan2,uint16_t chan3,uint16_t chan4, int pos);
    //SEMANA3: senhal que se genera al recibir del ALD
    void commandALDReceived(PARAM_COMANDO_ALD muestras ,uint16_t tam);

public slots:
    void startSlave(QString puerto); //Este Slot arranca la comunicacion
    void pingTiva(void); //Este Slot provoca el envio del comando PING
    void LEDsGpioTiva(bool red, bool green, bool blue); //Este Slot provoca el envio del comando LED
    void LEDPwmBrightness(double value); //Este Slot provoca el envio del comando Brightness

    void SondeoTiva(void);
    void LEDsPWMTiva(const QColor &arg1);
    void SwitchPWM_GPIO(bool value);
    void PermisoSondeo(bool val);

    //SEMANA2: Este Slot permite ordenar al objeto TIVA que envie un comando de conversion
    void ADCSample(void);
    void CambioPeriodo(double fre);
    void RealTimeControl(uint8_t val);
    void cambioModo(uint8_t val);
    void CambioMuestreo(double val);

    //SEMANA3:
    void ActualizarDatos();


private slots:
    void readRequest(); //Este Slot se conecta a la señal readyRead(..) del puerto serie. Se encarga de procesar y decodificar los mensajes que llegan de la TIVA y
                        //generar señales para algunos de ellos.

private:
    QSerialPort serial;
    QString LastError;
    bool connected;
    QByteArray request;

};

#endif // QREMOTETIVA_H
