/****************************************************************************
** Meta object code from reading C++ file 'qdatashowwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qdatashowwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdatashowwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QDataShowWidget_t {
    QByteArrayData data[8];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QDataShowWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QDataShowWidget_t qt_meta_stringdata_QDataShowWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QDataShowWidget"
QT_MOC_LITERAL(1, 16, 7), // "clicked"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "BeginQueryData"
QT_MOC_LITERAL(4, 40, 13), // "QueryDataOver"
QT_MOC_LITERAL(5, 54, 16), // "ChangeDeviceSlot"
QT_MOC_LITERAL(6, 71, 16), // "AveCalculateSlot"
QT_MOC_LITERAL(7, 88, 14) // "GetAveDataSlot"

    },
    "QDataShowWidget\0clicked\0\0BeginQueryData\0"
    "QueryDataOver\0ChangeDeviceSlot\0"
    "AveCalculateSlot\0GetAveDataSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QDataShowWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QDataShowWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDataShowWidget *_t = static_cast<QDataShowWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->BeginQueryData(); break;
        case 2: _t->QueryDataOver(); break;
        case 3: _t->ChangeDeviceSlot(); break;
        case 4: _t->AveCalculateSlot(); break;
        case 5: _t->GetAveDataSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QDataShowWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDataShowWidget::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QDataShowWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDataShowWidget::BeginQueryData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QDataShowWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDataShowWidget::QueryDataOver)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QDataShowWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QDataShowWidget.data,
      qt_meta_data_QDataShowWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QDataShowWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QDataShowWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QDataShowWidget.stringdata0))
        return static_cast<void*>(const_cast< QDataShowWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QDataShowWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void QDataShowWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QDataShowWidget::BeginQueryData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QDataShowWidget::QueryDataOver()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
