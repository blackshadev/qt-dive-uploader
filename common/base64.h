#ifndef BASE64_H
#define BASE64_H
#include <QString>

typedef unsigned char byte;

QString base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::vector<byte> base64_decode(QString bytes_to_encode);

#endif // BASE64_H
