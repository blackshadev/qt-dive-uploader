#ifndef DCCONTECTINTERFACE_H
#define DCCONTECTINTERFACE_H
#include <libdivecomputer/context.h>
typedef struct {
    dc_loglevel_t loglevel;
    const char *file;
    unsigned int line;
    const char *function;
    const char *msg;
} logdata_t;

class DCContextInterface
{
public:
    virtual dc_context_t *getNative() = 0;
    virtual void log(logdata_t logdata) = 0;
};

#endif // DCCONTECTINTERFACE_H
