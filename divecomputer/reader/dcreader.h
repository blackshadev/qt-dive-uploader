#ifndef DIVEREADER_H
#define DIVEREADER_H
#include "../entities/dcdive.h"
#include "../devices/dcdevice.h"
#include "../parsers/dcdiveparser.h"
#include "../common.h"
#include <functional>


//typedef std::function<void (DCDive* dive)> dive_callback_t;
//typedef std::function<void (dc_event_progress_t progress)> progress_callback_t;
//typedef std::function<void (dc_event_clock_t clock)> clock_callback_t;
//typedef std::function<void ()> waiting_callback_t;
//typedef std::function<void (dc_event_devinfo_t info)> devinfo_callback_t;

class DCReader
{
public:
    DCReader();
    DCReader *setDevice(DCDevice *device);
    DCReader *setParser(DiveParser *parser);
//    DCReader *setOnDiveCallback(dive_callback_t cb);
//    DCReader *setOnProgressCallback(progress_callback_t cb);
    bool isReady();
    bool isReading();
    void start();
    void cancel();
    bool isCancelled();

protected:
    virtual void receiveDeviceInfoEvent(dc_event_devinfo_t *devInfo) = 0;
    virtual void receiveProgressEvent(dc_event_progress_t *progress) = 0;
    virtual void receiveWaitingEvent() = 0;
    virtual void receiveClockEvent(dc_event_clock_t *clock) = 0;
    virtual void receiveDive(DCDive *dive) = 0;

private:
    DCDevice *device;
    DiveParser *parser;
    bool cancelled = false;
    bool reading = false;

    static int nativeDiveCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
    static void nativeEventCallback(dc_device_t *device, dc_event_type_t event, const void *data, void *userdata);
    static int nativeCancelCallback(void *userdata);

};

#endif // DIVEREADER_H
