#include "fingerprintdata.h"
#include <cstring>

void deleteFingerprintData(fingerprint_t *fp)
{
    if (fp->data) {
        delete[] fp->data;
        fp->data = NULL;
    }
    fp->size = 0;
}

void copyFingerprintData(fingerprint_t *to, const fingerprint_t *from)
{
    to->size = from->size;
    to->data = new unsigned char[from->size];
    std::memcpy(to->data, from->data, from->size);
}

void copyFingerprintData(fingerprint_t *to, const rawfingerprint_t *from)
{
    to->size = from->size;
    to->data = new unsigned char[from->size];
    std::memcpy(to->data, from->data, from->size);
}


fingerprint_t::fingerprint_t()
{
    data = NULL;
    size = 0;
}

fingerprint_t::fingerprint_t(const fingerprint_t &fp)
    : fingerprint_t()
{
    copyFingerprintData(this, &fp);
}

fingerprint_t::fingerprint_t(unsigned char *d, unsigned int s)
    : fingerprint_t()
{
    size = s;
    data = new unsigned char[size];
    std::memcpy(data, d, size);
}

fingerprint_t::~fingerprint_t()
{
    deleteFingerprintData(this);
}

void fingerprint_t::operator=(const fingerprint_t &fp)
{
    deleteFingerprintData(this);

    copyFingerprintData(this, &fp);
}

void fingerprint_t::operator=(const rawfingerprint_t &fp)
{
    deleteFingerprintData(this);

    copyFingerprintData(this, &fp);
}
