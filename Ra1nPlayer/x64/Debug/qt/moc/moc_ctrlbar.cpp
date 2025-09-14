/****************************************************************************
** Meta object code from reading C++ file 'ctrlbar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../ctrlbar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ctrlbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CtrlBar_t {
    QByteArrayData data[16];
    char stringdata0[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CtrlBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CtrlBar_t qt_meta_stringdata_CtrlBar = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CtrlBar"
QT_MOC_LITERAL(1, 8, 11), // "EndBtnClick"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "FullBtnClick"
QT_MOC_LITERAL(4, 34, 21), // "Play_Or_PauseBtnClick"
QT_MOC_LITERAL(5, 56, 13), // "VoiceBtnClick"
QT_MOC_LITERAL(6, 70, 14), // "SigPlayOrPause"
QT_MOC_LITERAL(7, 85, 19), // "FastForwardBtnClick"
QT_MOC_LITERAL(8, 105, 16), // "BackOffdBtnClick"
QT_MOC_LITERAL(9, 122, 9), // "Play_Seek"
QT_MOC_LITERAL(10, 132, 8), // "position"
QT_MOC_LITERAL(11, 141, 13), // "Volume_Change"
QT_MOC_LITERAL(12, 155, 7), // "positon"
QT_MOC_LITERAL(13, 163, 12), // "Speed_Change"
QT_MOC_LITERAL(14, 176, 3), // "str"
QT_MOC_LITERAL(15, 180, 12) // "Use_HwDecode"

    },
    "CtrlBar\0EndBtnClick\0\0FullBtnClick\0"
    "Play_Or_PauseBtnClick\0VoiceBtnClick\0"
    "SigPlayOrPause\0FastForwardBtnClick\0"
    "BackOffdBtnClick\0Play_Seek\0position\0"
    "Volume_Change\0positon\0Speed_Change\0"
    "str\0Use_HwDecode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CtrlBar[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,
       6,    0,   73,    2, 0x06 /* Public */,
       7,    0,   74,    2, 0x06 /* Public */,
       8,    0,   75,    2, 0x06 /* Public */,
       9,    1,   76,    2, 0x06 /* Public */,
      11,    1,   79,    2, 0x06 /* Public */,
      13,    1,   82,    2, 0x06 /* Public */,
      15,    1,   85,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Float,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,

       0        // eod
};

void CtrlBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CtrlBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->EndBtnClick(); break;
        case 1: _t->FullBtnClick(); break;
        case 2: _t->Play_Or_PauseBtnClick(); break;
        case 3: _t->VoiceBtnClick(); break;
        case 4: _t->SigPlayOrPause(); break;
        case 5: _t->FastForwardBtnClick(); break;
        case 6: _t->BackOffdBtnClick(); break;
        case 7: _t->Play_Seek((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->Volume_Change((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->Speed_Change((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->Use_HwDecode((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::EndBtnClick)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::FullBtnClick)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::Play_Or_PauseBtnClick)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::VoiceBtnClick)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::SigPlayOrPause)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::FastForwardBtnClick)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::BackOffdBtnClick)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::Play_Seek)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::Volume_Change)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)(const QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::Speed_Change)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (CtrlBar::*)(const QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlBar::Use_HwDecode)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CtrlBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CtrlBar.data,
    qt_meta_data_CtrlBar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CtrlBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CtrlBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CtrlBar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CtrlBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CtrlBar::EndBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CtrlBar::FullBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CtrlBar::Play_Or_PauseBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CtrlBar::VoiceBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CtrlBar::SigPlayOrPause()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CtrlBar::FastForwardBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CtrlBar::BackOffdBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CtrlBar::Play_Seek(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CtrlBar::Volume_Change(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CtrlBar::Speed_Change(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CtrlBar::Use_HwDecode(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
