#include "guipanel.h"
#include "ui_guipanel.h"
#include <QSerialPort>      // Comunicacion por el puerto serie
#include <QSerialPortInfo>  // Comunicacion por el puerto serie
#include <QMessageBox>      // Se deben incluir cabeceras a los componentes que se vayan a crear en la clase
// y que no estén incluidos en el interfaz gráfico. En este caso, la ventana de PopUp <QMessageBox>
// que se muestra al recibir un PING de respuesta

#include<stdint.h>      // Cabecera para usar tipos de enteros con tamaño
#include<stdbool.h>     // Cabecera para usar booleanos

extern "C" {
#include "protocol.h"    // Cabecera de funciones de gestión de tramas; se indica que está en C, ya que QTs
// se integra en C++, y el C puede dar problemas si no se indica.
}
static uint8_t okey=0;
static double frecuencia;
GUIPanel::GUIPanel(QWidget *parent) :  // Constructor de la clase
    QWidget(parent),
    ui(new Ui::GUIPanel)               // Indica que guipanel.ui es el interfaz grafico de la clase
  , transactionCount(0)
{

    ui->setupUi(this);                // Conecta la clase con su interfaz gráfico.
    setWindowTitle(tr("Interfaz de Control")); // Título de la ventana

    // Inicializa la lista de puertos serie
    ui->serialPortComboBox->clear(); // Vacía de componentes la comboBox
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        // La descripción nos permite que SOLO aparezcan los interfaces tipo USB serial con el identificador de fabricante y producto de la TIVA
        if ((info.vendorIdentifier()==0x1CBE) && (info.productIdentifier()==0x0002))
        {
            ui->serialPortComboBox->addItem(info.portName());
        }
    }

    //inicializacion counter
    ui->Counter->setMaximum(2*1000000.0);
    ui->Counter->setMinimum(100.0);
    //fin inicializacion

    ui->serialPortComboBox->setFocus();   // Componente del GUI seleccionado de inicio


    ui->pingButton->setEnabled(false);    // Se deshabilita el botón de ping del interfaz gráfico, hasta que

    //Semana 2. Inicializacion GRAFICA
    ui->Grafica->setTitle("Sinusoides"); // Titulo de la grafica
    ui->Grafica->setAxisTitle(QwtPlot::xBottom, "Tiempo"); // Etiqueta eje X de coordenadas
    ui->Grafica->setAxisTitle(QwtPlot::yLeft, "Valor");    // Etiqueta eje Y de coordenadas
    //ui->Grafica->axisAutoScale(true); // Con Autoescala
    ui->Grafica->setAxisScale(QwtPlot::yLeft, 0, 3.3); // Escala fija
    ui->Grafica->setAxisScale(QwtPlot::xBottom,0,1024.0);

    // Formateo de la curva
    for(int i=0;i<4;i++){
    Channels[i] = new QwtPlotCurve();
    Channels[i]->attach(ui->Grafica);
    }

    //Inicializacion de los valores básicos
//    for(int i=0;i<1024;i++){
//            xVal[i]=i;
//            yVal[0][i]=3.3*(sin((double)i*2.0*3.14159/1024.0)+1.0)/2.0;
//            yVal[1][i]=3.3*(sin((double)i*4.0*3.14159/1024.0)+1.0)/2.0;
//            yVal[2][i]=3.3*(sin((double)i*8.0*3.14159/1024.0)+1.0)/2.0;
//            yVal[3][i]=3.3*(sin((double)i*16.0*3.14159/1024.0)+1.0)/2.0;
//    }
    Channels[0]->setRawSamples(xVal,yVal[0],1024);
    Channels[1]->setRawSamples(xVal,yVal[1],1024);
    Channels[2]->setRawSamples(xVal,yVal[2],1024);
    Channels[3]->setRawSamples(xVal,yVal[3],1024);

    Channels[0]->setPen(QPen(Qt::red)); // Color de la curva
    Channels[1]->setPen(QPen(Qt::cyan));
    Channels[2]->setPen(QPen(Qt::yellow));
    Channels[3]->setPen(QPen(Qt::green));
    m_Grid = new QwtPlotGrid();     // Rejilla de puntos
    m_Grid->attach(ui->Grafica);    // que se asocia al objeto QwtPl
    ui->Grafica->setAutoReplot(false); //Desactiva el autoreplot (mejora la eficiencia)
    //Semana 2. FIN Inicializacion GRAFICA

    ui->GraficaDigital->setTitle("Simulador de analizador logico"); // Titulo de la grafica
    ui->GraficaDigital->setAxisScale(QwtPlot::yLeft, 0, 8.0*CHAN_STEP); // Escala fija
    ui->GraficaDigital->enableAxis(QwtPlot::yLeft,false); //Oculta el "eje y" y no muestra sus valores (no tiene sentido en este grafico)
    ui->GraficaDigital->setAxisScale(QwtPlot::xBottom,0,1024.0);

    // Creo las curvas
    for(int i=0;i<8;i++){
    ChannelsDig[i] = new QwtPlotCurve();
    ChannelsDig[i]->attach(ui->GraficaDigital);
    }

    //Inicializacion de los valores básicos
    for(int i=0;i<1024;i++){
            xValDig[i]=i;
            yValDig[0][i]=0.0;
            yValDig[1][i]=CHAN_STEP;
            yValDig[2][i]=2.0*CHAN_STEP;
            yValDig[3][i]=3.0*CHAN_STEP;
            yValDig[4][i]=4.0*CHAN_STEP;
            yValDig[5][i]=5.0*CHAN_STEP;
            yValDig[6][i]=6.0*CHAN_STEP;
            yValDig[7][i]=7.0*CHAN_STEP;
    }

    //Asigno arrays a las curvas
    ChannelsDig[0]->setRawSamples(xValDig,yValDig[0],1024);
    ChannelsDig[1]->setRawSamples(xValDig,yValDig[1],1024);
    ChannelsDig[2]->setRawSamples(xValDig,yValDig[2],1024);
    ChannelsDig[3]->setRawSamples(xValDig,yValDig[3],1024);
    ChannelsDig[4]->setRawSamples(xValDig,yValDig[4],1024);
    ChannelsDig[5]->setRawSamples(xValDig,yValDig[5],1024);
    ChannelsDig[6]->setRawSamples(xValDig,yValDig[6],1024);
    ChannelsDig[7]->setRawSamples(xValDig,yValDig[7],1024);

    // Color de la curva [CAMBIAR SI SE DESEA]
    ChannelsDig[0]->setPen(QPen(Qt::red));
    ChannelsDig[1]->setPen(QPen(Qt::cyan));
    ChannelsDig[2]->setPen(QPen(Qt::yellow));
    ChannelsDig[3]->setPen(QPen(Qt::green));
    ChannelsDig[4]->setPen(QPen(Qt::magenta));
    ChannelsDig[5]->setPen(QPen(Qt::white));
    ChannelsDig[6]->setPen(QPen(Qt::darkRed));
    ChannelsDig[7]->setPen(QPen(Qt::darkGreen));

    //Cambia de interpolacion lineal a "escalones" (mejor para digital)
    ChannelsDig[0]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[1]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[2]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[3]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[4]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[5]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[6]->setStyle(QwtPlotCurve::Steps);
    ChannelsDig[7]->setStyle(QwtPlotCurve::Steps);



    m_GridDig = new QwtPlotGrid();     // Rejilla de puntos
    m_GridDig->attach(ui->GraficaDigital);    // que se asocia al objeto QwtPl
    ui->GraficaDigital->setAutoReplot(false); //Desactiva el autoreplot (mejora la eficiencia)


    //Conectamos Slots del objeto "Tiva" con Slots de nuestra aplicacion (o con widgets)
    connect(&tiva,SIGNAL(statusChanged(int,QString)),this,SLOT(tivaStatusChanged(int,QString)));
    connect(ui->pingButton,SIGNAL(clicked(bool)),&tiva,SLOT(pingTiva()));
    connect(ui->Knob,SIGNAL(valueChanged(double)),&tiva,SLOT(LEDPwmBrightness(double)));
    connect(&tiva,SIGNAL(pingReceivedFromTiva()),this,SLOT(pingResponseReceived()));
    connect(&tiva,SIGNAL(commandRejectedFromTiva(int16_t)),this,SLOT(CommandRejected(int16_t)));

    connect(ui->BotonSondeo,SIGNAL(clicked(bool)),&tiva,SLOT(SondeoTiva()));
    connect(&tiva,SIGNAL(RespuestaSondeo(uint8_t, uint8_t)),this,SLOT(BotonesLEDsPulsados(uint8_t, uint8_t)));
    connect(&tiva,SIGNAL(Respuesta_check(bool)),this,SLOT(RespuestaCheckbox(bool)));
    connect(&tiva,SIGNAL(Respuesta_brillo(float)),this,SLOT(RespuestaBrillo(float)));
    connect(&tiva,SIGNAL(Respuesta_rosca(QColor)),this,SLOT(RespuestaRosca(QColor)));
    connect(ui->EnableSondeo,SIGNAL(clicked(bool)),&tiva,SLOT(PermisoSondeo(bool)));


    //SEMANA2: conecta las nuevas senhales y slots
    connect(&tiva,SIGNAL(commandADCReceived(uint16_t,uint16_t,uint16_t,uint16_t,int)),this,SLOT(procesaDatoADC(uint16_t,uint16_t,uint16_t,uint16_t,int)));
    connect(ui->ADCButton,SIGNAL(clicked(bool)),&tiva,SLOT(ADCSample()));
    connect(&tiva,SIGNAL(commandALDReceived(PARAM_COMANDO_ALD,uint16_t)),this,SLOT(procesaDatoALD(PARAM_COMANDO_ALD,uint16_t)));
    connect(ui->Actualizar,SIGNAL(clicked(bool)),&tiva,SLOT(ActualizarDatos()));

}

GUIPanel::~GUIPanel() // Destructor de la clase
{
    delete ui;   // Borra el interfaz gráfico asociado a la clase
}

void GUIPanel::on_serialPortComboBox_currentIndexChanged(const QString &arg1)
{
    activateRunButton();
}

// Funcion auxiliar de procesamiento de errores de comunicación
void GUIPanel::processError(const QString &s)
{
    activateRunButton(); // Activa el botón RUN
    // Muestra en la etiqueta de estado la razón del error (notese la forma de pasar argumentos a la cadena de texto)
    ui->statusLabel->setText(tr("Status: Not running, %1.").arg(s));
}

// Funcion de habilitacion del boton de inicio/conexion
void GUIPanel::activateRunButton()
{
    ui->runButton->setEnabled(true);
}

//Este Slot lo hemos conectado con la señal statusChange de la TIVA, que se activa cuando
//El puerto se conecta o se desconecta y cuando se producen determinados errores.
//Esta función lo que hace es procesar dichos eventos
void GUIPanel::tivaStatusChanged(int status,QString message)
{
    switch (status)
    {
        case QRemoteTIVA::TivaConnected:

            //Caso conectado..
            // Deshabilito el boton de conectar
            ui->runButton->setEnabled(false);

            // Se indica que se ha realizado la conexión en la etiqueta 'statusLabel'
            ui->statusLabel->setText(tr("Ejecucion, conectado al puerto %1.")
                             .arg(ui->serialPortComboBox->currentText()));

            //    // Y se habilitan los controles deshabilitados
            ui->pingButton->setEnabled(true);

        break;

        case QRemoteTIVA::TivaDisconnected:
            //Caso desconectado..
            // Rehabilito el boton de conectar
            ui->runButton->setEnabled(false);
        break;
        case QRemoteTIVA::UnexpectedPacketError:
        case QRemoteTIVA::FragmentedPacketError:
            //Errores detectados en la recepcion de paquetes
            ui->statusLabel->setText(message);
        default:
            //Otros errores (por ejemplo, abriendo el puerto)
            processError(tiva.getLastErrorMessage());
    }
}


// SLOT asociada a pulsación del botón RUN
void GUIPanel::on_runButton_clicked()
{
    //Intenta arrancar la comunicacion con la TIVA
    tiva.startSlave( ui->serialPortComboBox->currentText());
}

//Slot asociado al chechbox rojo (por nombre)
void GUIPanel::on_rojo_stateChanged(int arg1)
{
    cambiaLEDs();
}

//Slot asociado al chechbox verde (por nombre)
void GUIPanel::on_verde_stateChanged(int arg1)
{
    cambiaLEDs();
}

//Slot asociado al chechbox azul (por nombre)
void GUIPanel::on_azul_stateChanged(int arg1)
{
    cambiaLEDs();
}

void GUIPanel::cambiaLEDs(void)
{
    //Invoca al metido LEDsGpioTiva para enviar la orden a la TIVA
    tiva.LEDsGpioTiva(ui->rojo->isChecked(),ui->verde->isChecked(),ui->azul->isChecked());
}

//Slots Asociado al boton que limpia los mensajes del interfaz
void GUIPanel::on_pushButton_clicked()
{
    ui->statusLabel->setText(tr(""));
}

//**** Slots asociados a la recepción de mensajes desde la TIVA ********/
//Están conectados a señales generadas por el objeto TIVA de clase QRemoteTiva (se conectan en el constructor de la ventana, más arriba en este fichero))
//Se pueden añadir los que se quieran para completar la aplicación

//Este se ejecuta cuando se recibe una respuesta de PING
void GUIPanel::pingResponseReceived()
{

    // Ventana popUP para el caso de comando PING; no te deja definirla en un "caso"
    QMessageBox ventanaPopUp(QMessageBox::Information,tr("Evento"),tr("Status: RESPUESTA A PING RECIBIDA"),QMessageBox::Ok,this,Qt::Popup);
    ventanaPopUp.setStyleSheet("background-color: lightgrey");
    ventanaPopUp.exec();
}


//Este se ejecuta cuando se recibe un mensaje de comando rechazado
void GUIPanel::CommandRejected(int16_t code)
{
    ui->statusLabel->setText(tr("Status: Comando rechazado,%1").arg(code));
}

void GUIPanel::BotonesLEDsPulsados(uint8_t izq, uint8_t der)
{
   ui->BotonDer->setChecked(der);
   ui->BotonIzq->setChecked(izq);
}

void GUIPanel::on_colorWheel_colorChanged(const QColor &arg1)
{
    //Poner aqui el codigo para pedirle al objeto "tiva" (clase QRemoteTIVA) que envíe la orden de cambiar el Color hacia el microcontrolador
    tiva.LEDsPWMTiva(arg1);
}
void GUIPanel::RespuestaRosca(const QColor &val)
{
    ui->colorWheel->setColor(val);
}

void GUIPanel::RespuestaCheckbox(bool valor)
{
    ui->checkBox->setChecked(valor);
}
void GUIPanel::RespuestaBrillo( float valor)
{
    ui->Knob->setValue(valor);
}
void GUIPanel::RespuestaLeds(uint8_t color1, uint8_t color2, uint8_t color3)
{
    //Invoca al metido LEDsGpioTiva para enviar la orden a la TIVA
    ui->rojo->setChecked(color1);
    ui->azul->setChecked(color2);
    ui->verde->setChecked(color3);

}
void GUIPanel::on_checkBox_clicked(bool checked)
{
    tiva.SwitchPWM_GPIO(checked);
}


//SEMANA2: Slot que asociaremos a una señal que genera el objeto TIVA cuando recibe datos del ADC
void GUIPanel::procesaDatoADC(uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4, int n)
{
    //Manda cada dato a su correspondiente display (pasandolos a voltios)
     ui->lcdCh1->display(((double)chan1)*3.3/4096.0);
     ui->lcdCh2->display(((double)chan2)*3.3/4096.0);
     ui->lcdCh3->display(((double)chan3)*3.3/4096.0);
     ui->lcdCh4->display(((double)chan4)*3.3/4096.0);

      xVal[n]=n;
 //Los valores "constantes" deberian definirse como etiquetas por "estética"
     yVal[0][n]=((double)chan1)*3.3/4096.0*3.3;
     yVal[1][n]=((double)chan2)*3.3/4096.0*3.3;
     yVal[2][n]=((double)chan3)*3.3/4096.0*3.3;
     yVal[3][n]=((double)chan4)*3.3/4096.0*3.3;
     ui->Grafica->replot(); //Refresca la grafica una vez actualizados los valores
 }

void GUIPanel::procesaDatoALD(PARAM_COMANDO_ALD param, uint16_t tam)
{
//    if(tam>1024){
//        tam=1024;
//    }
    tam=1024;
    int i=0;
    int j=0;
    while(i<tam)
    {
        while(j<128){
            yValDig[0][i]=CHAN_AMPLITUDE*((param.muestras.chan1[j])&0x01);
            yValDig[1][i]=CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan2[j+128])&0x01);
            yValDig[2][i]=2.0+CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan3[j+256])&0x01);
            yValDig[3][i]=3.0+CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan4[j+384])&0x01);
            yValDig[4][i]=4.0+CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan5[j+512])&0x01);
            yValDig[5][i]=5.0+CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan6[j+640])&0x01);
            yValDig[6][i]=6.0+CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan7[j+768])&0x01);
            yValDig[7][i]=7.0+CHAN_STEP+CHAN_AMPLITUDE*((param.muestras.chan8[j+896])&0x01);
            j++;
            i++;
        }
        if(j==128){
            j=0;
        }
    }
    ui->GraficaDigital->replot(); //Refresca la grafica una vez actualizados los valores
}

//SEMANA2: Slot asociado a la rosca "frecuencia"
void GUIPanel::on_frecuencia_valueChanged(double value)
{
    if(okey==0){
        if(ui->x10->isChecked()){
            frecuencia=value*10;
            okey=1;
        }else if(ui->x100->isChecked()){
            frecuencia=value*100;
            okey=2;
        }else if(ui->x1000->isChecked()){
            frecuencia=value*1000;
            okey=3;
        }else{
            frecuencia=value;
        }
    }else if(okey==1){
        if(ui->x100->isChecked()){
            frecuencia=value*100;
            okey=2;
            ui->x10->setChecked(0);
        }else if(ui->x1000->isChecked()){
            frecuencia=value*1000;
            okey=3;
             ui->x10->setChecked(0);
        }else if(!ui->x10->isChecked()){
            frecuencia=value;
        }
   }else if (okey==2){
        if(ui->x10->isChecked()){
            frecuencia=value*10;
            okey=1;
            ui->x100->setChecked(0);
        }else if(ui->x1000->isChecked()){
            frecuencia=value*1000;
            okey=3;
             ui->x100->setChecked(0);
        }else if(!ui->x100->isChecked()){
            frecuencia=value;
        }
    }else if(okey==3){
        if(ui->x10->isChecked()){
            frecuencia=value*10;
            okey=1;
            ui->x1000->setChecked(0);
        }else if(ui->x100->isChecked()){
            frecuencia=value*100;
            okey=2;
             ui->x1000->setChecked(0);
        }else if(!ui->x1000->isChecked()){
            frecuencia=value;
        }
    }
    tiva.CambioPeriodo(frecuencia);
//    int i;

//    //Recalcula los valores de la grafica
//    for(int i=0;i<1024;i++){
//            //xVal[i]=i;
//        //Los valores "constantes" deberian definirse como etiquetas por "estética"
//            yVal[0][i]=((yVal[0][i]*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
//            yVal[1][i]=((yVal[1][i]*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
//            yVal[2][i]=((yVal[2][i]*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
//            yVal[3][i]=((yVal[3][i]*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
//    }
//    ui->Grafica->replot(); //Refresca la grafica una vez actualizados los valores
}


void GUIPanel::on_STOP_clicked(bool checked)
{
    uint8_t val;
    if(checked){
        val=1;
    }else{
        val=0;
    }
    tiva.RealTimeControl(val);
}

void GUIPanel::on_MODO_clicked(bool checked)
{
    uint8_t val;
    if(checked){
        val=1;
    }else{
        val=0;
    }
    tiva.cambioModo(val);
}

void GUIPanel::on_Counter_valueChanged(double value)
{
    tiva.CambioMuestreo(value);
}
