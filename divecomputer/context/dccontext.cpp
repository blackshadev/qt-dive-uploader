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
}

DCContext::~DCContext()
{
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
