#include "transporttype.h"

transports_t operator|(TransportType a, TransportType b)
{
    return (transports_t)(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

transports_t operator|(transports_t a, TransportType b)
{
    return (transports_t)(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

transports_t operator|(TransportType a, transports_t b)
{
    return (transports_t)(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
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

transports_t getTransports(dc_transport_t transport)
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
