#ifndef DCCONTEXT_H
#define DCCONTEXT_H
#include "../descriptor/dcdevicedescriptor.h"
#include "../transports/dctransportinterface.h"
#include <libdivecomputer/context.h>
#include <vector>
#include <functional>
#include <string>

typedef dc_loglevel_t loglevel_t;
typedef struct {
    dc_loglevel_t loglevel;
    const char *file;
    unsigned int line;
    const char *function;
    const char *msg;
} logdata_t;
typedef std::function<void (logdata_t)> logfunc_t;

class DCContext
{
public:
    DCContext();
    ~DCContext();
    void setLogFunction(logfunc_t logFunc);
    void setLogLevel(loglevel_t logLevel);
    dc_context_t *getNative();
    std::vector<DCDeviceDescriptor *> *getDescriptors();
    std::vector<DCTransportInterface *> *getTransport(TransportType flag);
private:
    loglevel_t logLevel = DC_LOGLEVEL_ERROR;
    logfunc_t log;
    dc_context_t *context;
    static void logfunction(dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata);
    std::vector<DCDeviceDescriptor *> *descriptors;
    std::vector<DCTransportInterface *> *transports;
    std::vector<DCDeviceDescriptor *> *loadDescriptors();
    std::vector<DCTransportInterface *> *loadTransports();
};

#endif // DCCONTEXT_H
