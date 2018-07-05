/****************************************************************************
** Meta object code from reading C++ file 'guipanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QTGUI-partida-p1/guipanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'guipanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GUIPanel_t {
    QByteArrayData data[27];
    char stringdata0[354];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GUIPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GUIPanel_t qt_meta_stringdata_GUIPanel = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GUIPanel"
QT_MOC_LITERAL(1, 9, 20), // "on_runButton_clicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 41), // "on_serialPortComboBox_current..."
QT_MOC_LITERAL(4, 73, 4), // "arg1"
QT_MOC_LITERAL(5, 78, 20), // "on_rojo_stateChanged"
QT_MOC_LITERAL(6, 99, 21), // "on_verde_stateChanged"
QT_MOC_LITERAL(7, 121, 20), // "on_azul_stateChanged"
QT_MOC_LITERAL(8, 142, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(9, 164, 17), // "tivaStatusChanged"
QT_MOC_LITERAL(10, 182, 6), // "status"
QT_MOC_LITERAL(11, 189, 7), // "message"
QT_MOC_LITERAL(12, 197, 20), // "pingResponseReceived"
QT_MOC_LITERAL(13, 218, 15), // "CommandRejected"
QT_MOC_LITERAL(14, 234, 7), // "int16_t"
QT_MOC_LITERAL(15, 242, 4), // "code"
QT_MOC_LITERAL(16, 247, 14), // "procesaDatoADC"
QT_MOC_LITERAL(17, 262, 8), // "uint16_t"
QT_MOC_LITERAL(18, 271, 5), // "chan1"
QT_MOC_LITERAL(19, 277, 5), // "chan2"
QT_MOC_LITERAL(20, 283, 5), // "chan3"
QT_MOC_LITERAL(21, 289, 5), // "chan4"
QT_MOC_LITERAL(22, 295, 1), // "n"
QT_MOC_LITERAL(23, 297, 26), // "on_frecuencia_valueChanged"
QT_MOC_LITERAL(24, 324, 5), // "value"
QT_MOC_LITERAL(25, 330, 15), // "on_STOP_clicked"
QT_MOC_LITERAL(26, 346, 7) // "checked"

    },
    "GUIPanel\0on_runButton_clicked\0\0"
    "on_serialPortComboBox_currentIndexChanged\0"
    "arg1\0on_rojo_stateChanged\0"
    "on_verde_stateChanged\0on_azul_stateChanged\0"
    "on_pushButton_clicked\0tivaStatusChanged\0"
    "status\0message\0pingResponseReceived\0"
    "CommandRejected\0int16_t\0code\0"
    "procesaDatoADC\0uint16_t\0chan1\0chan2\0"
    "chan3\0chan4\0n\0on_frecuencia_valueChanged\0"
    "value\0on_STOP_clicked\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GUIPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    1,   75,    2, 0x08 /* Private */,
       5,    1,   78,    2, 0x08 /* Private */,
       6,    1,   81,    2, 0x08 /* Private */,
       7,    1,   84,    2, 0x08 /* Private */,
       8,    0,   87,    2, 0x08 /* Private */,
       9,    2,   88,    2, 0x08 /* Private */,
      12,    0,   93,    2, 0x08 /* Private */,
      13,    1,   94,    2, 0x08 /* Private */,
      16,    5,   97,    2, 0x08 /* Private */,
      23,    1,  108,    2, 0x08 /* Private */,
      25,    1,  111,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   10,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17, 0x80000000 | 17, 0x80000000 | 17, 0x80000000 | 17, QMetaType::Int,   18,   19,   20,   21,   22,
    QMetaType::Void, QMetaType::Double,   24,
    QMetaType::Void, QMetaType::Bool,   26,

       0        // eod
};

void GUIPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GUIPanel *_t = static_cast<GUIPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_runButton_clicked(); break;
        case 1: _t->on_serialPortComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_rojo_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_verde_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_azul_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_pushButton_clicked(); break;
        case 6: _t->tivaStatusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->pingResponseReceived(); break;
        case 8: _t->CommandRejected((*reinterpret_cast< int16_t(*)>(_a[1]))); break;
        case 9: _t->procesaDatoADC((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< uint16_t(*)>(_a[2])),(*reinterpret_cast< uint16_t(*)>(_a[3])),(*reinterpret_cast< uint16_t(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 10: _t->on_frecuencia_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->on_STOP_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject GUIPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GUIPanel.data,
      qt_meta_data_GUIPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GUIPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GUIPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GUIPanel.stringdata0))
        return static_cast<void*>(const_cast< GUIPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int GUIPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
