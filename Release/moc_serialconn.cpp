/****************************************************************************
** Meta object code from reading C++ file 'serialconn.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sensor_reader/serialconn.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialconn.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialConn_t {
    QByteArrayData data[8];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialConn_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialConn_t qt_meta_stringdata_SerialConn = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SerialConn"
QT_MOC_LITERAL(1, 11, 18), // "SC_sendInitMsgInfo"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 16), // "VL53LX_INIT_MSG*"
QT_MOC_LITERAL(4, 48, 12), // "rcv_msg_init"
QT_MOC_LITERAL(5, 61, 17), // "SC_readDataHandle"
QT_MOC_LITERAL(6, 79, 16), // "SC_establishConn"
QT_MOC_LITERAL(7, 96, 14) // "SC_dismissConn"

    },
    "SerialConn\0SC_sendInitMsgInfo\0\0"
    "VL53LX_INIT_MSG*\0rcv_msg_init\0"
    "SC_readDataHandle\0SC_establishConn\0"
    "SC_dismissConn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialConn[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x0a /* Public */,
       6,    1,   38,    2, 0x0a /* Public */,
       7,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void SerialConn::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialConn *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SC_sendInitMsgInfo((*reinterpret_cast< VL53LX_INIT_MSG*(*)>(_a[1]))); break;
        case 1: _t->SC_readDataHandle(); break;
        case 2: _t->SC_establishConn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->SC_dismissConn(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialConn::*)(VL53LX_INIT_MSG * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialConn::SC_sendInitMsgInfo)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialConn::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_SerialConn.data,
    qt_meta_data_SerialConn,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialConn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialConn::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialConn.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SerialConn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SerialConn::SC_sendInitMsgInfo(VL53LX_INIT_MSG * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
