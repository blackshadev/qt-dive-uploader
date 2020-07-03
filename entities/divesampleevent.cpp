#include "divesampleevent.h"

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


DiveSampleEvent::DiveSampleEvent()
{
    data.type = 0;
    data.value = 0;
}

unsigned int DiveSampleEvent::getType()
{
    return data.type;
}

void DiveSampleEvent::setType(unsigned int type)
{
    data.type = type;
}

const char *DiveSampleEvent::getTypeName()
{
    return event_type_names[data.type];
}

int DiveSampleEvent::getValue()
{
    return data.value;
}

void DiveSampleEvent::setValue(int value)
{
    data.value = value;
}
