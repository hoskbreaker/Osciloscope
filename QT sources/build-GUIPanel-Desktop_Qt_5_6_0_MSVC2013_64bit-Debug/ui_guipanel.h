/********************************************************************************
** Form generated from reading UI file 'guipanel.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIPANEL_H
#define UI_GUIPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "qwt_knob.h"
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_GUIPanel
{
public:
    QPushButton *runButton;
    QSplitter *splitter;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QPushButton *pushButton;
    QLabel *statusLabel;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *ADCButton;
    QwtKnob *Knob;
    QSplitter *splitter_2;
    QCheckBox *rojo;
    QCheckBox *verde;
    QCheckBox *azul;
    QPushButton *pingButton;
    QLCDNumber *lcdCh1;
    QLCDNumber *lcdCh2;
    QLCDNumber *lcdCh3;
    QLCDNumber *lcdCh4;
    QWidget *tab_2;
    QwtPlot *Grafica;
    QwtKnob *frecuencia;
    QCheckBox *x10;
    QCheckBox *x100;
    QCheckBox *x1000;
    QCheckBox *STOP;

    void setupUi(QWidget *GUIPanel)
    {
        if (GUIPanel->objectName().isEmpty())
            GUIPanel->setObjectName(QStringLiteral("GUIPanel"));
        GUIPanel->resize(962, 625);
        runButton = new QPushButton(GUIPanel);
        runButton->setObjectName(QStringLiteral("runButton"));
        runButton->setGeometry(QRect(260, 10, 98, 27));
        splitter = new QSplitter(GUIPanel);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(20, 10, 221, 27));
        splitter->setOrientation(Qt::Horizontal);
        serialPortLabel = new QLabel(splitter);
        serialPortLabel->setObjectName(QStringLiteral("serialPortLabel"));
        splitter->addWidget(serialPortLabel);
        serialPortComboBox = new QComboBox(splitter);
        serialPortComboBox->setObjectName(QStringLiteral("serialPortComboBox"));
        splitter->addWidget(serialPortComboBox);
        pushButton = new QPushButton(GUIPanel);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(420, 10, 64, 27));
        statusLabel = new QLabel(GUIPanel);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));
        statusLabel->setGeometry(QRect(490, 14, 331, 17));
        tabWidget = new QTabWidget(GUIPanel);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 40, 951, 581));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        ADCButton = new QPushButton(tab);
        ADCButton->setObjectName(QStringLiteral("ADCButton"));
        ADCButton->setGeometry(QRect(620, 160, 75, 23));
        Knob = new QwtKnob(tab);
        Knob->setObjectName(QStringLiteral("Knob"));
        Knob->setGeometry(QRect(130, 130, 129, 134));
        Knob->setUpperBound(1);
        Knob->setValue(0.5);
        Knob->setSingleSteps(0u);
        Knob->setPageSteps(10u);
        splitter_2 = new QSplitter(tab);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setGeometry(QRect(12, 166, 97, 66));
        splitter_2->setOrientation(Qt::Vertical);
        rojo = new QCheckBox(splitter_2);
        rojo->setObjectName(QStringLiteral("rojo"));
        splitter_2->addWidget(rojo);
        verde = new QCheckBox(splitter_2);
        verde->setObjectName(QStringLiteral("verde"));
        splitter_2->addWidget(verde);
        azul = new QCheckBox(splitter_2);
        azul->setObjectName(QStringLiteral("azul"));
        splitter_2->addWidget(azul);
        pingButton = new QPushButton(tab);
        pingButton->setObjectName(QStringLiteral("pingButton"));
        pingButton->setGeometry(QRect(260, 10, 61, 27));
        lcdCh1 = new QLCDNumber(tab);
        lcdCh1->setObjectName(QStringLiteral("lcdCh1"));
        lcdCh1->setGeometry(QRect(470, 20, 171, 61));
        lcdCh1->setFrameShape(QFrame::Box);
        lcdCh1->setFrameShadow(QFrame::Plain);
        lcdCh1->setSmallDecimalPoint(false);
        lcdCh1->setDigitCount(5);
        lcdCh1->setSegmentStyle(QLCDNumber::Flat);
        lcdCh1->setProperty("value", QVariant(0));
        lcdCh2 = new QLCDNumber(tab);
        lcdCh2->setObjectName(QStringLiteral("lcdCh2"));
        lcdCh2->setGeometry(QRect(470, 90, 171, 61));
        lcdCh2->setFrameShape(QFrame::Box);
        lcdCh2->setFrameShadow(QFrame::Plain);
        lcdCh2->setSmallDecimalPoint(false);
        lcdCh2->setDigitCount(5);
        lcdCh2->setSegmentStyle(QLCDNumber::Flat);
        lcdCh2->setProperty("value", QVariant(0));
        lcdCh3 = new QLCDNumber(tab);
        lcdCh3->setObjectName(QStringLiteral("lcdCh3"));
        lcdCh3->setGeometry(QRect(650, 20, 171, 61));
        lcdCh3->setFrameShape(QFrame::Box);
        lcdCh3->setFrameShadow(QFrame::Plain);
        lcdCh3->setSmallDecimalPoint(false);
        lcdCh3->setDigitCount(5);
        lcdCh3->setSegmentStyle(QLCDNumber::Flat);
        lcdCh3->setProperty("value", QVariant(0));
        lcdCh4 = new QLCDNumber(tab);
        lcdCh4->setObjectName(QStringLiteral("lcdCh4"));
        lcdCh4->setGeometry(QRect(650, 90, 171, 61));
        lcdCh4->setFrameShape(QFrame::Box);
        lcdCh4->setFrameShadow(QFrame::Plain);
        lcdCh4->setSmallDecimalPoint(false);
        lcdCh4->setDigitCount(5);
        lcdCh4->setSegmentStyle(QLCDNumber::Flat);
        lcdCh4->setProperty("value", QVariant(0));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        Grafica = new QwtPlot(tab_2);
        Grafica->setObjectName(QStringLiteral("Grafica"));
        Grafica->setGeometry(QRect(0, 0, 941, 411));
        Grafica->setAutoFillBackground(false);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        Grafica->setCanvasBackground(brush);
        Grafica->setAutoReplot(true);
        frecuencia = new QwtKnob(tab_2);
        frecuencia->setObjectName(QStringLiteral("frecuencia"));
        frecuencia->setGeometry(QRect(100, 419, 131, 131));
        frecuencia->setUpperBound(5);
        x10 = new QCheckBox(tab_2);
        x10->setObjectName(QStringLiteral("x10"));
        x10->setGeometry(QRect(230, 440, 70, 17));
        x100 = new QCheckBox(tab_2);
        x100->setObjectName(QStringLiteral("x100"));
        x100->setGeometry(QRect(230, 470, 70, 17));
        x1000 = new QCheckBox(tab_2);
        x1000->setObjectName(QStringLiteral("x1000"));
        x1000->setGeometry(QRect(230, 500, 70, 17));
        STOP = new QCheckBox(tab_2);
        STOP->setObjectName(QStringLiteral("STOP"));
        STOP->setGeometry(QRect(320, 440, 70, 17));
        tabWidget->addTab(tab_2, QString());
        QWidget::setTabOrder(serialPortComboBox, pingButton);
        QWidget::setTabOrder(pingButton, runButton);

        retranslateUi(GUIPanel);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(GUIPanel);
    } // setupUi

    void retranslateUi(QWidget *GUIPanel)
    {
        GUIPanel->setWindowTitle(QApplication::translate("GUIPanel", "GUIPanel", 0));
        runButton->setText(QApplication::translate("GUIPanel", "Inicio", 0));
        serialPortLabel->setText(QApplication::translate("GUIPanel", "Puerto Serie:", 0));
        pushButton->setText(QApplication::translate("GUIPanel", "Estado:", 0));
        statusLabel->setText(QApplication::translate("GUIPanel", "Detenido", 0));
        ADCButton->setText(QApplication::translate("GUIPanel", "LeeADC", 0));
        rojo->setText(QApplication::translate("GUIPanel", "Rojo", 0));
        verde->setText(QApplication::translate("GUIPanel", "Verde", 0));
        azul->setText(QApplication::translate("GUIPanel", "Azul", 0));
        pingButton->setText(QApplication::translate("GUIPanel", "Ping", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("GUIPanel", "Tab 1", 0));
        x10->setText(QApplication::translate("GUIPanel", "x10", 0));
        x100->setText(QApplication::translate("GUIPanel", "x100", 0));
        x1000->setText(QApplication::translate("GUIPanel", "x1000", 0));
        STOP->setText(QApplication::translate("GUIPanel", "STOP", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("GUIPanel", "Tab 2", 0));
    } // retranslateUi

};

namespace Ui {
    class GUIPanel: public Ui_GUIPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPANEL_H
