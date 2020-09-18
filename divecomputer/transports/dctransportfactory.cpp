#include "dctransportfactory.h"
#include "dcusbhidtransport.h"
#include "dcirdatransport.h"
#include "dcserialtransport.h"

DCTransportInterface *DCTransportFactory::create(TransportType type)
{
    switch (type) {
        case TransportType::USBHID:
            return  (DCTransportInterface *)(new DCUSBHIDTransport(context));
        case TransportType::IRDA:
            return  (DCTransportInterface *)(new DCIRDATransport(context));
        case TransportType::Serial:
            return  (DCTransportInterface *)(new DCSerialTransport(context));
        default:
            return NULL;
    }

}

DCTransportFactory *DCTransportFactory::setContext(DCContextInterface *ctx)
{
    context = ctx;
    return this;
}
