#ifndef COMMON_H
#define COMMON_H

typedef struct {
    unsigned int size;
    const unsigned char *data;
} fingerprint_t;

typedef struct {
    unsigned int size;
    const unsigned char* data;
    fingerprint_t fingerprint;
} rawdivedata_t;


#endif // COMMON_H
