#ifndef DCCONTEXT_H
#define DCCONTEXT_H
#include "../descriptor/dcdevicedescriptor.h"
#include "../transports/dctransportinterface.h"
#include "../devices/dcdeviceinterface.h"
#include <libdivecomputer/context.h>
#include <vector>
#include <functional>
#include <string>
#include "dccontextinterface.h"

typedef dc_loglevel_t loglevel_t;
typedef std::function<void (logdata_t)> logfunc_t;

class DCContext : public DCContextInterface
{
public:
    DCContext();
    virtual ~DCContext();
    void setLogFunction(logfunc_t logFunc);
    void setLogLevel(loglevel_t logLevel);
    dc_context_t *getNative() override;
    virtual void log(logdata_t logdata) override;
    virtual DCContext *clone() override;
    std::vector<DCDeviceDescriptor *> *getDescriptors();
    std::vector<DCTransportInterface *> *getTransports();
protected:
    loglevel_t logLevel = DC_LOGLEVEL_ERROR;
    logfunc_t logfunc;
    dc_context_t *context;
    static void logfunction(dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata);
    virtual DCDeviceDescriptor *createDescriptor(dc_descriptor_t *descr);
    std::vector<DCDeviceDescriptor *> *descriptors;
    std::vector<DCTransportInterface *> *transports;
    std::vector<DCDeviceDescriptor *> *loadDescriptors();
    std::vector<DCTransportInterface *> *loadTransports();
};

#endif // DCCONTEXT_H
