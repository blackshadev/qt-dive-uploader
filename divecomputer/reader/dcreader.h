#ifndef DIVEREADER_H
#define DIVEREADER_H
#include "../entities/dcdive.h"
#include "../devices/dcdevice.h"
#include "../parsers/dcdiveparser.h"
#include "../common.h"
#include <functional>

typedef std::function<void (DCDive* dive)> dive_callback_t;

class DCReader
{
public:
    DCReader();
    DCReader *setDevice(DCDevice *device);
    DCReader *setParser(DiveParser *parser);
    DCReader *setCallback(dive_callback_t cb);
    void start();
private:
    DCDevice *device;
    DiveParser *parser;
    dive_callback_t callback;
    static int nativeCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
};

#endif // DIVEREADER_H
