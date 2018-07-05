/****************************************************************************
** Meta object code from reading C++ file 'qremotetiva.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QTGUI-partida-p1/qremotetiva.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qremotetiva.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QRemoteTIVA_t {
    QByteArrayData data[44];
    char stringdata0[486];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QRemoteTIVA_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QRemoteTIVA_t qt_meta_stringdata_QRemoteTIVA = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QRemoteTIVA"
QT_MOC_LITERAL(1, 12, 13), // "statusChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "status"
QT_MOC_LITERAL(4, 34, 7), // "message"
QT_MOC_LITERAL(5, 42, 20), // "pingReceivedFromTiva"
QT_MOC_LITERAL(6, 63, 23), // "commandRejectedFromTiva"
QT_MOC_LITERAL(7, 87, 7), // "int16_t"
QT_MOC_LITERAL(8, 95, 4), // "code"
QT_MOC_LITERAL(9, 100, 18), // "commandADCReceived"
QT_MOC_LITERAL(10, 119, 8), // "uint16_t"
QT_MOC_LITERAL(11, 128, 5), // "chan1"
QT_MOC_LITERAL(12, 134, 5), // "chan2"
QT_MOC_LITERAL(13, 140, 5), // "chan3"
QT_MOC_LITERAL(14, 146, 5), // "chan4"
QT_MOC_LITERAL(15, 152, 3), // "pos"
QT_MOC_LITERAL(16, 156, 10), // "startSlave"
QT_MOC_LITERAL(17, 167, 6), // "puerto"
QT_MOC_LITERAL(18, 174, 8), // "pingTiva"
QT_MOC_LITERAL(19, 183, 12), // "LEDsGpioTiva"
QT_MOC_LITERAL(20, 196, 3), // "red"
QT_MOC_LITERAL(21, 200, 5), // "green"
QT_MOC_LITERAL(22, 206, 4), // "blue"
QT_MOC_LITERAL(23, 211, 16), // "LEDPwmBrightness"
QT_MOC_LITERAL(24, 228, 5), // "value"
QT_MOC_LITERAL(25, 234, 9), // "ADCSample"
QT_MOC_LITERAL(26, 244, 13), // "CambioPeriodo"
QT_MOC_LITERAL(27, 258, 3), // "fre"
QT_MOC_LITERAL(28, 262, 15), // "RealTimeControl"
QT_MOC_LITERAL(29, 278, 7), // "uint8_t"
QT_MOC_LITERAL(30, 286, 3), // "val"
QT_MOC_LITERAL(31, 290, 11), // "readRequest"
QT_MOC_LITERAL(32, 302, 10), // "TivaStatus"
QT_MOC_LITERAL(33, 313, 13), // "TivaConnected"
QT_MOC_LITERAL(34, 327, 16), // "TivaDisconnected"
QT_MOC_LITERAL(35, 344, 13), // "OpenPortError"
QT_MOC_LITERAL(36, 358, 13), // "BaudRateError"
QT_MOC_LITERAL(37, 372, 12), // "DataBitError"
QT_MOC_LITERAL(38, 385, 11), // "ParityError"
QT_MOC_LITERAL(39, 397, 9), // "StopError"
QT_MOC_LITERAL(40, 407, 16), // "FlowControlError"
QT_MOC_LITERAL(41, 424, 21), // "UnexpectedPacketError"
QT_MOC_LITERAL(42, 446, 21), // "FragmentedPacketError"
QT_MOC_LITERAL(43, 468, 17) // "ReceivedDataError"

    },
    "QRemoteTIVA\0statusChanged\0\0status\0"
    "message\0pingReceivedFromTiva\0"
    "commandRejectedFromTiva\0int16_t\0code\0"
    "commandADCReceived\0uint16_t\0chan1\0"
    "chan2\0chan3\0chan4\0pos\0startSlave\0"
    "puerto\0pingTiva\0LEDsGpioTiva\0red\0green\0"
    "blue\0LEDPwmBrightness\0value\0ADCSample\0"
    "CambioPeriodo\0fre\0RealTimeControl\0"
    "uint8_t\0val\0readRequest\0TivaStatus\0"
    "TivaConnected\0TivaDisconnected\0"
    "OpenPortError\0BaudRateError\0DataBitError\0"
    "ParityError\0StopError\0FlowControlError\0"
    "UnexpectedPacketError\0FragmentedPacketError\0"
    "ReceivedDataError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QRemoteTIVA[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       1,  116, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       5,    0,   79,    2, 0x06 /* Public */,
       6,    1,   80,    2, 0x06 /* Public */,
       9,    5,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,   94,    2, 0x0a /* Public */,
      18,    0,   97,    2, 0x0a /* Public */,
      19,    3,   98,    2, 0x0a /* Public */,
      23,    1,  105,    2, 0x0a /* Public */,
      25,    0,  108,    2, 0x0a /* Public */,
      26,    1,  109,    2, 0x0a /* Public */,
      28,    1,  112,    2, 0x0a /* Public */,
      31,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 10, 0x80000000 | 10, 0x80000000 | 10, QMetaType::Int,   11,   12,   13,   14,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,   20,   21,   22,
    QMetaType::Void, QMetaType::Double,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   27,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,

 // enums: name, flags, count, data
      32, 0x0,   11,  120,

 // enum data: key, value
      33, uint(QRemoteTIVA::TivaConnected),
      34, uint(QRemoteTIVA::TivaDisconnected),
      35, uint(QRemoteTIVA::OpenPortError),
      36, uint(QRemoteTIVA::BaudRateError),
      37, uint(QRemoteTIVA::DataBitError),
      38, uint(QRemoteTIVA::ParityError),
      39, uint(QRemoteTIVA::StopError),
      40, uint(QRemoteTIVA::FlowControlError),
      41, uint(QRemoteTIVA::UnexpectedPacketError),
      42, uint(QRemoteTIVA::FragmentedPacketError),
      43, uint(QRemoteTIVA::ReceivedDataError),

       0        // eod
};

void QRemoteTIVA::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QRemoteTIVA *_t = static_cast<QRemoteTIVA *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->pingReceivedFromTiva(); break;
        case 2: _t->commandRejectedFromTiva((*reinterpret_cast< int16_t(*)>(_a[1]))); break;
        case 3: _t->commandADCReceived((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])),(*reinterpret_cast< uint16_t(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 4: _t->startSlave((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->pingTiva(); break;
        case 6: _t->LEDsGpioTiva((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 7: _t->LEDPwmBrightness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->ADCSample(); break;
        case 9: _t->CambioPeriodo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->RealTimeControl((*reinterpret_cast< uint8_t(*)>(_a[1]))); break;
        case 11: _t->readRequest(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QRemoteTIVA::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QRemoteTIVA::statusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QRemoteTIVA::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QRemoteTIVA::pingReceivedFromTiva)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QRemoteTIVA::*_t)(int16_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QRemoteTIVA::commandRejectedFromTiva)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QRemoteTIVA::*_t)(uint16_t , uint16_t , uint16_t , uint16_t , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QRemoteTIVA::commandADCReceived)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject QRemoteTIVA::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QRemoteTIVA.data,
      qt_meta_data_QRemoteTIVA,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QRemoteTIVA::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QRemoteTIVA::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QRemoteTIVA.stringdata0))
        return static_cast<void*>(const_cast< QRemoteTIVA*>(this));
    return QObject::qt_metacast(_clname);
}

int QRemoteTIVA::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void QRemoteTIVA::statusChanged(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QRemoteTIVA::pingReceivedFromTiva()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QRemoteTIVA::commandRejectedFromTiva(int16_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QRemoteTIVA::commandADCReceived(uint16_t _t1, uint16_t _t2, uint16_t _t3, uint16_t _t4, int _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
