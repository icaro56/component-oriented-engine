/****************************************************************************
** Meta object code from reading C++ file 'Collider.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/components/physics/Collider.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Collider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Collider_t {
    QByteArrayData data[11];
    char stringdata[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Collider_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Collider_t qt_meta_stringdata_Collider = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 15),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 10),
QT_MOC_LITERAL(4, 37, 9),
QT_MOC_LITERAL(5, 47, 14),
QT_MOC_LITERAL(6, 62, 14),
QT_MOC_LITERAL(7, 77, 14),
QT_MOC_LITERAL(8, 92, 9),
QT_MOC_LITERAL(9, 102, 13),
QT_MOC_LITERAL(10, 116, 13)
    },
    "Collider\0onColisionEnter\0\0Collision&\0"
    "collision\0onColisionStay\0onColisionExit\0"
    "onTriggerEnter\0Collider*\0otherCollider\0"
    "onTriggerExit\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Collider[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x05,
       5,    1,   42,    2, 0x05,
       6,    1,   45,    2, 0x05,
       7,    1,   48,    2, 0x05,
      10,    1,   51,    2, 0x05,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void Collider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Collider *_t = static_cast<Collider *>(_o);
        switch (_id) {
        case 0: _t->onColisionEnter((*reinterpret_cast< Collision(*)>(_a[1]))); break;
        case 1: _t->onColisionStay((*reinterpret_cast< Collision(*)>(_a[1]))); break;
        case 2: _t->onColisionExit((*reinterpret_cast< Collision(*)>(_a[1]))); break;
        case 3: _t->onTriggerEnter((*reinterpret_cast< Collider*(*)>(_a[1]))); break;
        case 4: _t->onTriggerExit((*reinterpret_cast< Collider*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Collider* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Collider* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Collider::*_t)(Collision & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Collider::onColisionEnter)) {
                *result = 0;
            }
        }
        {
            typedef void (Collider::*_t)(Collision & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Collider::onColisionStay)) {
                *result = 1;
            }
        }
        {
            typedef void (Collider::*_t)(Collision & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Collider::onColisionExit)) {
                *result = 2;
            }
        }
        {
            typedef void (Collider::*_t)(Collider * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Collider::onTriggerEnter)) {
                *result = 3;
            }
        }
        {
            typedef void (Collider::*_t)(Collider * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Collider::onTriggerExit)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject Collider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Collider.data,
      qt_meta_data_Collider,  qt_static_metacall, 0, 0}
};


const QMetaObject *Collider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Collider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Collider.stringdata))
        return static_cast<void*>(const_cast< Collider*>(this));
    if (!strcmp(_clname, "Component"))
        return static_cast< Component*>(const_cast< Collider*>(this));
    return QObject::qt_metacast(_clname);
}

int Collider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Collider::onColisionEnter(Collision & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Collider::onColisionStay(Collision & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Collider::onColisionExit(Collision & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Collider::onTriggerEnter(Collider * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Collider::onTriggerExit(Collider * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
