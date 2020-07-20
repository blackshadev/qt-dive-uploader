#ifndef DIVEREADER_H
#define DIVEREADER_H
#include "../entities/dcdive.h"
#include "../devices/dcdevice.h"
#include "../parsers/dcdiveparser.h"
#include "../common.h"
#include <functional>
typedef struct {

} progress_t;


typedef std::function<void (DCDive* dive)> dive_callback_t;
typedef std::function<void (progress_t progress)> progress_callback_t;

class DCReader
{
public:
    DCReader();
    DCReader *setDevice(DCDevice *device);
    DCReader *setParser(DiveParser *parser);
    DCReader *setOnDiveCallback(dive_callback_t cb);
    DCReader *setOnProgressCallback(progress_callback_t cb);
    bool isReady();
    void start();
    void cancel();
private:
    DCDevice *device;
    DiveParser *parser;
    dive_callback_t onDiveCallback;
    progress_callback_t onProgressCallback;
    static int nativeDiveCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
    static void nativeEventCallback(dc_device_t *device, dc_event_type_t event, const void *data, void *userdata);
};

#endif // DIVEREADER_H
