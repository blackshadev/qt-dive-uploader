#include "fingerprintdata.h"
#include <cstring>

fingerprint_t::fingerprint_t()
{
    data = NULL;
    size = 0;
}

fingerprint_t::~fingerprint_t()
{
    if (data) {
        delete[] data;
        data = NULL;
    }
    size = 0;
}

void fingerprint_t::operator=(const fingerprint_t &fp)
{
    size = fp.size;
    if (data) {
        delete[] data;
        data = NULL;
    }

    data = new unsigned char[size];
    std::memcpy(data, fp.data, size);
}

void fingerprint_t::operator=(const rawfingerprint_t &fp)
{
    size = fp.size;
    if (data) {
        delete[] data;
        data = NULL;
    }

    data = new unsigned char[size];
    std::memcpy(data, fp.data, size);
}
