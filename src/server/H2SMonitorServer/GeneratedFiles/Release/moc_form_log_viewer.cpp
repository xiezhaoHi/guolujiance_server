/****************************************************************************
** Meta object code from reading C++ file 'form_log_viewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../server/ui/form_log_viewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form_log_viewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CFormLogViewer_t {
    QByteArrayData data[6];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CFormLogViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CFormLogViewer_t qt_meta_stringdata_CFormLogViewer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CFormLogViewer"
QT_MOC_LITERAL(1, 15, 9), // "LogSignal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "log"
QT_MOC_LITERAL(4, 30, 7), // "LogSlot"
QT_MOC_LITERAL(5, 38, 14) // "UpdateLogLevel"

    },
    "CFormLogViewer\0LogSignal\0\0log\0LogSlot\0"
    "UpdateLogLevel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CFormLogViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x0a /* Public */,
       5,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void CFormLogViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CFormLogViewer *_t = static_cast<CFormLogViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LogSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->LogSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->UpdateLogLevel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CFormLogViewer::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CFormLogViewer::LogSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CFormLogViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CFormLogViewer.data,
      qt_meta_data_CFormLogViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CFormLogViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CFormLogViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CFormLogViewer.stringdata0))
        return static_cast<void*>(const_cast< CFormLogViewer*>(this));
    if (!strcmp(_clname, "ILogObserver"))
        return static_cast< ILogObserver*>(const_cast< CFormLogViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int CFormLogViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CFormLogViewer::LogSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
