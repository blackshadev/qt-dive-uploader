#include "dctransportfactory.h"
#include "dcusbhidtransport.h"

DCTransportInterface *DCTransportFactory::create(TransportType type)
{
    switch (type) {
        case TransportType::USBHID:
            return  (DCTransportInterface *)(new DCUSBHIDTransport(context));
        case TransportType::IRDA:
            return  (DCTransportInterface *)(new DCUSBHIDTransport(context));
        default:
            return NULL;
    }

}

DCTransportFactory *DCTransportFactory::setContext(DCContextInterface *ctx)
{
    context = ctx;
    return this;
}
