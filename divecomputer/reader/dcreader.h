#ifndef DIVEREADER_H
#define DIVEREADER_H
#include "../entities/dcdive.h"
#include "../devices/dcdeviceinterface.h"
#include "../parsers/dcdiveparserinterface.h"
#include "../common.h"
#include "dcreaderinterface.h"
#include <functional>

class DCReader : DCReaderInterface
{
public:
    DCReader();
    virtual DCReader *setDevice(DCDeviceInterface *device) override;
    virtual DCReader *setParser(DCDiveParserInterface *parser) override;

    virtual void start() override;
    virtual void cancel();
    virtual bool isReady();
    virtual void setFingerprint(fingerprint_t fp);
    void readAll();
    bool isCancelled();

protected:
    virtual void error(const char *msg);
    virtual void receiveDeviceInfoEvent(dc_event_devinfo_t *devInfo) = 0;
    virtual void receiveProgressEvent(dc_event_progress_t *progress) = 0;
    virtual void receiveWaitingEvent() = 0;
    virtual void receiveClockEvent(dc_event_clock_t *clock) = 0;
    virtual void receiveDive(DCDive *dive) = 0;

private:
    DCDeviceInterface *device;
    DCDiveParserInterface *parser;
    bool cancelled = false;

    static int nativeDiveCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
    static void nativeEventCallback(dc_device_t *device, dc_event_type_t event, const void *data, void *userdata);
    static int nativeCancelCallback(void *userdata);

};

#endif // DIVEREADER_H
