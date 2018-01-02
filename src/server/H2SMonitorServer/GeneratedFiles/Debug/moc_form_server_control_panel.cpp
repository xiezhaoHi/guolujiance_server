/****************************************************************************
** Meta object code from reading C++ file 'form_server_control_panel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../server/ui/form_server_control_panel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'form_server_control_panel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CFormServerControlPanel_t {
    QByteArrayData data[6];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CFormServerControlPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CFormServerControlPanel_t qt_meta_stringdata_CFormServerControlPanel = {
    {
QT_MOC_LITERAL(0, 0, 23), // "CFormServerControlPanel"
QT_MOC_LITERAL(1, 24, 19), // "SwitchSerivceStatus"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 12), // "startService"
QT_MOC_LITERAL(4, 58, 19), // "SwitchServiceStatus"
QT_MOC_LITERAL(5, 78, 18) // "UpdateTotalRunTime"

    },
    "CFormServerControlPanel\0SwitchSerivceStatus\0"
    "\0startService\0SwitchServiceStatus\0"
    "UpdateTotalRunTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CFormServerControlPanel[] = {

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
       4,    0,   32,    2, 0x08 /* Private */,
       5,    0,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CFormServerControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CFormServerControlPanel *_t = static_cast<CFormServerControlPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SwitchSerivceStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->SwitchServiceStatus(); break;
        case 2: _t->UpdateTotalRunTime(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CFormServerControlPanel::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CFormServerControlPanel::SwitchSerivceStatus)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CFormServerControlPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CFormServerControlPanel.data,
      qt_meta_data_CFormServerControlPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CFormServerControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CFormServerControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CFormServerControlPanel.stringdata0))
        return static_cast<void*>(const_cast< CFormServerControlPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int CFormServerControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CFormServerControlPanel::SwitchSerivceStatus(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
