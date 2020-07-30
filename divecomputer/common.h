#ifndef COMMON_H
#define COMMON_H

struct rawfingerprint_t {
    unsigned int size;
    const unsigned char *data;
};

struct fingerprint_t {
    unsigned int size;
    unsigned char *data;
public:
    fingerprint_t();
    ~fingerprint_t();
    void operator=(const fingerprint_t &fp );
    void operator=(const rawfingerprint_t &fp );
};

struct rawdivedata_t {
    unsigned int size;
    const unsigned char* data;
    rawfingerprint_t fingerprint;
};


#endif // COMMON_H
