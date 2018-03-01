#ifndef DIVE_H
#define DIVE_H
#define EVENT_TYPE_COUNT 26
#include <libdivecomputer/parser.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>
#include "../common/list.h"
#include "../common/optional.h"

struct tank_event_t {
    unsigned int tank;
    double pressure;
};

class Event {
public:
    static const char* invalid_event_type_name;
    static const char* event_type_names[26];
    Event(unsigned int type, int value);
    unsigned int type;
    int value;
    const char* type_name;
};

class Sample {
public:
    int time;
    optional_t<int> bearing;
    optional_t<int> gasmix;
    optional_t<int> heartbeat;
    optional_t<int> rbt;
    optional_t<double> depth;
    optional_t<double> temperature;
    optional_t<double> cns;
    optional_t<double> setpoint;
    optional_t<double> ppo2;
    std::vector<Event> events;
    std::vector<tank_event_t> pressures;

};

class Dive
{
public:
    dc_datetime_t datetime;
    unsigned int divetime;
    double maxDepth;

    optional_t<double> maxTemperature;
    optional_t<double> minTemperature;
    optional_t<double> surfaceTemperature;
    optional_t<double> atmosphericPressure;
    optional_t<dc_salinity_t> salinity;
    optional_t<dc_divemode_t> divemode;

    List<unsigned char> fingerprint;
    List<dc_tank_t> tankPressures;
    List<dc_gasmix_t> gasMixures;

    std::vector<Sample*> samples;

    Dive();
    ~Dive();
    void parse(dc_parser_t* parser);
    void set_fingerprint(const unsigned char* fingerprint, unsigned int fsize);
    void process_sample(dc_sample_type_t type, dc_sample_value_t value);
protected:
    Sample* current_sample;

    void new_sample();

    template <typename T>
    static dc_status_t get_field(dc_parser_t* parser, dc_field_type_t type, T* field, unsigned int flag = 0);
    template <typename T>
    static dc_status_t get_optional_field(dc_parser_t* parser, dc_field_type_t type, optional_t<T>* field, unsigned int flag = 0);
    template <typename TData>
    static void get_list_field(dc_parser_t* parser, dc_field_type_t length_type, dc_field_type_t data_type, List<TData>* field);
};

#endif // DIVE_H
