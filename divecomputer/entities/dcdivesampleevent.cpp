#include "dcdivesampleevent.h"

#define EVENT_TYPE_COUNT 26
const char* invalid_event_type_name = "unknown";
const char* event_type_names[EVENT_TYPE_COUNT] = {
    "none", "deco", "rbt", "ascent", "ceiling", "workload", "transmitter",
    "violation", "bookmark", "surface", "safety stop", "gaschange",
    "safety stop (voluntary)", "safety stop (mandatory)", "deepstop",
    "ceiling (safety stop)", "floor", "divetime", "maxdepth",
    "OLF", "PO2", "airtime", "rgbm", "heading", "tissue level warning",
    "gaschange2"
};


DCDiveSampleEvent::DCDiveSampleEvent()
{
    data.type = 0;
    data.value = 0;
    data.flags = 0;
}

unsigned int DCDiveSampleEvent::getType()
{
    return data.type;
}

void DCDiveSampleEvent::setType(unsigned int type)
{
    data.type = type;
}

const char *DCDiveSampleEvent::getTypeName()
{
    return event_type_names[data.type];
}

int DCDiveSampleEvent::getValue()
{
    return data.value;
}

void DCDiveSampleEvent::setValue(int value)
{
    data.value = value;
}

unsigned int DCDiveSampleEvent::getFlags()
{
    return data.flags;
}

void DCDiveSampleEvent::setFlags(unsigned int flags)
{
    data.flags = flags;
}
