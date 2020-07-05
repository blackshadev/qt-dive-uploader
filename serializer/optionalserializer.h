#ifndef IOptionalSERIALIZER_H
#define IOptionalSERIALIZER_H
#include <QJsonObject>
#include <QJsonArray>
#include "../common/optional.h"

class OptionalSerializer
{
public:
    template<typename T>
    void serialize(QJsonObject &obj, const char *key, optional_t<T> value);
};

#endif // IOptionalSERIALIZER_H
