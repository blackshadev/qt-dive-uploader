#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <QJsonObject>
#include <QJsonValue>

template <typename T>
struct optional_t {
    bool has_value = false;
    T   value;

    operator QJsonValue()
    {
        return !has_value ? QJsonValue() :  QJsonValue(value);
    }

    optional_t<T>& operator =(T val)
    {
        has_value = true;
        value = val;
        return *this;
    }

};

template <typename T>
void optional_write(QJsonObject* obj, const char* fld, optional_t<T> opt) {
    if(opt.has_value) {
        (*obj)[fld] = opt.value;
    }
}

#endif // OPTIONAL_H
