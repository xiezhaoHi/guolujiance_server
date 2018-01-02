/****************************************************************************
** Meta object code from reading C++ file 'form_login.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../client/ui/form_login.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form_login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CFormLogin_t {
    QByteArrayData data[13];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CFormLogin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CFormLogin_t qt_meta_stringdata_CFormLogin = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CFormLogin"
QT_MOC_LITERAL(1, 11, 14), // "LoginCompleted"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "loginSucceed"
QT_MOC_LITERAL(4, 40, 12), // "SendUserName"
QT_MOC_LITERAL(5, 53, 5), // "Login"
QT_MOC_LITERAL(6, 59, 11), // "LoginCancel"
QT_MOC_LITERAL(7, 71, 17), // "ServerRepInfoSlot"
QT_MOC_LITERAL(8, 89, 9), // "CMessage*"
QT_MOC_LITERAL(9, 99, 11), // "WaitTimeOut"
QT_MOC_LITERAL(10, 111, 16), // "ServerRepMsgSlot"
QT_MOC_LITERAL(11, 128, 15), // "ResgitLocalHost"
QT_MOC_LITERAL(12, 144, 17) // "DeviceCollectSlot"

    },
    "CFormLogin\0LoginCompleted\0\0loginSucceed\0"
    "SendUserName\0Login\0LoginCancel\0"
    "ServerRepInfoSlot\0CMessage*\0WaitTimeOut\0"
    "ServerRepMsgSlot\0ResgitLocalHost\0"
    "DeviceCollectSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CFormLogin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    2,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    1,   69,    2, 0x08 /* Private */,
       9,    0,   72,    2, 0x08 /* Private */,
      10,    1,   73,    2, 0x08 /* Private */,
      11,    0,   76,    2, 0x08 /* Private */,
      12,    0,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CFormLogin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CFormLogin *_t = static_cast<CFormLogin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LoginCompleted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->SendUserName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->Login(); break;
        case 3: _t->LoginCancel(); break;
        case 4: _t->ServerRepInfoSlot((*reinterpret_cast< CMessage*(*)>(_a[1]))); break;
        case 5: _t->WaitTimeOut(); break;
        case 6: _t->ServerRepMsgSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->ResgitLocalHost(); break;
        case 8: _t->DeviceCollectSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CFormLogin::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CFormLogin::LoginCompleted)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CFormLogin::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CFormLogin::SendUserName)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CFormLogin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CFormLogin.data,
      qt_meta_data_CFormLogin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CFormLogin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CFormLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CFormLogin.stringdata0))
        return static_cast<void*>(const_cast< CFormLogin*>(this));
    return QWidget::qt_metacast(_clname);
}

int CFormLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CFormLogin::LoginCompleted(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CFormLogin::SendUserName(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
