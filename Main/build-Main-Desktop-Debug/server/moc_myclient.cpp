/****************************************************************************
** Meta object code from reading C++ file 'myclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../server/myclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyClient_t {
    QByteArrayData data[18];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyClient_t qt_meta_stringdata_MyClient = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyClient"
QT_MOC_LITERAL(1, 9, 12), // "addUserToGui"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "name"
QT_MOC_LITERAL(4, 28, 17), // "removeUserFromGui"
QT_MOC_LITERAL(5, 46, 10), // "removeUser"
QT_MOC_LITERAL(6, 57, 9), // "MyClient*"
QT_MOC_LITERAL(7, 67, 6), // "client"
QT_MOC_LITERAL(8, 74, 12), // "messageToGui"
QT_MOC_LITERAL(9, 87, 7), // "message"
QT_MOC_LITERAL(10, 95, 4), // "from"
QT_MOC_LITERAL(11, 100, 5), // "users"
QT_MOC_LITERAL(12, 106, 9), // "onConnect"
QT_MOC_LITERAL(13, 116, 12), // "onDisconnect"
QT_MOC_LITERAL(14, 129, 11), // "onReadyRead"
QT_MOC_LITERAL(15, 141, 7), // "onError"
QT_MOC_LITERAL(16, 149, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(17, 178, 11) // "socketError"

    },
    "MyClient\0addUserToGui\0\0name\0"
    "removeUserFromGui\0removeUser\0MyClient*\0"
    "client\0messageToGui\0message\0from\0users\0"
    "onConnect\0onDisconnect\0onReadyRead\0"
    "onError\0QAbstractSocket::SocketError\0"
    "socketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,
       8,    3,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   70,    2, 0x08 /* Private */,
      13,    0,   71,    2, 0x08 /* Private */,
      14,    0,   72,    2, 0x08 /* Private */,
      15,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QStringList,    9,   10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void MyClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addUserToGui((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->removeUserFromGui((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->removeUser((*reinterpret_cast< MyClient*(*)>(_a[1]))); break;
        case 3: _t->messageToGui((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 4: _t->onConnect(); break;
        case 5: _t->onDisconnect(); break;
        case 6: _t->onReadyRead(); break;
        case 7: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MyClient* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyClient::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyClient::addUserToGui)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MyClient::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyClient::removeUserFromGui)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MyClient::*)(MyClient * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyClient::removeUser)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MyClient::*)(QString , QString , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyClient::messageToGui)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MyClient::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MyClient.data,
    qt_meta_data_MyClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MyClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MyClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MyClient::addUserToGui(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyClient::removeUserFromGui(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyClient::removeUser(MyClient * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyClient::messageToGui(QString _t1, QString _t2, const QStringList & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE