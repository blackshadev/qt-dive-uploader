#ifndef TRANSPORTTYPE_H
#define TRANSPORTTYPE_H
#include <libdivecomputer/descriptor.h>

typedef unsigned int transports_t;

enum TransportType {
    USB = 0,
    USBHID = 1,
    Serial = 2,
    Bluetooth = 3,
    IRDA = 4,
    BLE  = 5
};

transports_t operator|(TransportType a, TransportType b);
transports_t operator|(transports_t a, TransportType b);
transports_t operator|(TransportType a, transports_t b);
const char* transportTypeToString(TransportType t);
transports_t getTransports(dc_transport_t transport);


#endif // TRANSPORTTYPE_H
