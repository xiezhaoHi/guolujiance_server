/****************************************************************************
** Meta object code from reading C++ file 'qmessagethread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../client/connection/qmessagethread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qmessagethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QMessageThread_t {
    QByteArrayData data[9];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QMessageThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QMessageThread_t qt_meta_stringdata_QMessageThread = {
    {
QT_MOC_LITERAL(0, 0, 14), // "QMessageThread"
QT_MOC_LITERAL(1, 15, 12), // "ServerRepMsg"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "ServerDisconnect"
QT_MOC_LITERAL(4, 46, 14), // "ServierConnect"
QT_MOC_LITERAL(5, 61, 24), // "ServerRepDeviceDataCount"
QT_MOC_LITERAL(6, 86, 9), // "CMessage*"
QT_MOC_LITERAL(7, 96, 11), // "ConnectSlot"
QT_MOC_LITERAL(8, 108, 15) // "ReconnectServer"

    },
    "QMessageThread\0ServerRepMsg\0\0"
    "ServerDisconnect\0ServierConnect\0"
    "ServerRepDeviceDataCount\0CMessage*\0"
    "ConnectSlot\0ReconnectServer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QMessageThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    0,   47,    2, 0x06 /* Public */,
       4,    0,   48,    2, 0x06 /* Public */,
       5,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   52,    2, 0x0a /* Public */,
       8,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QMessageThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QMessageThread *_t = static_cast<QMessageThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ServerRepMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ServerDisconnect(); break;
        case 2: _t->ServierConnect(); break;
        case 3: _t->ServerRepDeviceDataCount((*reinterpret_cast< CMessage*(*)>(_a[1]))); break;
        case 4: _t->ConnectSlot(); break;
        case 5: _t->ReconnectServer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QMessageThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMessageThread::ServerRepMsg)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QMessageThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMessageThread::ServerDisconnect)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QMessageThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMessageThread::ServierConnect)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QMessageThread::*_t)(CMessage * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMessageThread::ServerRepDeviceDataCount)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject QMessageThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QMessageThread.data,
      qt_meta_data_QMessageThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QMessageThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QMessageThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QMessageThread.stringdata0))
        return static_cast<void*>(const_cast< QMessageThread*>(this));
    return QThread::qt_metacast(_clname);
}

int QMessageThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QMessageThread::ServerRepMsg(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QMessageThread::ServerDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QMessageThread::ServierConnect()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void QMessageThread::ServerRepDeviceDataCount(CMessage * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
