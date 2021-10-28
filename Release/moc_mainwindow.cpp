/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sensor_reader/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[393];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "MW_connectBtnHandle"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 24), // "MW_ComboConnIndexChanged"
QT_MOC_LITERAL(4, 57, 5), // "index"
QT_MOC_LITERAL(5, 63, 20), // "MW_CheckConfigHandle"
QT_MOC_LITERAL(6, 84, 11), // "check_state"
QT_MOC_LITERAL(7, 96, 20), // "MW_4x4RadioBtnHandle"
QT_MOC_LITERAL(8, 117, 20), // "MW_8x8RadioBtnHandle"
QT_MOC_LITERAL(9, 138, 24), // "MW_frequencyIndexChanged"
QT_MOC_LITERAL(10, 163, 24), // "MW_sharpenerIndexChanged"
QT_MOC_LITERAL(11, 188, 17), // "MW_startBtnHandle"
QT_MOC_LITERAL(12, 206, 16), // "MW_stopBtnHandle"
QT_MOC_LITERAL(13, 223, 21), // "MW_receiveInitMsgInfo"
QT_MOC_LITERAL(14, 245, 16), // "VL53LX_INIT_MSG*"
QT_MOC_LITERAL(15, 262, 12), // "rcv_msg_init"
QT_MOC_LITERAL(16, 275, 23), // "MW_receiveData16MsgInfo"
QT_MOC_LITERAL(17, 299, 19), // "VL53LX_DATA_16_MSG*"
QT_MOC_LITERAL(18, 319, 14), // "rcv_msg_data16"
QT_MOC_LITERAL(19, 334, 23), // "MW_receiveData64MsgInfo"
QT_MOC_LITERAL(20, 358, 19), // "VL53LX_DATA_64_MSG*"
QT_MOC_LITERAL(21, 378, 14) // "rcv_msg_data64"

    },
    "MainWindow\0MW_connectBtnHandle\0\0"
    "MW_ComboConnIndexChanged\0index\0"
    "MW_CheckConfigHandle\0check_state\0"
    "MW_4x4RadioBtnHandle\0MW_8x8RadioBtnHandle\0"
    "MW_frequencyIndexChanged\0"
    "MW_sharpenerIndexChanged\0MW_startBtnHandle\0"
    "MW_stopBtnHandle\0MW_receiveInitMsgInfo\0"
    "VL53LX_INIT_MSG*\0rcv_msg_init\0"
    "MW_receiveData16MsgInfo\0VL53LX_DATA_16_MSG*\0"
    "rcv_msg_data16\0MW_receiveData64MsgInfo\0"
    "VL53LX_DATA_64_MSG*\0rcv_msg_data64"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    1,   75,    2, 0x0a /* Public */,
       5,    1,   78,    2, 0x0a /* Public */,
       7,    0,   81,    2, 0x0a /* Public */,
       8,    0,   82,    2, 0x0a /* Public */,
       9,    1,   83,    2, 0x0a /* Public */,
      10,    1,   86,    2, 0x0a /* Public */,
      11,    0,   89,    2, 0x0a /* Public */,
      12,    0,   90,    2, 0x0a /* Public */,
      13,    1,   91,    2, 0x0a /* Public */,
      16,    1,   94,    2, 0x0a /* Public */,
      19,    1,   97,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->MW_connectBtnHandle(); break;
        case 1: _t->MW_ComboConnIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->MW_CheckConfigHandle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->MW_4x4RadioBtnHandle(); break;
        case 4: _t->MW_8x8RadioBtnHandle(); break;
        case 5: _t->MW_frequencyIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->MW_sharpenerIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->MW_startBtnHandle(); break;
        case 8: _t->MW_stopBtnHandle(); break;
        case 9: _t->MW_receiveInitMsgInfo((*reinterpret_cast< VL53LX_INIT_MSG*(*)>(_a[1]))); break;
        case 10: _t->MW_receiveData16MsgInfo((*reinterpret_cast< VL53LX_DATA_16_MSG*(*)>(_a[1]))); break;
        case 11: _t->MW_receiveData64MsgInfo((*reinterpret_cast< VL53LX_DATA_64_MSG*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
