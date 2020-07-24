#include "transporttype.h"

transports_t operator|(TransportType a, TransportType b)
{
    return (transports_t)(1 << static_cast<unsigned int>(a) |  1 << static_cast<unsigned int>(b));
}

transports_t operator|(transports_t a, TransportType b)
{
    return (transports_t)(static_cast<unsigned int>(a) | 1 << static_cast<unsigned int>(b));
}

transports_t operator|(TransportType a, transports_t b)
{
    return (transports_t)(static_cast<unsigned int>(1 << a) | static_cast<unsigned int>(b));
}

const char *transportTypeToString(TransportType t)
{
    switch (t) {
        case TransportType::BLE:
            return "Bluetooth Low Energy";
        case TransportType::Bluetooth:
            return "Bluetooth";
        case TransportType::USB:
            return "USB";
        case TransportType::USBHID:
            return "USB HID";
        case TransportType::IRDA:
            return "InfraRed";
        case TransportType::Serial:
            return "Serial";
    }
}

transports_t& operator|=(transports_t &a, TransportType b)
{
    return a = a | b;
}

bool operator&(transports_t a, TransportType b)
{
    return static_cast<unsigned int>(a) & 1 << static_cast<unsigned int>(b);
}

transports_t translateTransportsTypes(dc_transport_t transport)
{
    translateTransportsTypes((unsigned int) transport);
}

transports_t translateTransportsTypes(unsigned int transport)
{
    transports_t transports = 0;
    if (transport & DC_TRANSPORT_BLE) {
        transports |= TransportType::BLE;
    }
    if (transport & DC_TRANSPORT_BLUETOOTH) {
        transports |= TransportType::Bluetooth;
    }
    if (transport & DC_TRANSPORT_USB) {
        transports |= TransportType::USB;
    }
    if (transport & DC_TRANSPORT_USBHID) {
        transports |= TransportType::USBHID;
    }
    if (transport & DC_TRANSPORT_IRDA) {
        transports |= TransportType::IRDA;
    }
    if (transport & DC_TRANSPORT_SERIAL) {
        transports |= TransportType::Serial;
    }
    return transports;
}

const TransportType allTypes[TRANSPORTS_COUNT] = {
    TransportType::BLE,
    TransportType::USBHID,
    TransportType::USB,
    TransportType::Serial,
    TransportType::Bluetooth,
    TransportType::IRDA,
};

const TransportType *getAllTransportTypes()
{
    return allTypes;
}

transports_t getSupportedTransports(DCContextInterface *ctx)
{
    return translateTransportsTypes(dc_context_get_transports(ctx->getNative()));
}
