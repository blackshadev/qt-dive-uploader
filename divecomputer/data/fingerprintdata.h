#ifndef FINGERPRINTDATA_H
#define FINGERPRINTDATA_H
#include "rawfingerprintdata.h"

struct fingerprint_t {
    unsigned int size;
    unsigned char *data;
public:
    fingerprint_t();
    ~fingerprint_t();
    void operator=(const fingerprint_t &fp );
    void operator=(const rawfingerprint_t &fp );
};

#endif // FINGERPRINTDATA_H