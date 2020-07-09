#include "dccontext.h"


DCContext::DCContext()
{
    dc_context_new(&context);
    log = NULL;
    dc_context_set_logfunc(context, logfunction, this);
}

DCContext::~DCContext()
{

}

void DCContext::setLogFunction(logfunc_t logFunc)
{
    log = logFunc;
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

std::vector<DCDeviceDescriptor> DCContext::getDescriptors()
{

}

void DCContext::logfunction(dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
    auto ctx = (DCContext *)userdata;
    if (ctx->log == NULL) {
        return;
    }

    logdata_t logdata;
    logdata.function = function;
    logdata.loglevel = loglevel;
    logdata.file = file;
    logdata.line = line;
    logdata.msg = msg;
    ctx->log(logdata);
}
