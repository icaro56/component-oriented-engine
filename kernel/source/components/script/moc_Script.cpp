/****************************************************************************
** Meta object code from reading C++ file 'Script.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/components/script/Script.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Script.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Script_t {
    QByteArrayData data[16];
    char stringdata[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Script_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Script_t qt_meta_stringdata_Script = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 15),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 10),
QT_MOC_LITERAL(4, 35, 9),
QT_MOC_LITERAL(5, 45, 14),
QT_MOC_LITERAL(6, 60, 14),
QT_MOC_LITERAL(7, 75, 14),
QT_MOC_LITERAL(8, 90, 9),
QT_MOC_LITERAL(9, 100, 13),
QT_MOC_LITERAL(10, 114, 13),
QT_MOC_LITERAL(11, 128, 23),
QT_MOC_LITERAL(12, 152, 22),
QT_MOC_LITERAL(13, 175, 3),
QT_MOC_LITERAL(14, 179, 25),
QT_MOC_LITERAL(15, 205, 24)
    },
    "Script\0onColisionEnter\0\0Collision&\0"
    "collision\0onColisionStay\0onColisionExit\0"
    "onTriggerEnter\0Collider*\0otherCollider\0"
    "onTriggerExit\0onControllerColliderHit\0"
    "ControllerColliderHit&\0hit\0"
    "onControllerControllerHit\0"
    "ControllerControllerHit&\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Script[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a,
       5,    1,   52,    2, 0x0a,
       6,    1,   55,    2, 0x0a,
       7,    1,   58,    2, 0x0a,
      10,    1,   61,    2, 0x0a,
      11,    1,   64,    2, 0x0a,
      14,    1,   67,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   13,

       0        // eod
};

void Script::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Script *_t = static_cast<Script *>(_o);
        switch (_id) {
        case 0: _t->onColisionEnter((*reinterpret_cast< Collision(*)>(_a[1]))); break;
        case 1: _t->onColisionStay((*reinterpret_cast< Collision(*)>(_a[1]))); break;
        case 2: _t->onColisionExit((*reinterpret_cast< Collision(*)>(_a[1]))); break;
        case 3: _t->onTriggerEnter((*reinterpret_cast< Collider*(*)>(_a[1]))); break;
        case 4: _t->onTriggerExit((*reinterpret_cast< Collider*(*)>(_a[1]))); break;
        case 5: _t->onControllerColliderHit((*reinterpret_cast< ControllerColliderHit(*)>(_a[1]))); break;
        case 6: _t->onControllerControllerHit((*reinterpret_cast< ControllerControllerHit(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Script::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Script.data,
      qt_meta_data_Script,  qt_static_metacall, 0, 0}
};


const QMetaObject *Script::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Script::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Script.stringdata))
        return static_cast<void*>(const_cast< Script*>(this));
    if (!strcmp(_clname, "Component"))
        return static_cast< Component*>(const_cast< Script*>(this));
    return QObject::qt_metacast(_clname);
}

int Script::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
