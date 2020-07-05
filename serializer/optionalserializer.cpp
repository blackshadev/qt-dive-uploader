#include "optionalserializer.h"

template<typename T>
void  OptionalSerializer::serialize(QJsonObject &obj, const char *key, optional_t<T> value)
{
    if (value.has_value) {
        obj[key] = value.value;
    }
}

template<>
void  OptionalSerializer::serialize(QJsonObject &obj, const char *key, optional_t<unsigned int> value)
{
    if (value.has_value) {
        obj[key] = (int)value.value;
    }
}

template void OptionalSerializer::serialize<double>(QJsonObject &obj, const char *key, optional_t<double> value);
template void OptionalSerializer::serialize<int>(QJsonObject &obj, const char *key, optional_t<int> value);

