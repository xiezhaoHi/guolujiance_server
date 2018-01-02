/****************************************************************************
** Meta object code from reading C++ file 'qdevice_collect_dlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qdevice_collect_dlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdevice_collect_dlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QDevice_Collect_dlg_t {
    QByteArrayData data[7];
    char stringdata0[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QDevice_Collect_dlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QDevice_Collect_dlg_t qt_meta_stringdata_QDevice_Collect_dlg = {
    {
QT_MOC_LITERAL(0, 0, 19), // "QDevice_Collect_dlg"
QT_MOC_LITERAL(1, 20, 13), // "H2StoZeroSlot"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "H2StoFullSlot"
QT_MOC_LITERAL(4, 49, 13), // "SO2toZeroSlot"
QT_MOC_LITERAL(5, 63, 13), // "SO2toFullSlot"
QT_MOC_LITERAL(6, 77, 12) // "ReadCommInfo"

    },
    "QDevice_Collect_dlg\0H2StoZeroSlot\0\0"
    "H2StoFullSlot\0SO2toZeroSlot\0SO2toFullSlot\0"
    "ReadCommInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QDevice_Collect_dlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QDevice_Collect_dlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDevice_Collect_dlg *_t = static_cast<QDevice_Collect_dlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->H2StoZeroSlot(); break;
        case 1: _t->H2StoFullSlot(); break;
        case 2: _t->SO2toZeroSlot(); break;
        case 3: _t->SO2toFullSlot(); break;
        case 4: _t->ReadCommInfo(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QDevice_Collect_dlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QDevice_Collect_dlg.data,
      qt_meta_data_QDevice_Collect_dlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QDevice_Collect_dlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QDevice_Collect_dlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QDevice_Collect_dlg.stringdata0))
        return static_cast<void*>(const_cast< QDevice_Collect_dlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QDevice_Collect_dlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
