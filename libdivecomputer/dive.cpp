#include "dive.h"
#include "../common/datetime.h"

const char* Event::invalid_event_type_name = "unknown";
const char* Event::event_type_names[EVENT_TYPE_COUNT] = {
    "none", "deco", "rbt", "ascent", "ceiling", "workload", "transmitter",
    "violation", "bookmark", "surface", "safety stop", "gaschange",
    "safety stop (voluntary)", "safety stop (mandatory)", "deepstop",
    "ceiling (safety stop)", "floor", "divetime", "maxdepth",
    "OLF", "PO2", "airtime", "rgbm", "heading", "tissue level warning",
    "gaschange2"
};

Event::Event(unsigned int t, int v)
{
    type = t;
    value = v;
    type_name = t < EVENT_TYPE_COUNT ? Event::event_type_names[t] : Event::invalid_event_type_name;
}

Dive::Dive()
{
    current_sample = NULL;
    datetime.timezone = 0;
}

Dive::~Dive()
{
    for(auto sample : samples) {
        delete sample;
    }
}

std::string Dive::iso_datetime() const
{
    return format_datetime_iso((dc_datetime_t&)datetime);
}

std::string Dive::display_datetime() const
{
    return format_datetime_display((dc_datetime_t&)datetime);
}

void Dive::parse(dc_parser_t* parser)
{
    dc_parser_get_datetime(parser, &datetime);

    get_field(parser, DC_FIELD_MAXDEPTH, &maxDepth);
    get_field(parser, DC_FIELD_DIVETIME, &divetime);

    get_optional_field(parser, DC_FIELD_TEMPERATURE_MAXIMUM, &maxTemperature);
    get_optional_field(parser, DC_FIELD_TEMPERATURE_MINIMUM, &minTemperature);
    get_optional_field(parser, DC_FIELD_TEMPERATURE_SURFACE, &surfaceTemperature);
    get_optional_field(parser, DC_FIELD_ATMOSPHERIC, &atmosphericPressure);
    get_optional_field(parser, DC_FIELD_SALINITY, &salinity);
    get_optional_field(parser, DC_FIELD_DIVEMODE, &divemode);

    get_list_field(parser, DC_FIELD_TANK_COUNT, DC_FIELD_TANK, &tankPressures);
    get_list_field(parser, DC_FIELD_GASMIX_COUNT, DC_FIELD_GASMIX, &gasMixures);

    dc_parser_samples_foreach(
        parser,
        [](dc_sample_type_t type, dc_sample_value_t value, void *userdata) {
        Dive* dive = (Dive*)userdata;
        dive->process_sample(type, value);
    }, this);

    if(current_sample != NULL) {
        samples.push_back(current_sample);
    }
}

void Dive::process_sample(dc_sample_type_t type, dc_sample_value_t value)
{
    switch(type) {
        case DC_SAMPLE_TIME:
            new_sample();
            current_sample->time = value.time;
        break;
        case DC_SAMPLE_DEPTH:
            current_sample->depth = value.depth;
        break;
        case DC_SAMPLE_TEMPERATURE:
            current_sample->temperature = value.temperature;
        break;
        case DC_SAMPLE_BEARING:
            current_sample->bearing = (int)value.bearing;
        break;
        case DC_SAMPLE_CNS:
            current_sample->cns = value.cns;
        break;
        case DC_SAMPLE_GASMIX:
            current_sample->gasmix = (int)value.gasmix;
        break;
        case DC_SAMPLE_HEARTBEAT:
            current_sample->heartbeat = (int)value.heartbeat;
        break;
        case DC_SAMPLE_PPO2:
            current_sample->ppo2 = value.ppo2;
        break;
        case DC_SAMPLE_PRESSURE:
            tank_event_t t;
            t.tank = (int)value.pressure.tank;
            t.pressure = value.pressure.value;
            current_sample->pressures.push_back(t);
        break;
        case DC_SAMPLE_RBT:
            current_sample->rbt = (int)value.rbt;
        break;
        case DC_SAMPLE_SETPOINT:
            current_sample->setpoint = value.setpoint;
        break;
        case DC_SAMPLE_EVENT:
            Event e(value.event.type, (int)value.event.value);
            current_sample->events.push_back(e);
        break;
    }

}

void Dive::new_sample()
{
    if(current_sample != NULL) {
        samples.push_back(current_sample);
    }
    current_sample = new Sample;
}


void Dive::set_fingerprint(const unsigned char* fdata, unsigned int fsize)
{
    fingerprint.set((unsigned char*)fdata, fsize);
}

template <typename T>
dc_status_t Dive::get_optional_field(dc_parser_t* parser, dc_field_type_t type,  optional_t<T>* field, unsigned int flag)
{
    auto st = get_field(parser, type, &field->value, flag);
    field->has_value = st == DC_STATUS_SUCCESS;
    return st;
}

template <typename T>
dc_status_t Dive::get_field(dc_parser_t* parser, dc_field_type_t type, T* field, unsigned int flag)
{
    return dc_parser_get_field(parser, type, flag, field);
}

template <typename TData>
void Dive::get_list_field(dc_parser_t* parser, dc_field_type_t length_type, dc_field_type_t data_type, List<TData>* field)
{
    unsigned int count;
    get_field(parser, length_type, &count);

    field->resize(count);

    for(unsigned int i = 0; i < count; i++) {
        get_field(parser, data_type, field->itemPtr(i), i);
    }

}
