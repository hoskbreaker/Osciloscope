#ifndef GUIPANEL_H
#define GUIPANEL_H

#include <QWidget>
#include <QtSerialPort/qserialport.h>
#include "qremotetiva.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

#define CHAN_STEP 1.5
#define CHAN_AMPLITUDE 1.0

namespace Ui {
class GUIPanel;
}

class GUIPanel : public QWidget
{
    Q_OBJECT
    
public:    
    explicit GUIPanel(QWidget *parent = 0);
    ~GUIPanel();
    
private slots:
    //void on_pingButton_clicked();
    void on_runButton_clicked();
    //void readRequest();
    void on_serialPortComboBox_currentIndexChanged(const QString &arg1);

    void on_rojo_stateChanged(int arg1);
    void on_verde_stateChanged(int arg1);
    void on_azul_stateChanged(int arg1);
    void on_pushButton_clicked();    

    void tivaStatusChanged(int status,QString message);
    void pingResponseReceived(void);
    void CommandRejected(int16_t code);

    void BotonesLEDsPulsados(uint8_t izq, uint8_t der);
    void on_colorWheel_colorChanged(const QColor &arg1);
    void RespuestaCheckbox(bool valor);
    void RespuestaBrillo( float valor);
    void RespuestaLeds(uint8_t color1, uint8_t color2, uint8_t color3);
    void RespuestaRosca(const QColor &val);
    void on_checkBox_clicked(bool checked);

    //SEMANA3:
    void procesaDatoALD(PARAM_COMANDO_ALD param, uint16_t tam);

    //SEMANA2:
    void procesaDatoADC(uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4, int n);   

    void on_frecuencia_valueChanged(double value);

    void on_STOP_clicked(bool checked);

    void on_MODO_clicked(bool checked);

    void on_Counter_valueChanged(double value);

private: // funciones privadas
    void pingDevice();
    //void startSlave();
    void processError(const QString &s);
    void activateRunButton();
    void cambiaLEDs();
private:
    Ui::GUIPanel *ui;
    int transactionCount;    
    QRemoteTIVA tiva;

    //SEMANA2: Para las graficas
    double xVal[1024]; //valores eje X
    double yVal[4][1024]; //valores ejes Y
    QwtPlotCurve *Channels[4]; //Curvas
    QwtPlotGrid  *m_Grid; //Cuadricula
     //SEMANA3: Para las graficas digitales
    double xValDig[1024]; //valores eje X
    double yValDig[8][1024]; //valores ejes Y
    QwtPlotCurve *ChannelsDig[8]; //Curvas
    QwtPlotGrid  *m_GridDig; //Cuadricula
};

#endif // GUIPANEL_H
