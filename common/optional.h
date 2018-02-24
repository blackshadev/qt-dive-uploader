#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <QJsonValue>

template <typename T>
struct optional_t {
    bool has_value = false;
    T   value;

    operator QJsonValue() {
        return has_value ? QJsonValue() :  QJsonValue(value);
    };
};

#endif // OPTIONAL_H
