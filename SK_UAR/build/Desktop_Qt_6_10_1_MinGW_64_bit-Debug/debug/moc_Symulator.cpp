/****************************************************************************
** Meta object code from reading C++ file 'Symulator.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Symulator.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Symulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12SymulatorUARE_t {};
} // unnamed namespace

template <> constexpr inline auto SymulatorUAR::qt_create_metaobjectdata<qt_meta_tag_ZN12SymulatorUARE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SymulatorUAR",
        "krokWykonany",
        "",
        "w",
        "y",
        "e",
        "u",
        "k",
        "P",
        "I",
        "D",
        "Tick"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'krokWykonany'
        QtMocHelpers::SignalData<void(double, double, double, double, int, double, double, double)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 }, { QMetaType::Double, 4 }, { QMetaType::Double, 5 }, { QMetaType::Double, 6 },
            { QMetaType::Int, 7 }, { QMetaType::Double, 8 }, { QMetaType::Double, 9 }, { QMetaType::Double, 10 },
        }}),
        // Slot 'Tick'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SymulatorUAR, qt_meta_tag_ZN12SymulatorUARE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SymulatorUAR::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SymulatorUARE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SymulatorUARE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12SymulatorUARE_t>.metaTypes,
    nullptr
} };

void SymulatorUAR::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SymulatorUAR *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->krokWykonany((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8]))); break;
        case 1: _t->Tick(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SymulatorUAR::*)(double , double , double , double , int , double , double , double )>(_a, &SymulatorUAR::krokWykonany, 0))
            return;
    }
}

const QMetaObject *SymulatorUAR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SymulatorUAR::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SymulatorUARE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SymulatorUAR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SymulatorUAR::krokWykonany(double _t1, double _t2, double _t3, double _t4, int _t5, double _t6, double _t7, double _t8)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
}
QT_WARNING_POP
