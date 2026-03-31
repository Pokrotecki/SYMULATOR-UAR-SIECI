/****************************************************************************
** Meta object code from reading C++ file 'arxwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../arxwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arxwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9ARXwindowE_t {};
} // unnamed namespace

template <> constexpr inline auto ARXwindow::qt_create_metaobjectdata<qt_meta_tag_ZN9ARXwindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ARXwindow",
        "zatwierdzonoARX",
        "",
        "std::vector<double>",
        "a",
        "b",
        "opoznienie",
        "szum",
        "uMin",
        "uMax",
        "yMin",
        "yMax",
        "czyAktywne",
        "on_Add_Button_wektorA_clicked",
        "on_Remove_Button_wektorA_clicked",
        "on_Add_Button_wektorB_clicked",
        "on_Remove_Button_wektorB_clicked",
        "on_Zatwierdz_Button_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'zatwierdzonoARX'
        QtMocHelpers::SignalData<void(const std::vector<double> &, const std::vector<double> &, int, double, double, double, double, double, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 3, 5 }, { QMetaType::Int, 6 }, { QMetaType::Double, 7 },
            { QMetaType::Double, 8 }, { QMetaType::Double, 9 }, { QMetaType::Double, 10 }, { QMetaType::Double, 11 },
            { QMetaType::Bool, 12 },
        }}),
        // Slot 'on_Add_Button_wektorA_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Remove_Button_wektorA_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Add_Button_wektorB_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Remove_Button_wektorB_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_Zatwierdz_Button_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ARXwindow, qt_meta_tag_ZN9ARXwindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ARXwindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ARXwindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ARXwindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9ARXwindowE_t>.metaTypes,
    nullptr
} };

void ARXwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ARXwindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->zatwierdzonoARX((*reinterpret_cast<std::add_pointer_t<std::vector<double>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<std::vector<double>>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[9]))); break;
        case 1: _t->on_Add_Button_wektorA_clicked(); break;
        case 2: _t->on_Remove_Button_wektorA_clicked(); break;
        case 3: _t->on_Add_Button_wektorB_clicked(); break;
        case 4: _t->on_Remove_Button_wektorB_clicked(); break;
        case 5: _t->on_Zatwierdz_Button_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ARXwindow::*)(const std::vector<double> & , const std::vector<double> & , int , double , double , double , double , double , bool )>(_a, &ARXwindow::zatwierdzonoARX, 0))
            return;
    }
}

const QMetaObject *ARXwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ARXwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ARXwindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ARXwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ARXwindow::zatwierdzonoARX(const std::vector<double> & _t1, const std::vector<double> & _t2, int _t3, double _t4, double _t5, double _t6, double _t7, double _t8, bool _t9)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
}
QT_WARNING_POP
