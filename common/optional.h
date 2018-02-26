#ifndef OPTIONAL_H
#define OPTIONAL_H
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
#endif // OPTIONAL_H
