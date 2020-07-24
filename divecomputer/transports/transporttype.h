#ifndef TRANSPORTTYPE_H
#define TRANSPORTTYPE_H
#include <libdivecomputer/descriptor.h>
#include "../context/dccontextinterface.h"
#define TRANSPORTS_NONE 0
#define TRANSPORTS_COUNT 6
#define TRANSPORTS_ALL (1 << TRANSPORTS_COUNT) - 1


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
bool operator&(transports_t a, TransportType b);
const char* transportTypeToString(TransportType t);
transports_t translateTransportsTypes(dc_transport_t transport);
transports_t translateTransportsTypes(unsigned int transport);
const TransportType *getAllTransportTypes();
transports_t getSupportedTransports(DCContextInterface *ctx);

#endif // TRANSPORTTYPE_H
