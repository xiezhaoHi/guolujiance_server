/****************************************************************************
** Meta object code from reading C++ file 'qexportwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qexportwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qexportwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QExportWidget_t {
    QByteArrayData data[16];
    char stringdata0[242];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QExportWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QExportWidget_t qt_meta_stringdata_QExportWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QExportWidget"
QT_MOC_LITERAL(1, 14, 11), // "BeginExport"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "ExportCompelete"
QT_MOC_LITERAL(4, 43, 1), // "i"
QT_MOC_LITERAL(5, 45, 13), // "BeginLoadData"
QT_MOC_LITERAL(6, 59, 14), // "ExportDataSlot"
QT_MOC_LITERAL(7, 74, 16), // "GetQueryDataInfo"
QT_MOC_LITERAL(8, 91, 19), // "ExportTestImageSlot"
QT_MOC_LITERAL(9, 111, 18), // "ExportAveImageSlot"
QT_MOC_LITERAL(10, 130, 22), // "AveCheckBoxStateChange"
QT_MOC_LITERAL(11, 153, 18), // "LoadServerDataSlot"
QT_MOC_LITERAL(12, 172, 18), // "ServerRepDataCount"
QT_MOC_LITERAL(13, 191, 9), // "CMessage*"
QT_MOC_LITERAL(14, 201, 18), // "RecvServerLoadData"
QT_MOC_LITERAL(15, 220, 21) // "SaveLoadDataCountSlot"

    },
    "QExportWidget\0BeginExport\0\0ExportCompelete\0"
    "i\0BeginLoadData\0ExportDataSlot\0"
    "GetQueryDataInfo\0ExportTestImageSlot\0"
    "ExportAveImageSlot\0AveCheckBoxStateChange\0"
    "LoadServerDataSlot\0ServerRepDataCount\0"
    "CMessage*\0RecvServerLoadData\0"
    "SaveLoadDataCountSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QExportWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       3,    1,   77,    2, 0x06 /* Public */,
       5,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    1,   87,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    1,   91,    2, 0x08 /* Private */,
      14,    1,   94,    2, 0x08 /* Private */,
      15,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void QExportWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QExportWidget *_t = static_cast<QExportWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->BeginExport((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ExportCompelete((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->BeginLoadData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->ExportDataSlot(); break;
        case 4: _t->GetQueryDataInfo(); break;
        case 5: _t->ExportTestImageSlot(); break;
        case 6: _t->ExportAveImageSlot(); break;
        case 7: _t->AveCheckBoxStateChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->LoadServerDataSlot(); break;
        case 9: _t->ServerRepDataCount((*reinterpret_cast< CMessage*(*)>(_a[1]))); break;
        case 10: _t->RecvServerLoadData((*reinterpret_cast< CMessage*(*)>(_a[1]))); break;
        case 11: _t->SaveLoadDataCountSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QExportWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QExportWidget::BeginExport)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QExportWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QExportWidget::ExportCompelete)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QExportWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QExportWidget::BeginLoadData)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QExportWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QExportWidget.data,
      qt_meta_data_QExportWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QExportWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QExportWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QExportWidget.stringdata0))
        return static_cast<void*>(const_cast< QExportWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QExportWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void QExportWidget::BeginExport(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QExportWidget::ExportCompelete(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QExportWidget::BeginLoadData(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
