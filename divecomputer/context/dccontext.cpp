#include <stdexcept>
#include <exception>
#include <vector>
#include "dccontext.h"
#include "../../common/vector.h"
#include "../utils/dcstatus.h"
#include "../transports/dcusbhidtransport.h"

DCContext::DCContext()
{
    dc_context_new(&context);
    logfunc = NULL;
    dc_context_set_logfunc(context, logfunction, this);

    descriptors = NULL;
    transports = NULL;
}

DCContext::~DCContext()
{
    if(descriptors) {
        FREE_VECTOR_PTR(descriptors)
    }
    if(transports) {
        FREE_VECTOR_PTR(transports)
    }
    dc_context_free(context);
}

void DCContext::setLogFunction(logfunc_t logFunc)
{
    logfunc = logFunc;
}

inline dc_loglevel_t translateLogLevel(loglevel_t loglevel)
{
    return (dc_loglevel_t)loglevel;
}

void DCContext::setLogLevel(loglevel_t level)
{
    logLevel = level;
    dc_context_set_loglevel(context, translateLogLevel(level));
}

dc_context_t *DCContext::getNative()
{
    return context;
}

void DCContext::log(logdata_t logdata)
{
    if (logfunc) {
        logfunc(logdata);
    }
}

DCContext *DCContext::clone()
{
    auto ctx = new DCContext();
    ctx->setLogLevel(logLevel);
    ctx->setLogFunction(logfunc);
    return ctx;
}

std::vector<DCDeviceDescriptor *> *DCContext::getDescriptors()
{
    if (!descriptors) {
        descriptors = loadDescriptors();
    }

    return descriptors;
}

std::vector<DCTransportInterface *> *DCContext::getTransports()
{
    if (!transports) {
        transports = loadTransports();
    }

    return transports;
}


std::vector<DCTransportInterface *> *DCContext::loadTransports()
{
    auto transports = new std::vector<DCTransportInterface *>();
    transports->push_back((DCTransportInterface *)new DCUSBHIDTransport(this));
    return transports;
}

std::vector<DCDeviceDescriptor *> *DCContext::loadDescriptors()
{
    dc_iterator_t *iter;
    dc_descriptor_t *descr;

    auto status = dc_descriptor_iterator(&iter);
    checkDCStatus(status);

    auto descriptors = new std::vector<DCDeviceDescriptor *>();
    while (dc_iterator_next(iter, &descr) == DC_STATUS_SUCCESS) {
        descriptors->push_back(createDescriptor(descr));
    }

    dc_iterator_free(iter);
    return descriptors;
}

DCDeviceDescriptor *DCContext::createDescriptor(dc_descriptor_t *descr)
{
    return new DCDeviceDescriptor(descr);
}

void DCContext::logfunction(
    dc_context_t *context,
    dc_loglevel_t loglevel,
    const char *file,
    unsigned int line,
    const char *function,
    const char *msg,
    void *userdata
) {
    auto ctx = (DCContext *)userdata;

    logdata_t logdata;
    logdata.function = function;
    logdata.loglevel = loglevel;
    logdata.file = file;
    logdata.line = line;
    logdata.msg = msg;
    ctx->log(logdata);
}
