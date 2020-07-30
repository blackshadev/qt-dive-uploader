#ifndef RAWDIVEDATA_H
#define RAWDIVEDATA_H
#include "rawfingerprintdata.h"

struct rawdivedata_t {
    unsigned int size;
    const unsigned char* data;
    rawfingerprint_t fingerprint;
};

#endif // RAWDIVEDATA_H
