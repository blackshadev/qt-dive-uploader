#ifndef DCCONTEXT_H
#define DCCONTEXT_H
#include "dcdevicedescriptor.h"
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

class DCTransport
{
public:
    enum Type {

    };
};

class DCContext
{
public:
    DCContext();
    ~DCContext();
    void setLogFunction(logfunc_t logFunc);
    void setLogLevel(loglevel_t logLevel);
    std::vector<DCDeviceDescriptor> getDescriptors();
    std::vector<DCTransport> getTransport(DCTransport::Type flag);
private:
    loglevel_t logLevel = DC_LOGLEVEL_ERROR;
    logfunc_t log;
    dc_context_t *context;
    static void logfunction(dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata);
};

#endif // DCCONTEXT_H
